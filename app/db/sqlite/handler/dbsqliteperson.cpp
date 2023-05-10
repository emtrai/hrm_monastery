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

GET_INSTANCE_IMPL(DbSqlitePerson)

DbSqlitePerson::DbSqlitePerson()
{
    tracein;
}

ErrCode DbSqlitePerson::add(DbModel *model, bool notify)
{

    tracein;
    ErrCode_t err = ErrNone;

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
            // mapping saint - person
            logd("Check to add saint - person ");
            Person* per = dynamic_cast<Person*>(model);
            QStringList saintList = per->saintUidList();
            if (!saintList.empty()) {
                logd("set saintList for person, no saintList %d", saintList.count());

                foreach (QString item, saintList) {
                    SaintPerson* saint = (SaintPerson*)SaintPerson::build();
                    if (saint) {
                        saint->setPersonDbId(per->dbId());
                        saint->setPersonUid(per->uid());
                        saint->setSaintUid(item);
                        logd("Save saint/person, saint uid '%s'", item.toStdString().c_str());
                        err = saint->save();
                        if (err != ErrNone) {
                            loge("Save saint-person '%s' failed, err=%d",
                                 STR2CHA(saint->toString()), err);
                        }
                        // TODO: handle error case
                        delete saint;
                    } else {
                        loge("Create saint person faield,no memory");
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
                        logd("Save specialist/person, model uid '%s'", STR2CHA(spelistPerson->toString()));
                        err = spelistPerson->save();
                        if (err != ErrNone) {
                            loge("Save specialist-person '%s' failed, err=%d",
                                 STR2CHA(model->toString()), err);
                        }
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
                        logd("Save person event '%s'", STR2CHA(event->toString()));
                        err = event->save();
                        if (err != ErrNone) {
                            loge("Save event-person '%s' failed, err=%d",
                                 STR2CHA(model->toString()), err);
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
    traceret(err);
    return err;
}

//ErrCode DbSqlitePerson::update(DbModel *model)
//{
//    tracein;
//    ErrCode_t err = ErrNone;

//    // TODO: should check if some sub-item not exist???
//    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

//    if (model != nullptr){
//        DbSqliteTbl* tbl = getMainTbl(); // TODO: check model name to get proper table??
//        if (tbl->isExist(model)){
//            err = tbl->update(model);
//        } else {
//            err = ErrNotExist;
//            loge("model %s not exist", model->name().toStdString().c_str());
//        }
//    }
//    else{
//        err = ErrInvalidArg;
//        loge("invalid argument");
//    }

//    return err;
//}

ErrCode DbSqlitePerson::add2Table(DbModel *model, DbSqliteTbl *tbl)
{
    tracein;
    ErrCode err = ErrNone;
    if (tbl != nullptr){
        if (!tbl->isExist(model)){
            err = tbl->add(model);
        }
        else{
            err = ErrExisted;
            loge("Person %s already exist", model->name().toStdString().c_str());
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
        logd("Delete hard model '%s', force %d", STR2CHA(model->toString()), force);

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
    if (name == KModelNamePerson || name.isEmpty()) {
        //            tbl = getMainTbl();
        tbl = DbSqlite::table(KTablePerson);
    } else if (name == KModelNamePersonEvent){
        tbl = DbSqlite::table(KTablePersonEvent);
    } else {
        loge("Unknow model name %s", name.toStdString().c_str());
    }
    if (tbl != nullptr){
        return tbl->getAll(builder, status, from, noItems, total);
    } else {
        loge("Not found corresponding table");
        return QList<DbModel *>();
    }

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

    ErrCode ret = tbl->filter(fieldId, operatorId, keyword,  &Person::build, outList,
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

ErrCode DbSqlitePerson::update(DbModel *model)
{
    tracein;
    ErrCode err = ErrNone;
    err = DbSqliteModelHandler::update(model);
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
                        err = model->save();
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
                                err = event->save();
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
                                        err = currentEvent->update(true);
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
                                        err = newModel->save();
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
    traceret(err);
    return err;
}

ErrCode DbSqlitePerson::update(DbModel *model, const QHash<QString, QString> &inFields, const QString &tableName)
{
    return DbSqliteModelHandler::update(model, inFields, tableName);
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
