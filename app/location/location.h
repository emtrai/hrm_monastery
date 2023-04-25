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
 * Filename: location.h
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#ifndef LOCATION_H
#define LOCATION_H


#include "controller.h"
#include "country.h"
#include <QHash>
#include "errcode.h"
#include <QList>

// TODO: remove location?

class Location: public Controller
{
public:
    static Location* getInstance();

    void addCountry(QString id, const Country& country);
private:
    Location();

    ErrCode loadCountryFromFiles();
    QList<Country*> getListCountry();

    ErrCode doCsvParseOneItem(const QStringList& items, void* param);
private:
    static ErrCode onCsvParseOneItemCallback(const QStringList& items, void* caller, void* param, quint32 idx);
private:
    static Location* gInstance;
    QHash<QString, Country*> mListCountry;

public:
    virtual QString getName();
    virtual ErrCode onLoad();
    virtual void onUnload();
};

#endif // LOCATION_H
