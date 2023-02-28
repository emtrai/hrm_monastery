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
 * Filename: commonctl.h
 * Author: Anh, Ngo Huy
 * Created date:10/29/2022
 * Brief:
 */
#ifndef COMMONCTL_H
#define COMMONCTL_H

#include "controller.h"
#include "dbmodel.h"
#include <QList>

class CommonCtl : public Controller
{

public:
    // Load Community from file
    ErrCode loadFromFile(const QString& path);

    virtual ErrCode reloadDb();
    virtual QList<DbModel*> getAllItems(bool reloadDb = false, int from = 0, int noItems = 0, int* total = nullptr);
    /**
     * @brief Get list of item all items from db, must be implemented by derived class
     *        Caller must free resource after usage
     * @return List of items. Caller must free resource after usage
     */
    virtual QList<DbModel*> getAllItemsFromDb(qint64 status = DB_RECORD_ACTIVE, int from = 0, int noItems = 0, int* total = nullptr);

    /* Derived class should implement below functions
     * virtual const char* getPrebuiltFileName();
     * virtual const char* getPrebuiltFileType();
     * virtual QList<DbModel*> getAllItemsFromDb(qint64 status = DB_RECORD_ACTIVE, int from, int noItems, int* total);
     */

    virtual quint64 getExportTypeList();
protected:
    CommonCtl();
    CommonCtl(const QString& name);

    virtual DbModelBuilder getMainBuilder() = 0;
    // build model from data
    virtual DbModel *buildModel(void *items, const QString &fmt);

    virtual void clearCacheItemList();
    const char* getPrebuiltFileType();
    /**
     * @brief Parse template file in json to get export info, including item name + header
     *        Currently, just make it simple
     * @param fpath: path to template file
     * @param ftype: File type, like json, csv, etc.
     * @return
     */
    virtual ErrCode getListExportKeywords(Exporter* exporter,
                                         QHash<QString, QString>& outMap) const;

protected:

    QList<DbModel*> mCacheItemList;

};

#endif // COMMONCTL_H
