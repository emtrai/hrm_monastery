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
 * Filename: dbmodel.h
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#ifndef DBMODEL_H
#define DBMODEL_H

#include <QString>
#include "errcode.h"
class DbModel;
class DbModelHandler;
typedef DbModel*(*DbModelBuilder)(void);

class DbModel
{
public:
    DbModel();
    virtual ~DbModel();

    virtual QString nameid() const;

    virtual const QString &name() const;
    virtual void setName(const QString &newName);

    virtual qint64 dbId() const;
    virtual void setDbId(qint64 newDbId);

    virtual const QString &uuid() const;
    virtual void setUuid(const QString &newUuid);

    virtual ErrCode save();

    virtual qint32 dbStatus() const;
    virtual void setDbStatus(qint32 newDbStatus);

    virtual const QString &history() const;
    virtual void setHistory(const QString &newHistory);

    virtual bool isValid();
    virtual void dump();
    virtual QString toString();


protected:
    virtual DbModelHandler* getDbModelHandler() = 0;

private:
    qint64 mDbId;
    QString mName;// TODO: support multi languate???
    QString mUuid;
    QString mHistory;
    qint32 mDbStatus;
};

#endif // DBMODEL_H
