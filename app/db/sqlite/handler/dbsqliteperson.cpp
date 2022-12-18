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

GET_INSTANCE_IMPL(DbSqlitePerson)

DbSqlitePerson::DbSqlitePerson()
{
    traced;
}

ErrCode DbSqlitePerson::add(DbModel *model)
{

    traced;
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
                    saint->setPersonDbId(per->dbId());
                    saint->setPersonUid(per->uid());
                    saint->setSaintUid(item);
                    logd("Save saint/person, saint uid '%s'", item.toStdString().c_str());
                    err = saint->save();
                    // TODO: handle error case
                    delete saint;
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
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    tracedr(err);
    return err;
}

ErrCode DbSqlitePerson::update(DbModel *model)
{
    traced;
    ErrCode_t err = ErrNone;

    // TODO: should check if some sub-item not exist???
    // i.e.import person, but country, holly name, etc. not exist, need to check and add it

    if (model != nullptr){
        DbSqliteTbl* tbl = getMainTbl(); // TODO: check model name to get proper table??
        if (tbl->isExist(model)){
            err = tbl->update(model);
        } else {
            err = ErrNotExist;
            loge("model %s not exist", model->name().toStdString().c_str());
        }
    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    return err;
}

ErrCode DbSqlitePerson::add2Table(DbModel *model, DbSqliteTbl *tbl)
{
    traced;
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
    tracedr(err);
    return err;
}

bool DbSqlitePerson::exist(const DbModel *edu)
{
    traced;

    return getMainTbl()->isExist(edu);;
}

QList<DbModel *> DbSqlitePerson::getAll(DbModelBuilder builder, const char* modelName)
{
    traced;

    DbSqliteTbl* tbl = nullptr;
    logd("model name %s", modelName);
    QString name(modelName);
    if (name == KModelNamePerson) {
        //            tbl = getMainTbl();
        tbl = DbSqlite::table(KTablePerson);
    } else if (name == KModelNamePersonEvent){
        tbl = DbSqlite::table(KTablePersonEvent);
    } else {
        loge("Unknow model name %s", name.toStdString().c_str());
    }
    if (tbl != nullptr){
        return tbl->getAll(builder);
    } else {
        loge("Not found corresponding table");
        return QList<DbModel *>();
    }

}

QHash<QString, DbModel *> DbSqlitePerson::getAllInDict(DbModelBuilder builder, const char *modelName)
{
    traced;
    QList<DbModel *> list = getAll(builder, modelName);
    QHash<QString, DbModel *> map;
    logd("found %lld item", list.count());
    foreach (DbModel* item, list) {
        map.insert(item->uid(), item);
    }
    // TODO: cache it??????
    tracede;
    return map;
}

DbModel *DbSqlitePerson::getModel(qint64 dbId)
{
    traced;
    return nullptr;
}

ErrCode DbSqlitePerson::addEvent(const QString &personUid, const QString &eventUid,
                                 qint64 date, const QString& title, const QString &remark)
{
    traced;
    ErrCode ret = ErrNone;
    PersonEvent* pe = new PersonEvent();
    pe->setPersonUid(personUid);
    pe->setEventUid(eventUid);
    pe->setDate(date);
    pe->setName(title);
    pe->setRemark(remark);
    pe->save();
    tracedr(ret);
    return ret;
}

QList<PersonEvent *> *DbSqlitePerson::getListEvents(const QString& personUid,
                                                    const QString* eventUid,
                                                    qint64 date)
{
    traced;
    DbSqlitePersonEventTbl* tbl = (DbSqlitePersonEventTbl*)DbSqlite::table(KTablePersonEvent);
    return tbl->getListEvents(personUid, eventUid, date);
}

DbModel *DbSqlitePerson::getByName(const QString &name, const DbModelBuilder &builder)
{
    traced;
    // TODO: implement it
    logd("NOT IMPLEMENT YET");
    return nullptr;
}

DbModel *DbSqlitePerson::getByName(const QString &name)
{
    traced;
    return getByName(name, &Person::build);
}

int DbSqlitePerson::search(const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    return tbl->search(keyword, &Person::build, outList);
}

int DbSqlitePerson::filter(int fieldId, int operatorId, const QString &keyword, QList<DbModel *> *outList)
{
    traced;
    DbSqliteTbl* tbl = getMainTbl();
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);

    int ret = tbl->filter(fieldId, operatorId, keyword,  &Person::build, outList);
    tracedr(ret);
    return ret;
}

QList<DbModel *> DbSqlitePerson::getSpecialistList(const QString &personUid)
{
    traced;
    DbSqliteSpecialistPersonTbl* tbl = (DbSqliteSpecialistPersonTbl*)DbSqlite::getInstance()->getTable(KTableSpecialistPerson);
    // assume main tbl is not null, if not programming error,
    // and require override search function
    Q_ASSERT(tbl != nullptr);
    QList<DbModel *> list = tbl->getListSpecialist(personUid);
    logd("found %d", list.count());
    tracede;
    return list;
}

ErrCode DbSqlitePerson::updateCommunity(const QString &personUid, const QString &communityUid)
{
    traced;
    ErrCode ret = ErrNone;
    DbSqlitePersonTbl* tbl = (DbSqlitePersonTbl*)DbSqlite::getInstance()->getTable(KTablePerson);
    if (tbl) {
        ret = tbl->updateCommunity(personUid, communityUid);
    } else {
        loge("invalid DbSqlitePersonTbl");
        ret = ErrInvalidData;
    }
    tracedr(ret);
    return ret;
}

QList<DbModel *> DbSqlitePerson::getListPersonInCommunity(const QString &communityUid)
{
    traced;
    int cnt = 0;
    DbSqlitePersonTbl* tbl = (DbSqlitePersonTbl*)DbSqlite::getInstance()->getTable(KTablePerson);
    QHash<QString, int> inFields;
    inFields[KFieldCommunityUid] = TEXT;
    QList<DbModel*> outList;
    logd("Search wit community uid %s", STR2CHA(communityUid));
    cnt = tbl->search(communityUid, inFields, &Person::build, &outList, true);
    logd("Search community found %d", cnt);
    tracede;
    return outList;
}


const QString DbSqlitePerson::getName()
{
    return KModelHdlPerson;
}

DbSqliteTbl *DbSqlitePerson::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTablePerson);
}
