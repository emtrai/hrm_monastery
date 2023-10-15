/*
 * Copyright (C) 2022 Ngo Huy Anh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * Filename: dbsqliteperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/4/2022
 * Brief:
 */
#include "dbsqliteperson.h"
#include "logger.h"
#include "defs.h"

#include "dbsqlitedefs.h"
#include "dbsqlite.h"
#include "dbsqlitepersonevent.h"
#include "table/dbsqlitecommunitypersontbl.h"
#include "table/dbsqlitepersoneventtbl.h"
#include "table/dbsqlitespecialistpersontbl.h"
#include "personevent.h"
#include "model/saintperson.h"
#include "model/specialistperson.h"
#include "dbctl.h"
#include "table/dbsqlitepersontbl.h"
#include "person.h"
#include "community.h"
#include "areaperson.h"
#include "communityperson.h"
#include "saintperson.h"
#include "specialistperson.h"
#include "dbsqlite.h"
#include "exception.h"
#include "dbcommunitymodelhandler.h"

GET_INSTANCE_IMPL(DbSqlitePerson)

DbSqlitePerson::DbSqlitePerson()
{
    tracein;
}

ErrCode DbSqlitePerson::add(DbModel *model, bool notify)
{

    tracein;
    ErrCode_t err = ErrNone;
    Community* comm = nullptr;

    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

    if (model != nullptr){
        DbSqliteTbl* tbl = nullptr;
        QString name = model->modelName();
        logd("model name %s", name.toStdString().c_str());
        if (name == KModelNamePerson) {
            tbl = DbSqlite::table(KTablePerson);
        } else if (name == KModelNamePersonEvent){
            tbl = DbSqlite::table(KTablePersonEvent);
        } else if (name == KModelNameSaintPerson){
            tbl = DbSqlite::table(KTableSaintPerson);
        } else {
            loge("Unknow model name %s", name.toStdString().c_str());
        }

        err = add2Table(model, tbl);
        if (err == ErrNone && name == KModelNamePerson) {

            Person* per = dynamic_cast<Person*>(model);
            // save community info
            logd("Check to add community");
            check2UpdateCommunity(per);

            // mapping saint - person
            logd("Check to add saint - person ");
            QStringList saintList = per->saintUidList();
            if (!saintList.empty()) {
                logd("set saintList for person, no saintList %d", saintList.count());

                foreach (QString item, saintList) {
                    SaintPerson* saint = (SaintPerson*)SaintPerson::build();
                    if (saint) {
                        saint->setPersonDbId(per->dbId());
                        saint->setPersonUid(per->uid());
                        saint->setSaintUid(item);
                        logi("Save saint/person, saint uid '%s'", MODELSTR2CHA(saint));
                        err = saint->save(false);
                        logife(err, "Save saint-person '%s' failed for model '%s'",
                               MODELSTR2CHA(saint), MODELSTR2CHA(model));
                        // TODO: handle error case
                        delete saint;
                    } else {
                        loge("Create saint person failed,no memory");
                    }
                }
            } else {
                logd("no saint for person");
            }

            // map person specialist
            logd("Check to add person - specialist ");
            QStringList list = per->specialistUidList();
            if (!list.empty()) {
                logd("set specialist for person, no. specialist %d", list.count());

                foreach (QString item, list) {
                    SpecialistPerson* spelistPerson = (SpecialistPerson*)SpecialistPerson::build();
                    if (spelistPerson) {
                        spelistPerson->setPersonUid(per->uid());
                        spelistPerson->setSpecialistUid(item);
                        logi("Save specialist/person, model uid '%s'",
                             MODELSTR2CHA(spelistPerson));
                        err = spelistPerson->save(false);
                        logife(err, "Save specialist-person '%s' failed for model '%s'",
                               MODELSTR2CHA(spelistPerson), MODELSTR2CHA(model));
                        // TODO: handle error case
                        delete spelistPerson;
                    } else {
                        loge("Create SpecialistPerson faield,no memory");
                    }
                }
            } else {
                logd("no specialist for person");
            }

            // person event
            logd("Save person event");
            const QList<DbModel*> events = per->personEventList();
            if (!events.empty()) {
                foreach (DbModel* item, events) {
                    PersonEvent* event = (PersonEvent*)item;
                    if (event) {
                        event->setPersonUid(per->uid());
                        logi("Save person event '%s'", MODELSTR2CHA(event));
                        err = event->save(false);
                        if (err != ErrNone) {
                            logife(err, "Save event-person '%s' failed for model '%s'",
                                   MODELSTR2CHA(event), MODELSTR2CHA(model));
                        }
                    } else {
                        loge("invalid person event, null value");
                    }
                }
            }

        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && notify) {
        notifyDataChange(model, DBMODEL_CHANGE_ADD, err);
    }
    FREE_PTR(comm);
    traceret(err);
    return err;
}

ErrCode DbSqlitePerson::add2Table(DbModel *model, DbSqliteTbl *tbl)
{
    tracein;
    ErrCode err = ErrNone;
    if (tbl != nullptr){
        if (!tbl->isExist(model)){
            logi("Add model '%s' to db table", MODELSTR2CHA(model));
            err = tbl->add(model);
        } else {
            err = ErrExisted;
            loge("Item '%s' already exist in table '%s'",
                 MODELSTR2CHA(model), STR2CHA(tbl->name()));
        }
    } else {
        err = ErrUnknown;
        loge("Not found corresponding table");
    }
    traceret(err);
    return err;
}

ErrCode DbSqlitePerson::deleteSoft(DbModel *model)
{
    tracein;
    ErrCode err = ErrNone;
    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

    if (model != nullptr){
        DbSqliteTbl* tbl = getMainTbl();
        if (tbl->isExist(model)){
            err = tbl->deleteSoft(model);
        } else {
            err = ErrNotExist;
            loge("model %s not exist", model->name().toStdString().c_str());
        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    traceret(err);
    return err;
}

ErrCode DbSqlitePerson::deleteHard(DbModel *model, bool force, QString* msg)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        err = ErrInvalidArg;
        loge("Invalid model");
    }

    if (err == ErrNone) {
        logi("Delete hard model '%s', force %d", STR2CHA(model->toString()), force);

        if (model->modelName() == KModelNamePerson){
            // KFieldAreaUid delete map, community, person
            QHash<QString, QString> itemToSearch; // for searching
            QHash<QString, QString> itemToSearchInComm; // for searching
            QHash<QString, QString> itemToSetInComm; // for searching
            QHash<QString, QString> itemToSearchInPerson; // for searching
            QHash<QString, QString> itemToSetInPerson; // for searching
            QHash<QString, QString> itemToSet; // for update
            QList<DbModel*> list;
            int count = 0;
            ErrCode tmpErr = ErrNone;
            bool errDependency = false;

            itemToSearch.insert(KFieldPersonUid, model->uid());
            itemToSearchInPerson.insert(KFieldVowsCEOUid, model->uid());
            itemToSearchInPerson.insert(KFieldEternalVowsPICUid, model->uid());
            itemToSearchInPerson.insert(KFieldEternalVowsCEOUid, model->uid());
            itemToSearchInComm.insert(KFieldCEOUid, model->uid());
            itemToSet.insert(KFieldPersonUid, ""); // update to null/emptys
            itemToSetInComm.insert(KFieldCEOUid, ""); // update to null/emptys
            itemToSetInPerson.insert(KFieldVowsCEOUid, ""); // update to null/emptys
            itemToSetInPerson.insert(KFieldEternalVowsPICUid, ""); // update to null/emptys
            itemToSetInPerson.insert(KFieldEternalVowsCEOUid, ""); // update to null/emptys

            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency, msg, force, itemToSearchInPerson, itemToSetInPerson, KTablePerson, &Person::build);
            CHECK_REMOVE_TO_CLEAR_DATA(err, errDependency, msg, force, itemToSearchInComm, itemToSetInComm, KTableCommunity, &Community::build);
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch, KTableCommPerson, &CommunityPerson::build);
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch, KTableAreaPerson, &AreaPerson::build);
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch, KTableSaintPerson, &SaintPerson::build);
            CHECK_REMOVE_TO_DELETE(err, errDependency, msg, force, itemToSearch, KTableSpecialistPerson, &SpecialistPerson::build);

            if (errDependency) {
                err = ErrDependency;
                loge("cannot delete, has dependency '%s'", msg?STR2CHA((*msg)):"");
            } else {
                err = DbSqliteModelHandler::deleteHard(model, force, msg);
            }
        }
    }
    traceout;
    return err;
}

