/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: datetimeutils.h
 * Author: Anh, Ngo Huy
 * Created date:6/18/2023
 * Brief:
 */
#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include <QString>

#define DATE_FORMAT_MD "M/D"
#define DATE_FORMAT_DM "D/M"
#define DATE_FORMAT_YMD "Y/M/D"
#define DATE_FORMAT_DMY "D/M/Y"
#define DEFAULT_FORMAT_YMD DATE_FORMAT_DMY
#define DEFAULT_FORMAT_MD DATE_FORMAT_DM

#define QT_DATE_FORMAT_DMY "dd/MM/yyyy"
#define QT_DATE_FORMAT_DMYHMS "dd_MM_yyyy_hh_mm_ss"



#define SET_DATE_FORMAT_VAL_FROM_WIDGET(widget,func, format) \
do { \
        QString val = widget->text().trimmed();\
        func(0);\
        if (!val.isEmpty()){ \
            bool isOk = false;\
            qint64 date = DatetimeUtils::dateFromString(val, format, &isOk);\
            if (isOk && date > 0){\
                func(date);\
            }\
        } \
} while (0)


#define SET_DATE_FORMAT_VAL_FROM_WIDGET_RET(widget,func, format, ret) \
do { \
        QString val = widget->text().trimmed();\
        func(0);\
        if (!val.isEmpty()){ \
            bool isOk = false;\
            qint64 date = DatetimeUtils::dateFromString(val, format, &isOk);\
            if (isOk && date > 0){\
                func(date);\
            } else {\
                loge("invalid date '%s'", STR2CHA(val)); \
                ret = ErrInvalidArg;\
            } \
        } \
} while (0)


#define SET_DATE_VAL_FROM_WIDGET(widget,func) \
        SET_DATE_FORMAT_VAL_FROM_WIDGET(widget, func, DEFAULT_FORMAT_YMD)

#define VALIDATE_DATE_STRING(date, allowEmtpy, fmt) \
((date.isEmpty() && allowEmtpy) || DatetimeUtils::isValidDateString(date.trimmed(), fmt))

class DatetimeUtils
{
public:
    static qint64 currentTimeMs();
    static QString timeMsToDatestring(qint64 timMs,
                                      const QString& format = "yyyy/MM/dd hh:mm:ss");
    static QString currentTimeToDatestring(const QString& format = QT_DATE_FORMAT_DMY);

    static qint64 dateFromString(const QString& date,
                                 const QString& format = DEFAULT_FORMAT_YMD,
                                 bool *isOk = nullptr);
    static bool isValidDateString(const QString& date,
                                  const QString& format = DEFAULT_FORMAT_YMD);
    // TODO: default for const QString is ok or not??? can set it ???
    static QString date2String(qint64 date,
                               const QString& format = DEFAULT_FORMAT_YMD,
                               bool* isOk = nullptr);

    static void date2ymd(qint64 date, int* day = nullptr,
                         int* month = nullptr, int* year = nullptr);
};

#endif // DATETIMEUTILS_H
