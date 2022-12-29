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
#include <QList>
#include "utils.h"

// if (markModified()) logd("value is different '%s'", QString("'%1' vs '%2'").arg(cur, next).toStdString().c_str());
#define CHECK_MODIFIED_THEN_SET(cur, next, itemName) \
    do { \
        if (cur != next) { \
            cur = next; \
            markItemAsModified(itemName); \
        } \
    } while (0)

#define CHECK_MODIFIED_THEN_SET_QLIST_STRING(cur, next, itemName) \
    do { \
        if (!Utils::isSameList<QString>(cur, next)) { \
            cur.clear(); \
            cur.append(next); \
            markItemAsModified(itemName); \
        } \
    } while (0)

class DbModel;
class DbModelHandler;
class IExporter;

typedef DbModel*(*DbModelBuilder)(void);
enum DB_RECORD_STATUS {
    DB_RECORD_NOT_READY = 0, // record not ready
    DB_RECORD_ACTIVE, // record ready to use
    DB_RECORD_DElETED, // record already deleted
    DB_RECORD_MAX
};

class DbModel
{
public:
    DbModel();
    DbModel(const DbModel& model);
    DbModel(const DbModel* model);
    virtual ~DbModel();
    // TODO: override operation ==?

    virtual void clone(const DbModel* per);

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


    /**
     * @brief Save all info. or create new
     * All information will be stored/replaced
     * @return Error code
     */
    virtual ErrCode save();
    /**
     * @brief Update modified info
     * @return Error code
     */
    virtual ErrCode update();
    /**
     * @brief remove data from db
     * @return
     */
    virtual ErrCode remove();
    virtual ErrCode markRemove();

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
    virtual bool isExist();

    QHash<QString, ErrCode> *validateResult() const;

    const QString &validateMsg() const;

    void appendValidateResult(const QString& item, ErrCode res);
    void appendValidateMsg(const QString &newValidateMsg);
    void cleanValidateResult();

    qint64 createdTime() const;
    void setCreatedTime(qint64 newCreatedTime);

    qint64 lastUpdatedTime() const;
    void setLastUpdatedTime(qint64 newLastUpdatedTime);

    const QList<QString> &updatedField() const;

    /**
     * @brief Clear list of all changes marked
     * Set that all changes are clear marked, start new mark for change
     */
    virtual void resetAllModifiedMark();
    bool markModified() const;
    void setMarkModified(bool newMarkModified);

protected:
    virtual DbModelHandler* getDbModelHandler() = 0;
    virtual ErrCode prepare2Save();
    virtual void markItemAsModified(const QString& itemName);
    virtual void checkModifiedThenSet(QString& cur, const QString& next, const QString& itemName);
    virtual void checkModifiedThenSet(qint32& cur, qint32 next, const QString& itemName);
    virtual void checkModifiedThenSet(qint64& cur, qint64 next, const QString& itemName);
protected:
    QHash<QString, ErrCode>* mValidateResult;
    QString mValidateMsg;
    QList<QString> mUpdatedField; // List of fields/info were changed its value
    bool mMarkModified; // true: check & mark item as modified when it's changed. false: not mark anything

    qint64 mDbId;
    QString mName;// TODO: support multi languate???
    QString mUid;
    QString mHistory; // History on DB
    qint32 mDbStatus;
    // TODO: time calculated from 1970 will be reset on 2038!!
    qint64 mCreatedTime; // time in ms, since epoc time (1970)
    qint64 mLastUpdatedTime; // time in ms, since epoc time (1970)
};

#endif // DBMODEL_H
