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
#include "modeldefs.h"
#include <QAtomicInt>
#include "importtype.h"

#define CLONE_MODEL(model, type) (model?((type*)((DbModel*)model)->clone()):nullptr)
#define CLONE_DBMODEL(model) (model?(model->clone()):nullptr)
#define CLONE_LIST(list, type) DbModel::cloneListModel<type>(list)
#define CLONE_LIST_DBMODEL(list) CLONE_LIST(list, DbModel)
#define CLONE_LIST_FROM_DBMODEL(list, type) DbModel::cloneListFromDbModelList<type>(list)

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

#define IS_MODEL_NAME(model, name) (model && name && (model->modelName() == name))

#define EXPORT_CALLBACK_STRING_IMPL(type, modelName, func) \
[](const DbModel* model, const QString& item){ \
    QString ret; \
    logd("get export item '%s'", STR2CHA(item));\
    UNUSED(item); \
    if (IS_MODEL_NAME(model, modelName)) { \
        ret = ((type*)model)->func(); \
    } else { \
        ret = STR_DATA_ERROR; \
    } \
    return ret; \
}

#define EXPORT_CALLBACK_INT_IMPL(type, modelName, func) \
[](const DbModel* model, const QString& item){ \
        QString ret; \
        logd("get export item '%s'", STR2CHA(item));\
        UNUSED(item); \
        if (IS_MODEL_NAME(model, modelName)) { \
            ret = QString::number(((type*)model)->func()); \
    } else { \
            ret = STR_DATA_ERROR; \
    } \
        return ret; \
}

#define EXPORT_CALLBACK_DATETIME_IMPL(type, modelName, func, format) \
[](const DbModel* model, const QString& item){ \
    QString ret; \
    logd("get export item '%s'", STR2CHA(item));\
    UNUSED(item); \
    if (IS_MODEL_NAME(model, modelName)) { \
        ret = DatetimeUtils::date2String(((type*)model)->func(), format); \
    } else { \
        ret = STR_DATA_ERROR; \
    } \
    return ret; \
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
typedef std::function<QString(const DbModel* model, const QString&)> ExportCallbackFunc;


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
    MODEL_STATUS_UNKNOWN               = 0, // unknown status
    MODEL_STATUS_INACTIVE      = (1 << 0), // Map is inactive/closed/stoped
    MODEL_STATUS_ACTIVE        = (1 << 1), // Map is active
    MODEL_STATUS_BUILDING      = (1 << 2), // building phase, not ready
    MODEL_STATUS_NOT_READY     = (1 << 3),
    MODEL_STATUS_MAX           =    MODEL_STATUS_INACTIVE |
                                    MODEL_STATUS_ACTIVE |
                                    MODEL_STATUS_BUILDING |
                                    MODEL_STATUS_NOT_READY
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
    static QString modelStatus2Name(DbModelStatus status, bool* ok = nullptr);
    static const QList<int>* getModelStatusList();
    static const QList<int>* getDbStatusList();
    template<class T>
    static QList<T*> cloneListModel(const QList<T*>& list)
    {
        QList<T*> outList;
        foreach (T* model, list) {
            if  (model) {
                T* item = ((T*)((DbModel*)model)->clone());
                if (item) {
                    outList.append(item);
                }
            }
        }
        return outList;
    }
    template<class T>
    static QList<T*> cloneListFromDbModelList(const QList<DbModel*>& list)
    {
        QList<T*> outList;
        foreach (DbModel* model, list) {
            if  (model) {
                T* item = (T*)(model->clone());
                if (item) {
                    outList.append(item);
                }
            }
        }
        return outList;
    }
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

    /**
     * @brief Copy data only, except identity such as uid, nameid, dbid
     * @param model
     */
    virtual ErrCode copyData(const DbModel* model);
    virtual void incRef(); // TODO: change to use reference counter to avoid overhead when clone
    virtual void decRef();
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
    virtual QString fullName() const;
    virtual void setName(const QString &newName);

    virtual qint64 dbId() const;
    virtual void setDbId(qint64 newDbId);

    virtual const QString &uid() const;
    virtual void setUid(const QString &newUid);
    virtual void buildUidIfNotSet();
    virtual QString buildUid(const QString* seed = nullptr);
    virtual QString buildNameId(const QString* seed = nullptr, bool* ok = nullptr);

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

    virtual qint32 dbStatus() const;
    virtual void setDbStatus(qint32 newDbStatus);

    virtual bool isValid();
    virtual void dump() const;
    virtual QString toString() const;

    virtual QString getName() const;

    void setNameId(const QString &newNameId);
    virtual const DataExporter* getExporter() const;

    /**
     * @brief validate if data is all valid
     * @param result of validate for each field Field:ErrCode
     * @return ErrNone on ok, ErrInvalidData if data is invalid, other error code otherwhise
     */
    virtual ErrCode validateAllFields(bool checkExist = false);
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

    virtual ErrCode exportTo(const QString &fpath, ExportType type);
    virtual ErrCode exportToFile(ExportType type, QString* fpath) const;
    virtual QString exportHtmlTemplateFile(const QString& name) const;
    virtual ErrCode exportTemplatePath(FileExporter* exporter,
                                       const QString& name,
                                       QString& fpath,
                                       QString* ftype = nullptr) const;

    virtual const QStringList getListExportKeyWord() const;
    virtual ErrCode getExportDataString(const QString& item, QString* data) const;
    const QString &nameId() const;

    const QString &remark() const;
    void setRemark(const QString &newRemark);

    const QString &dbHistory() const;
    void setDbHistory(const QString &newDbHistory);

    bool updateAllFields() const;

    quint32 refCnt() const;

    /**
     * @brief return if model is deleted or not
     * @param msg: informative message
     * @return true: allow to delete, false otherwise
     */
    virtual bool allowRemove(QString* msg = nullptr);
protected:
    virtual DbModelHandler* getDbModelHandler() const = 0;
    virtual ErrCode prepare2Save();
    virtual void markItemAsModified(const QString& itemName);
    virtual void checkModifiedThenSet(QString& cur, const QString& next, const QString& itemName);
    virtual void checkModifiedThenSet(qint32& cur, qint32 next, const QString& itemName);
    virtual void checkModifiedThenSet(qint64& cur, qint64 next, const QString& itemName);

protected:
    bool mDeletable; // model can be deleted from db or not
    QHash<QString, ExportCallbackFunc> mExportCallbacks;
    QHash<QString, ImportCallbackFunc> mImportCallbacks;
    QHash<QString, quint64> mImportItemsType;
    QList<QString> mImportFieldRequired;
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
    QAtomicInt mRefCnt;
};

#endif // DBMODEL_H