bool DbSqlitePerson::exist(const DbModel *edu)
{
    tracein;

    return getMainTbl()->isExist(edu);;
}

QList<DbModel *> DbSqlitePerson::getAll(DbModelBuilder builder, qint64 status,
                                        const char* modelName, int from,
                                        int noItems, int* total)
{
    tracein;

    DbSqliteTbl* tbl = nullptr;
    logd("model name %s", modelName);
    QString name(modelName);
    QList<DbModel *> ret;
    if (name == KModelNamePerson || name.isEmpty()) {
        //            tbl = getMainTbl();
        tbl = DbSqlite::table(KTablePerson);
    } else if (name == KModelNamePersonEvent){
        tbl = DbSqlite::table(KTablePersonEvent);
    } else {
        loge("Unknow model name %s", name.toStdString().c_str());
    }
    if (tbl != nullptr){
        ret = tbl->getAll(builder, status, from, noItems, total);
    } else {
        loge("Not found corresponding table");
    }
    traceout;
    return ret;
}

QHash<QString, DbModel *> DbSqlitePerson::getAllInDict(DbModelBuilder builder, qint64 status, const char *modelName)
{
    tracein;
    QList<DbModel *> list = getAll(builder, status, modelName);
    QHash<QString, DbModel *> map;
    logd("found %lld item", list.count());
    foreach (DbModel* item, list) {
        map.insert(item->uid(), item);
    }
    // TODO: cache it??????
    traceout;
    return map;
}

