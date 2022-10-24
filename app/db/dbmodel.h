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
#include "exportfactory.h"

class DbModel;
class DbModelHandler;
class IExporter;

typedef DbModel*(*DbModelBuilder)(void);

class DbModel
{
public:
    DbModel();
    DbModel(const DbModel& model);
    DbModel(const DbModel* model);
    virtual ~DbModel();


    virtual QString modelName() const;
    virtual int modelType() const;

    virtual const QString &name() const;
    virtual void setName(const QString &newName);

    virtual qint64 dbId() const;
    virtual void setDbId(qint64 newDbId);

    virtual const QString &uid() const;
    virtual void setUid(const QString &newUid);
    virtual void buildUidIfNotSet();
    virtual QString buildUid(const QString* seed = nullptr);


    virtual ErrCode save();
    virtual ErrCode exportTo(const QString &fpath, ExportType type);

    virtual qint32 dbStatus() const;
    virtual void setDbStatus(qint32 newDbStatus);

    virtual const QString &history() const;
    virtual void setHistory(const QString &newHistory);

    virtual bool isValid();
    virtual void dump();
    virtual QString toString();


    void setNameId(const QString &newNameId);
    virtual IExporter* getExporter();

    /**
     * @brief validate if data is all valid
     * @param result of validate for each field Field:ErrCode
     * @return ErrNone on ok, ErrInvalidData if data is invalid, other error code otherwhise
     */
    virtual ErrCode validate();

    QHash<QString, ErrCode> *validateResult() const;

    const QString &validateMsg() const;

    void appendValidateResult(const QString& item, ErrCode res);
    void appendValidateMsg(const QString &newValidateMsg);
    void cleanValidateResult();

protected:
    virtual DbModelHandler* getDbModelHandler() = 0;
    virtual ErrCode prepare2Save();
protected:
    QHash<QString, ErrCode>* mValidateResult;
    QString mValidateMsg;
private:
    qint64 mDbId;
    QString mName;// TODO: support multi languate???
    QString mUid;
    QString mHistory;
    qint32 mDbStatus;
};

#endif // DBMODEL_H
