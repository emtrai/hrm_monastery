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
 * Filename: area.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/18/2022
 * Brief:
 */
#include "area.h"

#include "utils.h"
#include "datetimeutils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"
#include "prebuiltdefs.h"
#include "filectl.h"
#include "exportdefs.h"
#include "areactl.h"
#include "areaperson.h"
#include "stringdefs.h"

Area::Area():DbModel(),
      mCountryDbId(0),
      mStartDate(0),
      mEndDate(0),
      mModelStatus(0)
{
    traced;
}

Area::~Area()
{
    tracein;
}

DbModel *Area::build()
{
    Area* item = new Area();
    item->init();
    return item;
}

void Area::clone(const DbModel *model)
{
    tracein;
    if (model) {
        DbModel::clone(model);
        copy(*(Area*)model);
    } else {
        loge("clone failed, null model");
    }
    traceout;
}

QString Area::modelName() const
{
    return KModelNameArea;
}

qint64 Area::countryDbId() const
{
    return mCountryDbId;
}

void Area::setCountryDbId(qint64 newCountryDbId)
{
    CHECK_MODIFIED_THEN_SET(mCountryDbId, newCountryDbId, KItemCountry);

}

QString Area::countryName() const
{
    return mCountryName;
}

void Area::setCountryName(QString newCountryName)
{
    mCountryName = newCountryName;
}

const QString &Area::countryUid() const
{
    return mCountryUid;
}

void Area::setCountryUid(const QString &newCountryUid)
{
    CHECK_MODIFIED_THEN_SET(mCountryUid, newCountryUid, KItemCountry);
}

QString Area::countryNameId() const
{
    return mCountryNameId;
}

void Area::setCountryNameId(const QString &newCountryNameId)
{
    mCountryNameId = newCountryNameId;
}

DbModelHandler *Area::getDbModelHandler() const
{

    return DB->getModelHandler(KModelHdlArea);
}

DbModelBuilder Area::getBuilder() const
{
    return &Area::build;
}

void Area::copy(const Area &model)
{
    tracein;
    mCountryName = model.mCountryName;
    mCountryDbId = model.mCountryDbId;
    mCountryUid = model.mCountryUid;
    mCountryNameId = model.mCountryNameId;
    mAddr = model.mAddr;
    mTel = model.mTel;
    mEmail = model.mEmail;
    mStartDate = model.mStartDate;
    mEndDate = model.mEndDate;
    mModelStatus = model.mModelStatus;
    mModelStatusName = model.mModelStatusName;
    mChangeHistory = model.mChangeHistory;
    traceout;
}

QString Area::modelStatusName() const
{
    return mModelStatusName;
}

void Area::setModelStatusName(QString newModelStatusName)
{
    mModelStatusName = newModelStatusName;
}

ErrCode Area::exportTemplatePath(FileExporter *exporter,
                                 const QString& name,
                                       QString& fpath, QString *ftype) const
{
    tracein;
    ErrCode err = ErrNone;
    if (exporter) {
        ExportType type = exporter->getExportType();
        switch (type) {
        case EXPORT_HTML:
            fpath = FileCtl::getPrebuiltDataFilePath(KPrebuiltAreaInfoTemplateFileName);
            break;
        default:
            loge("export type %d not support", type);
            err = ErrNotSupport;
        };
    } else {
        err = ErrInvalidArg;
        loge("invalid exporter");
    }
    traceret(err);
    return err;
}

void Area::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemCountry, [](const DbModel* model, const QString& item){
        return ((Area*)model)->countryName();
    });
    mExportCallbacks.insert(KItemCountryNameId, [](const DbModel* model, const QString& item){
        return ((Area*)model)->countryNameId();
    });
    mExportCallbacks.insert(KItemAddress, [](const DbModel* model, const QString& item){
        return ((Area*)model)->addr();
    });
    mExportCallbacks.insert(KItemEmail, [](const DbModel* model, const QString& item){
        return ((Area*)model)->email();
    });
    mExportCallbacks.insert(KItemTel, [](const DbModel* model, const QString& item){
        return ((Area*)model)->tel();
    });
    mExportCallbacks.insert(KItemStartDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Area*)model)->startDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemEndDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Area*)model)->endDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemStatus, [](const DbModel* model, const QString& item){
        return ((Area*)model)->modelStatusName();
    });
    mExportCallbacks.insert(KItemChangeHistory, [](const DbModel* model, const QString& item){
        return ((Area*)model)->changeHistory();
    });
    mExportCallbacks.insert(KItemContactPeopleList, [](const DbModel* model, const QString& item){
        QString contactListStr;
        QList<DbModel *> contactList;
        ErrCode err = ErrNone;
        if (!model) {
            err = ErrInvalidModel;
        }
        dbg(LOG_VERBOSE, "get contact list for model '%s'", MODELSTR2CHA(model));
        if (err == ErrNone) {
            err = AREACTL->getContactPeopleList(model->uid(), contactList, MODEL_STATUS_ACTIVE);
            logife(err, "failed to get managers list for ''%s'", MODELSTR2CHA(model));
        }

        if (err == ErrNone) {
            foreach (DbModel* item, contactList) {
                if (IS_MODEL_NAME(item, KModelNameAreaPerson)) {
                    AreaPerson* per = static_cast<AreaPerson*>(item);
                    contactListStr += QString("%1: %2\n").arg(per->personName(), per->roleName());
                    contactListStr += QString("- Điện thoại: %1\n").arg(per->personTel());
                    contactListStr += QString("- Email: %1\n").arg(per->personEmail());
                } else {
                    loge("invalid model '%s', expected %s", MODELSTR2CHA(item), KModelNameAreaPerson);
                }
            }
        }
        if (err != ErrNone) {
            contactListStr = QString(STR_DATA_ERROR_CODE).arg(err);
        }
        if (contactListStr.isEmpty()) {
            contactListStr = STR_NO_DATA;
        }
        return contactListStr;
    });
    // TODO: implement more
    traceout;

}

const QString &Area::addr() const
{
    return mAddr;
}

void Area::setAddr(const QString &newAddr)
{
    CHECK_MODIFIED_THEN_SET(mAddr, newAddr, KItemAddress);
}

const QString &Area::email() const
{
    return mEmail;
}

void Area::setEmail(const QString &newEmail)
{
    CHECK_MODIFIED_THEN_SET(mEmail, newEmail, KItemEmail);
}

const QString &Area::tel() const
{
    return mTel;
}

void Area::setTel(const QString &newTel)
{
    CHECK_MODIFIED_THEN_SET(mTel, newTel, KItemTel);
}

const QString &Area::changeHistory() const
{
    return mChangeHistory;
}

void Area::setChangeHistory(const QString &newChangeHistory)
{
    CHECK_MODIFIED_THEN_SET(mChangeHistory, newChangeHistory, KItemChangeHistory);
}

qint32 Area::modelStatus() const
{
    return mModelStatus;
}

void Area::setModelStatus(qint32 newModelStatus)
{
    CHECK_MODIFIED_THEN_SET(mModelStatus, newModelStatus, KItemStatus);
}

qint64 Area::endDate() const
{
    return mEndDate;
}

void Area::setEndDate(qint64 newEndDate)
{
    CHECK_MODIFIED_THEN_SET(mEndDate, newEndDate, KItemEndDate);
}

qint64 Area::startDate() const
{
    return mStartDate;
}

void Area::setStartDate(qint64 newStartDate)
{
    CHECK_MODIFIED_THEN_SET(mStartDate, newStartDate, KItemStartDate);
}
