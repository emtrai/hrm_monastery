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
 * Filename: personctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "personctl.h"
#include "dbctl.h"
#include "logger.h"
#include "errcode.h"
#include "logger.h"
#include "utils.h"
#include "person.h"
#include "personevent.h"
#include "filectl.h"
#include "specialistctl.h"
#include "event.h"
#include "dbpersonmodelhandler.h"
#include "filter.h"
#include "dbspecialistmodelhandler.h"

PersonCtl* PersonCtl::gInstance = nullptr;

ErrCode PersonCtl::addPerson(Person *person)
{
    traced;
    ErrCode_t err = ErrNone;
    // TODO: check null
    err = person->save();

    return err;
}

ErrCode PersonCtl::addPerson(const QString &fname)
{
    traced;
    Person *person = new Person();
    ErrCode ret = person->fromCSVFile(fname);
    // TODO: should call Config to update next person code in case of saving new person
    // succeed??
    // reason: when import, imported person will have code, but it may not be saved to db

    if (ret == ErrNone){
        ret = person->save();
    }

    delete person;
    tracedr(ret);
    return ret;
}

ErrCode PersonCtl::AddListPersons(const QString &fname)
{
    traced;
    return ErrNone;
}

QList<DbModel *> PersonCtl::getAllPerson()
{
    traced;
    return modelHdl()->getAll(&Person::build, KModelNamePerson);
}

QList<DbModel *> PersonCtl::getListEvent(const Person* person)
{
    traced;
    return modelHdl()->getAll(&PersonEvent::build, KModelNamePersonEvent);
}

DbModel *PersonCtl::doImportOneItem(int importFileType, const QStringList &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    Person* person = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    logd("no items %lld", items.count());
    if (idx == 0) {
        logd("HEADER, save it");
        foreach (QString item, items) {
            logd("Header %s", item.toStdString().c_str());
            mImportFields.append(item.trimmed());
        }
    } else {

        person = (Person*)Person::build();
        foreach (QString item, items) {
            QString field = mImportFields[i++];
            logd("Import field %s", field.toStdString().c_str());
            ret = person->onImportItem(importFileType, field, item, idx);
        }
    }

    tracedr(ret);
    return person;
}

DbModel *PersonCtl::doImportOneItem(int importFileType, const QHash<QString, QString> &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    Person* person = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    person = (Person*)Person::build();
    foreach (QString field, items.keys()) {
        QString value = items.value(field);
        logd("Import field %s", field.toStdString().c_str());
        logd("Import value %s", value.toStdString().c_str());
        ret = person->onImportItem(importFileType, field, value, idx);
    }

    tracedr(ret);
    return person;
}

int PersonCtl::filter(int catetoryid, const QString &catetory, qint64 opFlags, const QString &keywords, QList<DbModel *> *outList)
{
    traced;
    int ret = 0;
    logd("category id %d", catetoryid);
    if (catetoryid == FILTER_FIELD_SPECIALIST) {
        logd("filder by specialist");
        QList<DbModel *> specialistList;
        ret = SPECIALISTCTL->filter(catetoryid, catetory, opFlags, keywords, &specialistList);
        logd("Found %d specialist", ret);
        DbSpecialistModelHandler* specialistHdl = dynamic_cast<DbSpecialistModelHandler*>(DB->getModelHandler(KModelHdlSpecialist));
        if (ret > 0){
            logd("search person for each specialist");
            foreach (DbModel* item, specialistList) {

                logd("specialist uid %s, name %s",
                     item->uid().toStdString().c_str(),
                     item->name().toStdString().c_str());
                QList<DbModel *> perList = specialistHdl->getListPerson(item->uid());
                if (perList.count() > 0) {
                    logd("found %d person", perList.count());
                    outList->append(perList);
                } else {
                    logd("not found any person");
                }
            }
        } else {
            logi("No specialist found");
        }
    } else {
        logd("generic filter");
        ret = Controller::filter(catetoryid, catetory, opFlags, keywords, outList);
    }
    tracedr(ret);
    return ret;
}


QList<DbModel *> PersonCtl::getSpecialistList(const QString &personUid)
{
    traced;
    DbPersonModelHandler* hdl = dynamic_cast<DbPersonModelHandler*>(getModelHandler());
    logd("get specialist list of person uid %s", personUid.toStdString().c_str());
    QList<DbModel*> list = hdl->getSpecialistList(personUid);
    logd("No. item: %d", list.count());
    return list;
}

PersonCtl::PersonCtl():
    Controller(KModelHdlPerson),
    mModelHdl(nullptr)
{
}


PersonCtl::~PersonCtl()
{
    traced;
}


PersonCtl *PersonCtl::getInstance()
{
    if (gInstance == nullptr){
        gInstance = new PersonCtl();
    }

    return gInstance;
}

const QList<QString> &PersonCtl::importFields() const
{
    return mImportFields;
}

QList<Person *> PersonCtl::searchPerson(const QString &keyword)
{
    traced;
    logd("search persone by name %s", keyword.toStdString().c_str());
    // TODO:
    QList<DbModel*> list;
    QList<Person*> listret;
    int ret = modelHdl()->search(keyword, &list);
    logd("Search result %d", ret);
    logd("no item %d", list.count());
    if (ret > 0) {
        foreach(DbModel* item, list) {
            listret.append(dynamic_cast<Person*>(item));
        }
    }
    return listret;
}

DbPersonModelHandler *PersonCtl::modelHdl()
{
    if (mModelHdl == nullptr) {
        mModelHdl = dynamic_cast<DbPersonModelHandler*>(DB->getModelHandler(KModelHdlPerson));
    }
    return mModelHdl;
}

DbModelHandler *PersonCtl::getModelHandler()
{
    return modelHdl();
}

void PersonCtl::onLoad()
{
    traced;
}
