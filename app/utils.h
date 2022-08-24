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
 * Filename: utils.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include "defs.h"
#include "logger.h"
#include "errcode.h"
#include <QHash>
#include <QChar>

#define GET_INSTALCE_DECL(className) \
    public:\
        static className* getInstance(); \
    private: \
        static className* gInstance;

#define GET_INSTANCE_IMPL(gInstance, className) \
        className* className::gInstance = nullptr;\
        className* className::getInstance() { \
            if (gInstance == nullptr){ \
                gInstance = new className(); \
            } \
            return gInstance; \
        }

#define INSTANCE(className) className::getInstance()

typedef ErrCode (*func_one_csv_item_t)(const QStringList& items, void* caller, void* param);


class Utils
{
public:
    static qint64 getCurrentTimeMs();
    static Gender genderFromString(const QString& gender);
    /**
    * Y/M/D
    * M/D
    */
    // TODO: default for const QString is ok or not??? can set it ???
    static qint64 dateFromString(const QString& date, const QString& format = "YYYY/MM/DD", bool *isOk = nullptr);
    static QString date2String(qint64 date, const QString& format = "YYYY/MM/DD");
    static void date2ymd(qint64 date, int* day = nullptr,
                            int* month = nullptr, int* year = nullptr);

    static ErrCode parseCSVFile(const QString& filePath,
                            func_one_csv_item_t cb = nullptr,
                            void* caller = nullptr,
                            void* paramCb = nullptr,
                            QChar splitBy = ','
                            );

    static ErrCode parseDataFromCSVFile(const QString& filePath,
                                QHash<QString, QString>* items,
                                QChar splitBy = ':'
                                );
    static QString getPrebuiltFileByLang(const QString& prebuiltName, bool lang = true);
    static QString UidFromName(const QString& name);
    static QString readAll(const QString& fpath);

};

#endif // UTILS_H
