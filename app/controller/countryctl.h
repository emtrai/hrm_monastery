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
 * Filename: countryctl.h
 * Author: Anh, Ngo Huy
 * Created date:8/13/2022
 * Brief:
 */
#ifndef COUNTRYCTL_H
#define COUNTRYCTL_H

#include "controller.h"
#include "dbmodel.h"
#include "country.h"
#include "commonctl.h"


#define COUNTRYCTL (CountryCtl::getInstance())

class CountryCtl : public CommonCtl
{
public:
    CountryCtl();
    virtual ~CountryCtl();
    virtual DbModelHandler* getModelHandler();
protected:
    DbModel *buildModel(void *items, const QString &fmt);
    virtual const char *getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();
    virtual QList<DbModel*> getItemFromDb();
public:
    QList<Country*> getCountryList();
    QStringList getRegionList();
    QStringList getContinentList();
    static CountryCtl* getInstance();

public slots:
    virtual void onLoad();
private:
    static CountryCtl* gInstance;
    bool mLoaded;
    QHash<QString, Country*> mCountryList;
};

#endif // COUNTRYCTL_H
