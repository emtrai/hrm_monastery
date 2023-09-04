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
#include "utils.h"

class Person;
class Event;
class DbPersonModelHandler;
class PersonEvent;
class CommunityPerson;

#define PERSONCTL PersonCtl::getInstance()

class PersonCtl: public ModelController
{
    GET_INSTANCE_DECL(PersonCtl);
private:
    PersonCtl();
    virtual ~PersonCtl();

public:

    DbPersonModelHandler *personModelHdl();

    /**
     * @brief Get person in community
     *        Caller must free data after use
     * @param communityUid
     * @param list outlist, DbModel:Person
     * @param status status of person in community, i.e. active, etc.
     * @return err code
     */
    ErrCode getListPersonInCommunity(const QString& communityUid, QList<DbModel *>& list, qint32 status = MODEL_STATUS_MAX);
    ErrCode getListCommunityOfPerson(const QString& perUid,
                                     QList<CommunityPerson *>& list,
                                     qint32 status = MODEL_STATUS_MAX);

    /**
     * @brief get list event of person
     * @param personUid
     * @param list list of event, DbModel:PersonEvent
     * @return
     */
    ErrCode getListEvents(const QString& personUid, QList<DbModel*>& list); // TODO: should move to separate event controller?
    QString getListEventsInString(const QString& personUid,
                                        const QString &sep = "\n",
                                        bool* ok = nullptr);

    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QStringList& items, quint32 idx);
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QHash<QString, QString>& items, quint32 idx);

    /**
     * @brief Filter items, only filter active model
     * @param[in] fieldId Field id to be filter, \ref FilterField
     * @param[in] opFlags Operators, \ref FilterOperation
     * @param[in] keyword
     * @param[in] targetModelName name of target model of output list
     * @param[in] outList output list
     * @param[in] from from item idx
     * @param[in] noItems the number of items needs
     * @param[out] total Total found items
     * @return ErrNone if succeed
     */
    virtual ErrCode filter(int catetoryid,
                       qint64 opFlags,
                       const QString& keywords,
                       const char* targetModelName = nullptr,
                       const DbModel* parentModel = nullptr,
                       QList<DbModel*>* outList = nullptr,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

    /**
     * @brief get full path to template file for export data
     * @param[in] exporter exporter like csv, xlsx
     * @param[out] ftype template file format
     * @return path to template file
     */
    virtual ErrCode exportTemplatePath(FileExporter* exporter,
                                        const QString& name,
                                        QString& fpath,
                                        QString* ftype = nullptr) const;

    /**
     * @brief Export list of person in community
     * @param communityUid community Uid
     * @param exportType Export format, i.e. xlsx, etc.
     * @param fpath
     * @return
     */
    ErrCode exportListPersonInCommunity(const QString& communityUid, ExportType exportType, const QString& fpath);

    /**
     * @brief get List of supported export type
     * @return
     */
    virtual quint64 getExportTypeList();

    /**
     * @brief Main model builder
     * @return model builder
     */
    virtual DbModelBuilder getMainBuilder();
private:
    QList<QString> mImportFields;
};

#endif // PersonCTL_H
