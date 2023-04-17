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

const QString Area::exportTemplatePath(FileExporter *exporter, QString *ftype) const
{
    if (exporter) {
        switch (exporter->getExportType()) {
        case EXPORT_HTML:
            return FileCtl::getPrebuiltDataFilePath(KPrebuiltAreaInfoTemplateFileName);
        };
    }
    return QString();
}

void Area::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemCountry, [this](const QString& item){
        return this->countryName();
    });
    mExportCallbacks.insert(KItemAddress, [this](const QString& item){
        return this->addr();
    });
    mExportCallbacks.insert(KItemEmail, [this](const QString& item){
        return this->email();
    });
    mExportCallbacks.insert(KItemTel, [this](const QString& item){
        return this->tel();
    });
    mExportCallbacks.insert(KItemStartDate, [this](const QString& item){
        return Utils::date2String(this->startDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemEndDate, [this](const QString& item){
        return Utils::date2String(this->endDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemStatus, [this](const QString& item){
        return this->modelStatusName();
    });
    mExportCallbacks.insert(KItemChangeHistory, [this](const QString& item){
        return this->changeHistory();
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
