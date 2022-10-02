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
#include "personevent.h"

GET_INSTANCE_IMPL(DbSqlitePerson)

DbSqlitePerson::DbSqlitePerson()
{
    traced;
}

ErrCode DbSqlitePerson::add(const DbModel *model)
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
//            tbl = getMainTbl();
            tbl = DbSqlite::table(KTablePerson);
        } else if (name == KModelNamePersonEvent){
            tbl = DbSqlite::table(KTablePersonEvent);
        } else {
            loge("Unknow model name %s", name.toStdString().c_str());
        }

        if (tbl != nullptr){
            if (!tbl->isExist(model)){
                err = tbl->add(model);
            }
            else{
                err = ErrExisted;
                loge("Saint %s already exist", model->name().toStdString().c_str());
            }
        } else {
            err = ErrUnknown;
            loge("Not found corresponding table");
        }

    }
    else{
        err = ErrInvalidArg;
        loge("invalid argument");
    }



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


const QString DbSqlitePerson::getName()
{
    return KModelHdlPerson;
}

DbSqliteTbl *DbSqlitePerson::getMainTbl()
{
    return (DbSqliteTbl*)DbSqlite::getInstance()->getTable(KTablePerson);
}
