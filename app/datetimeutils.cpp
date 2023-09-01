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
 * Filename: datetimeutils.cpp
 * Author: Anh, Ngo Huy
 * Created date:6/18/2023
 * Brief:
 */
#include "datetimeutils.h"
#include <QDateTime>
#include "logger.h"
#include "errcode.h"
#include "utils.h"

// yymd
#define YMD_TO_INT(y,m,d) (((y) << 16) | ((m) << 8) | (d))
#define YEAR_FROM_INT(date) ((date >> 16) & 0xFFFF)
#define MONTH_FROM_INT(date) ((date >> 8) & 0xFF)
#define DAY_FROM_INT(date) (date & 0xFF)

static const QChar supportedSpli[] = {'/', '-', '.'}; // TODO: make it global????


qint64 DatetimeUtils::currentTimeMs()
{
    return QDateTime::currentMSecsSinceEpoch();
}


QString DatetimeUtils::timeMsToDatestring(qint64 timeMs, const QString &format)
{
    tracein;
    logd("conver time ms %ld", timeMs);
    QString time = QDateTime::fromMSecsSinceEpoch(timeMs).toString(format);
    logd("time in string %s", time.toStdString().c_str());
    traceout;
    return time;
}

QString DatetimeUtils::currentTimeToDatestring(const QString &format)
{
    return timeMsToDatestring(currentTimeMs(), format);
}


qint64 DatetimeUtils::dateFromString(const QString &date, const QString &f, bool* isOk)
{
    QStringList listFormat;
    QStringList listDate;
    qint64 ret = 0;
    QString format = f.toUpper().simplified();
    bool ok = false;
    tracein;

    logd("Convert date '%s' to int, format '%s'",
         date.toStdString().c_str(),
         format.toStdString().c_str()
         );
    int len = sizeof(supportedSpli)/sizeof(supportedSpli[0]);

    for (int i = 0; i < len; i++){
        if (format.contains(supportedSpli[i])){
            listFormat = format.split(supportedSpli[i]);
        }
        if (date.contains(supportedSpli[i])){
            listDate = date.split(supportedSpli[i]);
        }
    }
    if (listDate.empty())
        listDate.append(date);
    logd("listFormat length %lld", listFormat.length());
    logd("listDate length %lld", listDate.length());
    if (!listDate.empty() && !listFormat.empty()
        && (listDate.length() >= 1)) {

        qint32 year = 0, month = 0, day = 0;

        int idx = 0;
        int yearIdx = -1;
        int monthIdx = -1;
        int dayIdx = -1;
        int noDateItem = listDate.length();
        QString fmt;
        logd("parse each item in date");
        foreach (QString item, listFormat){
            logd("item %s", item.toStdString().c_str());
            logd("listDate %s", (idx < noDateItem)?listDate[idx].toStdString().c_str():"0");
            if (yearIdx < 0 && item.simplified().startsWith("Y")){ // year
                yearIdx = idx;
                fmt += "Y";
            } else if (monthIdx < 0 && item.simplified().startsWith("M")){
                monthIdx = idx;
                fmt += "M";
            }  else if (dayIdx < 0 && item.simplified().startsWith("D")){
                dayIdx = idx;
                fmt += "D";
            }
            idx ++;
        }
        logd("fmt=%s, noDateItem=%d", STR2CHA(fmt), noDateItem);
        logd("yearIdx=%d, monthIdx=%d, dayIdx=%d", yearIdx, monthIdx, dayIdx);
        if (noDateItem == 1) {
            if (fmt.contains("Y")) {
                year = listDate[0].toInt(&ok);
                month = day = 0;
            } else if (fmt.contains("M")) {
                month = listDate[0].toInt(&ok);
                year = day = 0;
            } else {
                day = listDate[0].toInt(&ok);
                year = month = 0;
            }
        } else if (noDateItem > 2 || fmt == "YMD" || fmt == "YM" || fmt == "MD") {
            year = (yearIdx >= 0 && yearIdx < noDateItem)?listDate[yearIdx].toInt(&ok):0;
            month = (monthIdx >= 0 && monthIdx < noDateItem)?listDate[monthIdx].toInt(&ok):0;
            day = (dayIdx >= 0 && dayIdx < noDateItem)?listDate[dayIdx].toInt(&ok):0;
        } else {
            // DMY, MDY
            // MY, DM
            if (fmt == "DMY" || fmt == "MDY" || fmt == "MY") { // skip D
                year = listDate[1].toInt(&ok);
                month = listDate[0].toInt(&ok);
                day = 0;
            } else if (fmt == "MD") {
                day = listDate[1].toInt(&ok);
                month = listDate[0].toInt(&ok);
                year = 0;
            } else if (fmt == "DM") {
                day = listDate[0].toInt(&ok);
                month = listDate[1].toInt(&ok);
                year = 0;
            } else {
                ok = false;
            }
            //            if (yearIdx >= 0) { // has year, mean Y/M/D, D/M/Y, M/D/Y or M/Y, Y/M or Y
            //                if (noDateItem  == 1) { // year only
            //                    year = listDate[0].toInt(&ok);
            //                } else if (noDateItem  == 2) { // M/Y or Y/M
            //                    if (yearIdx == 0) {
            //                        year = listDate[0].toInt(&ok); // Y/M
            //                    } else {
            //                        year = listDate[noDateItem-1].toInt(&ok); // M/Y
            //                    }
            //                } else { // Y/M/D or D/M/Y or M/D/Y
            //                    if (yearIdx < noDateItem)
            //                        year = listDate[yearIdx].toInt(&ok);
            //                    else
            //                        year = listDate[noDateItem-1].toInt(&ok);
            //                }
            //            }asdaddasasd
            //            if (monthIdx >= 0) {
            //                if (noDateItem == 2) { // Y/M, M/Y, M/D, D/M
            //                    // 2 case: Y & M or M & D
            //                    if (yearIdx == 0) { // Y/M
            //                        month = listDate[noDateItem-1].toInt(&ok);
            //                    } else if (yearIdx > 0) {
            //                        month = listDate[0].toInt(&ok);
            //                    } else { // M/D or D/M
            //                        year = listDate[monthIdx].toInt(&ok);
            //                    }
            //                } else {

            //                }
            //            }
        }
        logd("year %d moth %d date %d", year, month, day);
        if (ok){
            ret = YMD_TO_INT(year, month, day);
        } else{
            ret = 0;
            loge("Invalid data/format: parse faile");

        }
        //        foreach (QString item, listFormat){
        //            if (idx >= listDate.length())
        //                break;
        //            ok = false;
        //            logd("item %s", item.toStdString().c_str());
        //            logd("listDate %s", listDate[idx].toStdString().c_str());
        //            if (item.simplified().startsWith("Y")){ // year
        //                year = listDate[idx].toInt(&ok);
        //                logd("year '%s' -> %d", STR2CHA(listDate[idx]), year);

        //            } else if (item.simplified().startsWith("M")){
        //                 // TODO: support format like Jan, January???
        //                month = listDate[idx].toInt(&ok);
        //                logd("month '%s' -> %d", STR2CHA(listDate[idx]), month);

        //            }  else if (item.simplified().startsWith("D")){
        //                // TODO: support format like Mon, Tuesday????
        //                day = listDate[idx].toInt(&ok);
        //                logd("day '%s' -> %d", STR2CHA(listDate[idx]), day);
        //            }
        //            if (!ok){
        //                break;
        //            }
        //            idx ++;
        //        }
        //        logd("year %d moth %d date %d", year, month, day);
        //        if (ok){
        //            ret = YMD_TO_INT(year, month, day);
        //        } else{
        //            ret = 0;
        //            loge("Invalid data/format: parse faile");

        //        }
    } else{
        loge("Invalid data/format: not match date and format");
    }
    if (isOk) *isOk = ok;
    logd("result date 0x%x, ok %d", (quint32) ret, ok);
    traceout;
    return ret;
}


