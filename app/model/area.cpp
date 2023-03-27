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


Area::Area():DbModel(),
      mCountryDbId(0)
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
    traceout;
}
