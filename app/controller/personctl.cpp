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
#include "event.h"
#include "dbpersonmodelhandler.h"
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


PersonCtl::PersonCtl():
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
