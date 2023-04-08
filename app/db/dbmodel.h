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
#include "idataimporter.h"
#include "dataexporter.h"

#define CLEAR_THEN_SET(curr, newModel, type) \
do { \
    if (curr) { \
        logd("delete olde model'%s'", STR2CHA(curr->toString())); \
        delete curr; \
        curr = nullptr; \
    } \
    if (newModel) { \
        logd("clone new model '%s'", STR2CHA(newModel->toString())); \
        curr = (type*)((DbModel*)newModel)->clone(); \
    } else { \
        logd("null model is set"); \
    } \
} while(0)

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

#define CHECK_SET_IMPORT_NAME_ID(err, value, ctl, funcSetUid, funcSetName) \
            do { \
                if (!value.isEmpty()) { \
                    DbModel* model = (ctl)->getModelByNameId(value); \
                    if (model) { \
                        funcSetUid(model->uid()); \
                        funcSetName(model->name()); \
                        delete model; \
                    } else { \
                        loge("Not found name id '%s'", STR2CHA(value)); \
                        err = ErrNotFound; \
                    } \
                } \
            } while (0)

/**
 *  uid: uid variable to check/query value
 *  valueToBeSet: variable to check and reset value
 *  funcget: DbModel function to query data from, i.e name, nameid
 */
#define CHECK_UID_TO_UPDATE_VALUE(uid, valueToBeSet, hdlName, funcget) \
            if (valueToBeSet.isEmpty() && !uid.isEmpty()) { \
                logd("no value, but has uid, update it"); \
                DbModelHandler* hdl = DB->getModelHandler(hdlName); \
                DbModel* model = hdl->getByUid(uid); \
                if (model) { \
                    valueToBeSet = model->funcget(); \
                    delete model; \
                } else { \
                    logw("uid '%s' not found", STR2CHA(uid)); \
                } \
            }

class DbModel;
class DbModelHandler;
class DataExporter;
class ModelController;

typedef DbModel*(*DbModelBuilder)(void);

/**
 * @brief parameter: value to be imported
 */
typedef std::function<ErrCode(const QString&)> ImportCallbackFunc;

/**
 * @brief parameter:field name
 */
typedef std::function<QString(const QString&)> ExportCallbackFunc;


/**
 * @brief status of record in db
 */
enum DB_RECORD_STATUS {
    DB_RECORD_NOT_READY = 0, // record not ready
    DB_RECORD_ACTIVE    = (1 << 0), // record ready to use
    DB_RECORD_DElETED   = (1 << 1), // record already deleted (soft delete)
    // Add new must update DB_RECORD_ALL
    DB_RECORD_ALL = DB_RECORD_ACTIVE | DB_RECORD_DElETED
};

// BE WARE, ANY CHANGE TO THIS STATUS WILL IMPACT TO DB
// THIS VALUE IS WRITTEN DIRECTLY TO DB
// TODO: to do what to improve this???
/**
 * @brief Status of a dbmodel
 *        It's different from record status. Record status is checked first,
 *        then check status of db model
 */
enum DbModelStatus{
    MODEL_INACTIVE      = (1 << 0), // Map is inactive/closed/stoped
    MODEL_ACTIVE        = (1 << 1), // Map is active
    MODEL_BUILDING      = (1 << 2), // building phase, not ready
    MODEL_NOT_READY     = (1 << 3),
    MODEL_STATUS_MAX    = MODEL_INACTIVE | MODEL_ACTIVE | MODEL_BUILDING | MODEL_NOT_READY
};

typedef std::shared_ptr<DbModel> DbModel_sp;

class OnDBModelListener {
public:
    virtual void onUpdate(const DbModel* model) = 0;
};

class DbModel: public IDataImporter, public DataExporter
{
public:
    static const QHash<int, QString>* getModelStatusIdNameMap();
    static QString modelStatus2Name(DbModelStatus status);
    static const QList<int>* getModelStatusList();
    static const QList<int>* getDbStatusList();
    static DbModelBuilder getBuilderByModelName(const QString& modelName);
protected:
    DbModel();
    DbModel(const DbModel& model);
    DbModel(const DbModel* model);
    void copy(const DbModel& model);
public:
    virtual ~DbModel();
    virtual void init();
    virtual DbModelBuilder getBuilder() const = 0;
    // TODO: override operation ==?

    virtual void clone(const DbModel* model);
    virtual DbModel* clone() const;

    virtual QString modelName() const;
    virtual int modelType() const;


