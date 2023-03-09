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
 * Filename: personctl.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef PersonCTL_H
#define PersonCTL_H

#include <QObject>
#include "errcode.h"
#include "modelcontroller.h"
#include <QHash>
#include "exporttype.h"
#include "modelcontroller.h"
class Person;
class Event;
class DbPersonModelHandler;

#define PERSONCTL PersonCtl::getInstance()

// TODO: observer Person change?
class PersonCtl: public ModelController
{
public:
    virtual ~PersonCtl();
    ErrCode addPerson(Person* person);
    ErrCode addPerson (const QString& fname);
    ErrCode markPersonDelete(Person* person);
    ErrCode deletePerson(Person* person);
    ErrCode AddListPersons(const QString& fname);
    QList<DbModel*> getAllPerson();
    QList<DbModel*> getPersonInCommunity(const QString& communityUid);
    QList<DbModel*> getListEvent(const Person* person); // TODO: should move to separate event controller?

    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QStringList& items, quint32 idx);
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QHash<QString, QString>& items, quint32 idx);

    virtual int filter(int catetoryid,
                       const QString& catetory,
                       qint64 opFlags,
                       const QString& keywords,
                       QList<DbModel*>* outList = nullptr);

    // TODO: consider this in specialist or in person????
    QList<DbModel *> getSpecialistList(const QString &personUid);

    virtual const QString exportTemplatePath(Exporter* exporter) const;
    virtual ErrCode getExportDataString(const QString& keyword, const DbModel* data, QString* exportData) const;
    ErrCode exportListPersonInCommunity(const QString& communityUid, ExportType exportType, const QString& fpath);
    virtual quint64 getExportTypeList();
private:
    PersonCtl();


public:
    static PersonCtl* getInstance();

    const QList<QString> &importFields() const;

    /**
     * Search person by keywords, search keywords in any fields possible
     */
    QList<Person*> searchPerson(const QString& keyword);

    DbModelHandler *modelHdl();
    DbPersonModelHandler *personModelHdl();

protected:
    virtual DbModelHandler* getModelHandler();
    virtual DbModelBuilder getMainBuilder();
private:
    static PersonCtl* gInstance;
    QList<QString> mImportFields;
    DbPersonModelHandler* mModelHdl;
//    QHash<QString, std::function<QString (Person::*())>> mExportFields;
public :
    virtual ErrCode onLoad();
};

#endif // PersonCTL_H