DbModel *DbSqlitePerson::getModel(qint64 dbId)
{
    tracein;
    return nullptr;
}

ErrCode DbSqlitePerson::addEvent(const QString &personUid, const QString &eventUid,
                                 qint64 date, const QString& title, const QString &remark)
{
    tracein;
    ErrCode ret = ErrNone;
    PersonEvent* pe = new PersonEvent();
    pe->setPersonUid(personUid);
    pe->setEventUid(eventUid);
    pe->setDate(date);
    pe->setName(title);
    pe->setRemark(remark);
    pe->save();
    traceret(ret);
    return ret;
}

ErrCode DbSqlitePerson::getListEvents(const QString& personUid,
                                                   QList<DbModel*>& list,
                                                    const QString* eventUid,
                                                    qint64 date)
{
    tracein;
    DbSqlitePersonEventTbl* tbl = (DbSqlitePersonEventTbl*)DbSqlite::table(KTablePersonEvent);
    return tbl->getListEvents(personUid, list, eventUid, date);
}


ErrCode DbSqlitePerson::search(const QString &keyword,
                           QList<DbModel *> *outList,
                           qint64 dbStatus,
                           int from,
                           int noItems,
                           int* total)
{
    tracein;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    return tbl->search(keyword, &Person::build, outList, dbStatus, from, noItems, total);
}

ErrCode DbSqlitePerson::filter(int fieldId, int operatorId,
                               const QString &keyword,
                               const char* targetModelName,
                               const DbModel* parentModel,
                               QList<DbModel *> *outList,
                               qint64 dbStatus,
                               int from,
                               int noItems,
                               int* total)
{
    tracein;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);

    ErrCode ret = tbl->filter(fieldId, operatorId, keyword,  &Person::build,
                              parentModel, outList,
                              dbStatus, from, noItems, total);
    traceret(ret);
    return ret;
}

QList<DbModel *> DbSqlitePerson::getSpecialistList(const QString &personUid)
{
    tracein;
    DbSqliteSpecialistPersonTbl* tbl = (DbSqliteSpecialistPersonTbl*)DbSqlite::getInstance()->getTable(KTableSpecialistPerson);
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    QList<DbModel *> list = tbl->getListSpecialist(personUid);
    QList<DbModel *> outList;
    foreach (DbModel* model, list) {
        if (model) {
            if (model->modelName() == KModelNameSpecialistPerson) {
                SpecialistPerson* specialist = (SpecialistPerson*) model;
                if (specialist->specialist())
                    outList.push_back(specialist->specialist()->clone());
            } else if (model->modelName() == KModelNameSpecialist) {
                outList.push_back(model);
            }
        }
    }
    RELEASE_LIST_DBMODEL(list);
    logd("found %d", outList.count());
    traceout;
    return outList;
}