    virtual void initExportFields();
    virtual void initImportFields();
    /**
     * @brief Import item (key:value)
     * @param importFileType
     * @param keyword
     * @param value
     * @param idx
     * @param tag
     * @return
     */
    virtual ErrCode onImportParseDataItem(const QString& importName,
                                 int importFileType,
                                 const QString& keyword,
                                 const QString& value,
                                 quint32 idx = 0,
                                 QList<DbModel *>* outList = nullptr);

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

    virtual DbModel* addFieldToBeUpdated(const QString& field);
    /**
     * @brief Update modified info
     * @return Error code
     */
    virtual ErrCode update(bool allFields = false);

    /**
     * @brief remove data from db
     * @param force: if set true, all dependency will be deleted, else not remove if there is dependency
     * @return
     */
    virtual ErrCode remove(bool force = false, QString* msg = nullptr);
    virtual ErrCode markRemove();

    virtual ErrCode exportTo(const QString &fpath, ExportType type);

    virtual qint32 dbStatus() const;
    virtual void setDbStatus(qint32 newDbStatus);

    virtual bool isValid();
    virtual void dump();
    virtual QString toString() const;


    void setNameId(const QString &newNameId);
    virtual DataExporter* getExporter();

    /**
     * @brief validate if data is all valid
     * @param result of validate for each field Field:ErrCode
     * @return ErrNone on ok, ErrInvalidData if data is invalid, other error code otherwhise
     */
    virtual ErrCode validateAllFields();
    virtual bool isExist();

    QHash<QString, ErrCode> *validateResult() const;

    const QString &validateMsg() const;

    void appendValidateResult(const QString& item, ErrCode res);
    void appendValidateMsg(const QString &newValidateMsg);
    void cleanValidateResult();

    qint64 dbCreatedTime() const;
    void setDbCreatedTime(qint64 newCreatedTime);

    qint64 lastDbUpdatedTime() const;
    void setLastDbUpdatedTime(qint64 newLastUpdatedTime);

    const QList<QString> &updatedField() const;

    /**
     * @brief Check if field is modified and should be updated
     * @return
     */
    bool isFieldUpdated(const QString& itemField);

    /**
     * @brief Clear list of all changes marked
     * Set that all changes are clear marked, start new mark for change
     */
    virtual void resetAllModifiedMark();
    bool markModified() const;
    void setMarkModified(bool newMarkModified);


    virtual ErrCode exportToFile(ExportType type, QString* fpath);
    virtual const QString exportTemplatePath(FileExporter* exporter, QString* ftype = nullptr) const;

    virtual const QStringList getListExportKeyWord() const;
    virtual ErrCode getExportDataString(const QString& item, QString* data) const;
    const QString &nameId() const;

    const QString &remark() const;
    void setRemark(const QString &newRemark);

    const QString &dbHistory() const;
    void setDbHistory(const QString &newDbHistory);

    bool updateAllFields() const;

protected:
    virtual DbModelHandler* getDbModelHandler() const = 0;
    virtual ErrCode prepare2Save();
    virtual void markItemAsModified(const QString& itemName);
    virtual void checkModifiedThenSet(QString& cur, const QString& next, const QString& itemName);
    virtual void checkModifiedThenSet(qint32& cur, qint32 next, const QString& itemName);
    virtual void checkModifiedThenSet(qint64& cur, qint64 next, const QString& itemName);

    /**
     * @brief return if model is deleted or not
     * @param msg: informative message
     * @return true: allow to delete, false otherwise
     */
    virtual bool allowRemove(QString* msg = nullptr);
protected:
    bool mDeletable; // model can be deleted from db or not
    QHash<QString, ExportCallbackFunc> mExportCallbacks;
    QHash<QString, ImportCallbackFunc> mImportCallbacks;
    QHash<QString, ErrCode>* mValidateResult;
    QString mValidateMsg;
    QList<QString> mUpdatedField; // List of fields/info were changed its value
    bool mMarkModified; // true: check & mark item as modified when it's changed. false: not mark anything
    bool mUpdateAllFields;

    qint64 mDbId;
    QString mNameId; // Code/Name id, human readable, for easy searching, unquide.
                     // Name id is unique, but changable, Should not use for mapping, mapping use uid, can be null
                     // TODO: this is newly add from Feb 12, let search and check whole
    QString mName;// TODO: support multi languate???
    QString mUid;
    QString mRemark;
    QString mDbHistory; // History on DB
    qint32 mDbStatus;
    // TODO: time calculated from 1970 will be reset on 2038!!
    qint64 mDbCreatedTime; // time in ms, since epoc time (1970)
    qint64 mLastDbUpdatedTime; // time in ms, since epoc time (1970)
};

#endif // DBMODEL_H
