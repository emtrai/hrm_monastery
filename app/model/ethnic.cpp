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
 * Filename: ethnic.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#include "ethnic.h"
#include "logger.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"
#include "prebuiltdefs.h"
#include "stringdefs.h"

Ethnic::Ethnic():DbModel()
{
    traced;
}

DbModel *Ethnic::build()
{
    tracein;
    Ethnic* model = new Ethnic();
    model->init();
    return model;
}

DbModelBuilder Ethnic::getBuilder() const
{
    return &Ethnic::build;
}

QString Ethnic::modelName() const
{
    return KModelNameEthnic;
}

void Ethnic::clone(const DbModel *model)
{
    tracein;
    DbModel::clone(model);
    if (model && model->modelName() == KModelNameEthnic) {
        Ethnic* ethnic = (Ethnic*)model;
        mCountryUid = ethnic->mCountryUid;
        mCountryDbId = ethnic->mCountryDbId;
        mCountryName = ethnic->mCountryName;
    } else {
        loge("null or invald model name '%s'", model?STR2CHA(model->modelName()):"null");
    }
    traceout;
}

void Ethnic::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemCountry, EXPORT_CALLBACK_STRING_IMPL(
                                                   Ethnic,
                                                   KModelNameEthnic,
                                                   countryName));
    traceout;
}

QString Ethnic::exportHtmlTemplateFile(const QString &name) const
{
    UNUSED(name);
    return KPrebuiltEthnicInfoTemplateFileName;

}

qint64 Ethnic::countryDbId() const
{
    return mCountryDbId;
}

void Ethnic::setCountryDbId(qint64 newCountryDbId)
{
    mCountryDbId = newCountryDbId;
}

DbModelHandler *Ethnic::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlEthnic);
}

const QString &Ethnic::countryName() const
{
    return mCountryName;
}

void Ethnic::setCountryName(const QString &newCountryName)
{
    mCountryName = newCountryName;
}

QString Ethnic::toString() const
{
    QString str = DbModel::toString();
    str += QString(":countryUid('%1')").arg(countryUid());
    str += QString(":countryName('%1')").arg(countryName());
    return str;
}

const QString &Ethnic::countryUid() const
{
    return mCountryUid;
}

void Ethnic::setCountryUid(const QString &newCountryUid)
{
    CHECK_MODIFIED_THEN_SET(mCountryUid, newCountryUid, KItemCountry);
}