ErrCode DbSqlitePerson::updateCommunity(const QString &personUid, const QString &communityUid)
{
    tracein;
    ErrCode ret = ErrNone;
    DbSqlitePersonTbl* tbl = (DbSqlitePersonTbl*)DbSqlite::getInstance()->getTable(KTablePerson);
    if (tbl) {
        ret = tbl->updateCommunity(personUid, communityUid);
    } else {
        loge("invalid DbSqlitePersonTbl");
        ret = ErrInvalidData;
    }
    traceret(ret);
    return ret;
}

ErrCode DbSqlitePerson::getListPersonInCommunity(const QString &communityUid, qint32 status, QList<DbModel*>& list)
{
    tracein;
    ErrCode err = ErrNone;
    DbSqlitePersonTbl* tbl = (DbSqlitePersonTbl*)DbSqlite::getInstance()->getTable(KTablePerson);
    QHash<QString, int> inFields;
    inFields[KFieldCommunityUid] = TEXT;
    logd("Search wit community uid %s", STR2CHA(communityUid));
    err = tbl->search(communityUid, inFields, &Person::build, &list, true);
    logd("Search community err=%d", err);
    traceout;
    return err;
}

ErrCode DbSqlitePerson::getListCommunitesOfPerson(const QString &perUid,
                                                  qint32 modelStatus, QList<CommunityPerson *> &list)
{
    tracein;
    ErrCode err = ErrNone;
    logd("get list community of person uid '%s'", STR2CHA(perUid));
    if(!perUid.isEmpty()) {
        DbSqliteCommunityPersonTbl* tbl = (DbSqliteCommunityPersonTbl*)DbSqlite::table(KTableCommPerson);
        if (tbl) {
            list = tbl->getListCommunityOfPerson(perUid, modelStatus);
        } else {
            loge("Not found table '%s'", KTableCommPerson);
            err = ErrNoTable;
        }
    } else {
        loge("invalid perUid '%s'", STR2CHA(perUid));
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        logd("found %d item, let get community", list.size());
        foreach (CommunityPerson* item, list) {
            if (item) {
                logd("get community id '%s'", STR2CHA(item->communityUid()));
                DbModel* community = DB->getCommunityModelHandler()->getByUid(item->communityUid());
                if (community) {
                    logd("set community '%s'", MODELSTR2CHA(community));
                    item->setCommunity((Community*)community);
                } else {
                    logw("not found community id '%s' on db", STR2CHA(item->communityUid()));
                }
            } else {
                logw("something stupid occurs, null in list????");
            }
        }
    }
    traceout;
    return err;
}

