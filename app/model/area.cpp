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

Area::Area():DbModel(),
      mCountryDbId(0),
      mStartDate(0),
      mEndDate(0),
      mModelStatus(0)
{
    tracein;
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
    mCountryDbId = newCountryDbId;
    markItemAsModified(KItemCountry);

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
    mCountryUid = newCountryUid;
    markItemAsModified(KItemCountry);
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
{    tracein;
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
    // TODO: implement more
    traceout;

}

const QString &Area::addr() const
{
    return mAddr;
}

void Area::setAddr(const QString &newAddr)
{
    mAddr = newAddr;
    markItemAsModified(KItemAddress);
}

const QString &Area::email() const
{
    return mEmail;
}

void Area::setEmail(const QString &newEmail)
{
    mEmail = newEmail;
    markItemAsModified(KItemEmail);
}

const QString &Area::tel() const
{
    return mTel;
}

void Area::setTel(const QString &newTel)
{
    mTel = newTel;
    markItemAsModified(KItemTel);
}

const QString &Area::changeHistory() const
{
    return mChangeHistory;
}

void Area::setChangeHistory(const QString &newChangeHistory)
{
    mChangeHistory = newChangeHistory;
    markItemAsModified(KItemChangeHistory);
}

qint32 Area::modelStatus() const
{
    return mModelStatus;
}

void Area::setModelStatus(qint32 newModelStatus)
{
    mModelStatus = newModelStatus;
    markItemAsModified(KItemStatus);
}

qint64 Area::endDate() const
{
    return mEndDate;
}

void Area::setEndDate(qint64 newEndDate)
{
    mEndDate = newEndDate;
    markItemAsModified(KItemEndDate);
}

qint64 Area::startDate() const
{
    return mStartDate;
}

void Area::setStartDate(qint64 newStartDate)
{
    mStartDate = newStartDate;
     markItemAsModified(KItemStartDate);
}
