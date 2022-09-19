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

#define DATE_FORMAT_MD "M/D"
#define DATE_FORMAT_DM "D/M"
#define DATE_FORMAT_YMD "Y/M/D"
#define DEFAULT_CSV_FIELD_SPLIT ':'

//TODO: change default split to '|'? '|' is not common used like ','
#define DEFAULT_CSV_ITEM_SPLIT ','
#define CSV_LIST_ITEM_SPLIT '|'

#define GET_INSTALCE_DECL(className) \
    public:\
        static className* getInstance(); \
    private: \
        static className* gInstance;

#define GET_INSTANCE_IMPL(className) \
        className* className::gInstance = nullptr;\
        className* className::getInstance() { \
            if (gInstance == nullptr){ \
                gInstance = new className(); \
            } \
            return gInstance; \
        }

#define INSTANCE(className) className::getInstance()

#define NAMEID(item1, item2) (item1 + "_" + item2)

#define DIALOG_SIZE_SHOW(dlg) \
                do {\
                    int h = 0;\
                    int w = 0;\
                    Utils::screenSize(&w, &h);\
                    dlg->resize(w/2, h-100);\
                } while (0)

typedef ErrCode (*func_one_csv_item_t)(const QStringList& items, void* caller, void* param);
typedef ErrCode (*func_one_csv_field_t)(const QString& key, const QString& value, void* caller, void* param);

enum UidNameConvertType {
    SIMPLIFY_UPPER = 0,
    NO_VN_MARK_UPPER,
    HASH_NAME,
    MAX
};


class QComboBox;
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

//    static ErrCode parseCSVFile(const QString& filePath,
//                            func_one_csv_item_t cb = nullptr,
//                            void* caller = nullptr,
//                            void* paramCb = nullptr,
//                            QChar splitBy = DEFAULT_CSV_ITEM_SPLIT
//                            );
    static ErrCode parseCSVFile(const QString& filePath,
                                std::function<
                                    ErrCode(const QStringList& items, void* caller, void* param, quint32 idx)>
                                    cb = nullptr,
                                void* caller = nullptr,
                                void* paramCb = nullptr,
                                QChar splitBy = DEFAULT_CSV_ITEM_SPLIT,
                                qint32* cnt = nullptr
                                );
    static ErrCode parseDataFromCSVFile(const QString& filePath,
                                QHash<QString, QString>* items = nullptr,
                                QChar splitBy = DEFAULT_CSV_FIELD_SPLIT,
                                func_one_csv_field_t cb = nullptr,
                                void* caller = nullptr,
                                void* paramCb = nullptr
                                );
    static QString getPrebuiltFileByLang(const QString& prebuiltName, bool lang = true);
    static QString UidFromName(const QString& name, UidNameConvertType type = SIMPLIFY_UPPER, bool* isOk = nullptr);
    static QString removeVietnameseSign(const QString& vietnameseString);
    static QString readAll(const QString& fpath);

    static void showErrorBox(const QString& msg);
    static void showErrorBox(int ret, const QString* msg = nullptr);
    static ErrCode screenSize(int* w=nullptr, int* h=nullptr);
    static int screenHeight();
    static int getCurrentComboxIndex(const QComboBox *cb);;
    static QString getCurrentComboxDataString(const QComboBox *cb, bool *isOk = nullptr);
    static ErrCode getCurrentComboxDataString(const QComboBox *cb, QString* data, QString* name = nullptr);
};

#endif // UTILS_H