QString DatetimeUtils::date2String(qint64 date, const QString& format, bool* isOk)
{
    tracein;
    int year = 0, month = 0, day = 0;
    QStringList listFormat;
    QChar split;
    bool foundSplit = false;
    bool ok = true;
    QString dateString;
    logd("date=0x%x format=%s", date, STR2CHA(format));
    if (date > 0) {
        year = YEAR_FROM_INT(date);
        month = MONTH_FROM_INT(date);
        day = DAY_FROM_INT(date);
        logd("Date 0x%x", (quint32)date);
        logd("year %d", (quint32)year);
        logd("month %d", (quint32)month);
        logd("day %d", (quint32)day);
        int len = sizeof(supportedSpli)/sizeof(supportedSpli[0]);

        for (int i = 0; i < len; i++){
            if (format.contains(supportedSpli[i])){
                listFormat = format.split(supportedSpli[i]);
                split = supportedSpli[i];
                foundSplit = true;
                break;
            }
        }
        if (foundSplit) {
            logd("split: '%s'", STR2CHA(QString(split)));
            int idx = 0;
            QString val;
            foreach (QString item, listFormat){

                // if 0, still print out 0 so that parsing later can work normally
                // TODO: should let empty instead? impact to convert string to int
                if (item.simplified().startsWith("Y")) { // year
                    if (year > 0) val = QString::number(year);
                } else if (item.simplified().startsWith("M")) {
                    if (month > 0) val = QString::number(month);
                }  else if (item.simplified().startsWith("D")) {
                    if (day > 0) val = QString::number(day);
                } else {
                    ok = false;
                    loge("invalid format '%s'", STR2CHA(item));
                    break;
                }
                idx++;
                if (!val.isEmpty()) {
                    dateString += val;
                }
                if (idx < listFormat.size() && !dateString.isEmpty() && !val.isEmpty()) {
                    dateString += split;
                }
            }
        } else {
            ok = false;
            loge("not found split for format '%s'", STR2CHA(format));
        }
    } else {
        logd("date is zero, nothing to return");
    }
    if (isOk) *isOk = ok;
    logd("DateString %s", STR2CHA(dateString));
    traceout;
    return dateString;
}


void DatetimeUtils::date2ymd(qint64 date, int *pday, int *pmonth, int *pyear)
{
    tracein;
    int year = 0, month = 0, day = 0;
    year = (date >> 16) & 0xFF;
    month = (date >> 8) & 0xFF;
    day = date & 0xFF;
    if (pyear != nullptr)
        *pyear = year;
    if (pmonth != nullptr)
        *pmonth = month;
    if (pday != nullptr)
        *pday = day;
    logd("%llu -> %d / %d / %d", date,year,month,day);
    traceout;
}