ErrCode DbSqlitePerson::update(DbModel *model, bool notify)
{
    tracein;
    ErrCode err = ErrNone;
    if (!model) {
        loge("Invalid model, null one");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        err = DbSqliteModelHandler::update(model, false);
    }
    if (err == ErrNone && (model->modelName() == KModelNamePerson)) {
        Person* per = (Person*) model;
        if (per->isFieldUpdated(KItemSpecialist)) {
            logd("field '%s' is updated", KItemSpecialist);
            DbSqliteTbl* tbl = DbSqlite::table(KTableSpecialistPerson);
            QHash<QString, QString> condition;
            condition.insert(KFieldPersonUid, model->uid());
            logd("Delete old mapping specilist vs version for person");
            err = tbl->deleteHard(condition);
            if (err == ErrNone) {
                // map person specialist
                logd("Check to add person - specialist ");
                QStringList list = per->specialistUidList();
                if (!list.empty()) {
                    logd("set specialist for person, no. specialist %d", list.count());

                    foreach (QString item, list) {
                        SpecialistPerson* model = (SpecialistPerson*)SpecialistPerson::build();
                        model->setPersonUid(per->uid());
                        model->setSpecialistUid(item);
                        logd("Save specialist/person, model uid '%s'", item.toStdString().c_str());
                        err = model->save(false);
                        // TODO: handle error case
                        delete model;
                    }
                } else {
                    logd("no specialist for person");
                }
            }
        } else {
            logd("field %s is not updated", KItemSpecialist);
        }

        if (per->isFieldUpdated(KItemCommunity)) {
            logd("Check to update community");
            check2UpdateCommunity(per);
            // TODO: handle error case?
            // currently, jus keep & continue other changes
        } else {
            logd("skip update community");
        }

        if (per->isFieldUpdated(KItemPersonEvent) && per->personEventListUpdated()) {
            // person event
            logd("Update person event");
            QList<DbModel*> currentEvents;
            err = getListEvents(per->uid(), currentEvents);
            const QHash<QString, DbModel*> events = per->personEventMap();
            if (err == ErrNone) {
                if (currentEvents.empty()) {
                    if (!events.empty()) {
                        logi("current list of person event empty, "
                             "but updated list has: add all as new");
                        foreach (DbModel* item, events.values()) {
                            PersonEvent* event = (PersonEvent*)item;
                            if (event) {
                                event->setPersonUid(per->uid());
                                logd("Save person event '%s'", STR2CHA(event->toString()));
                                err = event->save(false);
                                if (err != ErrNone) {
                                    loge("Save event-person '%s' failed, err=%d",
                                         STR2CHA(model->toString()), err);
                                }
                            } else {
                                loge("invalid person event, null value");
                            }
                        }
                    } else {
                        logi("all empty, nothing to update person event");
                    }
                } else {
                    if (events.empty()) {
                        logi("update list empty, remove all current events");
                        foreach (DbModel* item, currentEvents) {
                            PersonEvent* event = (PersonEvent*)item;
                            if (event) {
                                event->setPersonUid(per->uid());
                                logd("remove person event '%s'", STR2CHA(event->toString()));
                                err = event->remove();
                                if (err != ErrNone) {
                                    loge("remove event-person '%s' failed, err=%d",
                                         STR2CHA(model->toString()), err);
                                }
                            } else {
                                loge("invalid person event, null value");
                            }
                        }
                    } else {
                        QList<QString> keyList;
                        DbModel* updateEvent = nullptr;
                        logi("Check current event with update event to save it");
                        foreach(DbModel* currentEvent, currentEvents) {
                            updateEvent = nullptr;
                            logd("currentEvent '%s'", MODELSTR2CHA(currentEvent));
                            if (events.contains(currentEvent->nameId())) {
                                logd("current event nameid '%s' exist in updated event",
                                     STR2CHA(currentEvent->nameId()));
                                updateEvent = events.value(currentEvent->nameId());
                                if (updateEvent) {
                                    logd("copy data from update event '%s' to current event '%s'",
                                         MODELSTR2CHA(updateEvent), MODELSTR2CHA(currentEvent));
                                    currentEvent->setMarkModified(true);
                                    err = currentEvent->copyData(updateEvent);
                                    if (err == ErrNone) {
                                        err = currentEvent->update(true, false);
                                    } else {
                                        loge("failed to copy update data, out");
                                        break;
                                    }
                                }
                            } else {
                                logd("current event not exist in updated one, delete it '%s'",
                                     MODELSTR2CHA(currentEvent));
                                err = currentEvent->remove();
                            }
                            if (err == ErrNone) {
                                logd("add '%s' to keyList", STR2CHA(currentEvent->nameId()));
                                keyList.append(currentEvent->nameId());
                            } else {
                                break;
                            }
                        }
                        if (err == ErrNone) {
                            logd("check if all keys in upated event are updated");
                            foreach(QString key, events.keys()) {
                                if (!keyList.contains(key)) {
                                    logd("key '%s' to exist yet, save as new data");
                                    DbModel* newModel = events.value(key);
                                    if (newModel) {
                                        err = newModel->save(false);
                                    } else {
                                        loge("newModel with key '%s' is empty",
                                             STR2CHA(key));
                                        err = ErrNoData;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else {
            logi("not update person even, maybe person is not updated"
                 "isFieldUpdated=%d personEventListUpdated=%d",
                 per->isFieldUpdated(KItemPersonEvent), per->personEventListUpdated());
        }
    }
    if (err == ErrNone && notify) {
        notifyDataChange(model, DBMODEL_CHANGE_UPDATE, err);
    }
    traceret(err);
    return err;
}

// this function is called mainly for updating data when delete model, so should
// we update data like community change here??? let God answer, Godbless us
ErrCode DbSqlitePerson::update(DbModel *model, const QHash<QString, QString> &inFields,
                               const QString &tableName, bool notifyDataChange)
{
    return DbSqliteModelHandler::update(model, inFields, tableName, notifyDataChange);
}

ErrCode DbSqlitePerson::check2UpdateCommunity(Person* per)
{
    tracein;
    ErrCode err = ErrNone;
    DbCommunityModelHandler* hdlComm = dynamic_cast<DbCommunityModelHandler*>(DB->getCommunityModelHandler());
    const Community* rootComm = nullptr;
    Community* comm = nullptr;
    if (!per) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && !hdlComm) {
        err = ErrNoHandler;
        loge("not found community handler");
    }
    if (err == ErrNone) {
        logd("get root community");
        rootComm = hdlComm->getRootCommunity();
    }
    // save community info
    logd("Check to add community");
    if (err == ErrNone) {
        if (!per->communityUid().isEmpty()) {
            logd("get community for uid '%s'", STR2CHA(per->communityUid()));
            comm = static_cast<Community*>(DB->getCommunityModelHandler()->getByUid(per->communityUid()));
            logd("community '%s'", MODELSTR2CHA(comm));
            if (!comm) {
                loge("communit uid '%s' is set, but not found!",
                     STR2CHA(per->communityUid()));
                err = ErrNotFound;
            }
        } else { // empty, use root community
            if (rootComm) {
                logi("No community uid was found, use root comm one");
                comm = CLONE_MODEL(rootComm, Community);
                if (!comm) {
                    loge("cannot clone root comm, no memory?");
                    err = ErrNoMemory;
                }
            } else {
                logw("No root community to add");
            }
        }
    }
    if (err == ErrNone && comm) {
        logi("Add per '%s' to comm '%s'", MODELSTR2CHA(per), MODELSTR2CHA(comm));
        err = hdlComm->addPerson2Community(comm, per, true,
                                                      MODEL_STATUS_ACTIVE, 0, 0,
                                                      nullptr, false);
        // TODO: handle error case?
        logife(err, "Failed to add comm '%s' to person '%s'",
               MODELSTR2CHA(comm), MODELSTR2CHA(per));

    } else {
        logd("No community to add for person '%s'", MODELSTR2CHA(per));
    }
    logife(err, "Failed to add comm '%s' to person '%s'",
           MODELSTR2CHA(comm), MODELSTR2CHA(per));
    FREE_PTR(comm);
    traceret(err);
    return err;
}

int DbSqlitePerson::getTotalItemsByPersonStatus(const QString &statusUid)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    int ret = 0;
    tbl = DbSqlite::table(KTablePerson);
    logd("statusUid '%s'", STR2CHA(statusUid));
    if (tbl != nullptr){
        ret = (static_cast<DbSqlitePersonTbl*>(tbl))->getTotalItemsByPersonStatus(statusUid);
    } else {
        loge("Not found table '%s'", KTablePerson);
        ret = -(ErrNoTable);
    }
    traceret(ret);
    return ret;
}


const QString DbSqlitePerson::getName()
{
    return KModelHdlPerson;
}

DbSqliteTbl *DbSqlitePerson::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTablePerson);
}

DbModelBuilder DbSqlitePerson::getMainBuilder()
{
    return Person::build;
}

DbSqliteTbl *DbSqlitePerson::getTable(const QString &modelName)
{
    tracein;
    DbSqliteTbl* tbl = nullptr;
    logd("modelname '%s'", modelName.toStdString().c_str());
    if (modelName == KModelNamePerson || modelName.isEmpty()) {
        tbl = DbSqlite::table(KTablePerson);
    } else if (modelName == KModelNamePersonEvent) { // TODO: check & implement more??
        tbl = DbSqlite::table(KTablePersonEvent);
    } else { // TODO: check & implement more??
        tbl = getMainTbl();
    }
    traceout;
    return tbl;
}
