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
 * Filename: utils.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "utils.h"
#include <QHash>
#include <QString>
#include "std.h"
#include <QFile>
// yymd
#define YMD_TO_INT(y,m,d) (((y) << 16) | ((m) << 8) | (d))

qint64 Utils::getCurrentTimeMs()
{

}

Gender Utils::genderFromString(const QString &gender)
{
    static QHash<QString, Gender> map;
    static bool mapInit = false;
    traced;

    if (!mapInit)
    {
        logd("Init map gender with string");
        // TODO: should do init somewhere????
        map["MALE"] = Gender::MALE;
        map["FEMALE"] = Gender::FEMALE;
        mapInit = true;
    }
    QString key = gender.simplified().toUpper();
    logd("Gender key %s", key.toStdString().c_str());
    Gender ret = Gender::GENDER_UNKNOWN;
    if (!key.isEmpty() && map.contains(key)) {
        ret = map[key];
    }
    else{
        ret = Gender::GENDER_UNKNOWN;
    }
    logd("Gender %d", ret);
    return ret;
}

qint64 Utils::dateFromString(const QString &date, const QString &f)
{
    QStringList listFormat;
    QStringList listDate;
    qint64 ret = 0;
    QString format = f.toUpper().simplified();
    traced;
    static QChar supportedSpli[] = {'/', '-', '.'}; // TODO: make it global????
    logd("Convert date '%s' to int, format '%s'",
         date.toStdString().c_str(),
         format.toStdString().c_str()
         );
    int len = sizeof(supportedSpli)/sizeof(supportedSpli[0]);

    for (int i = 0; i < len; i++){
        if (format.contains(supportedSpli[i]) && date.contains(supportedSpli[i])){
            listFormat = format.split(supportedSpli[i]);
            listDate = date.split(supportedSpli[i]);
            break;
        }
    }

    if (!listDate.empty() && !listFormat.empty()
        && (listDate.length() > 1)
        && (listDate.length() == listFormat.length())){

        qint64 year = 0, month = 0, day = 0;
        bool ok = false;
        int idx = 0;
        logd("parse each item in date");
        foreach (QString item, listFormat){
            ok = false;
            logd("item %s", item.toStdString().c_str());
            logd("listDate %s", listDate[idx].toStdString().c_str());
            if (item.simplified().startsWith("Y")){
                year = listDate[idx].toInt(&ok);

            } // TODO: support format like Jan, January???
            else if (item.simplified().startsWith("M")){
                month = listDate[idx].toInt(&ok);

            } // TODO: support format like Mon, Tuesday????
            else if (item.simplified().startsWith("D")){
                day = listDate[idx].toInt(&ok);
            }
            if (!ok){
                break;
            }
            idx ++;
        }
        logd("year %d moth %d date %d", year, month, day);
        if (ok){
            ret = YMD_TO_INT(year, month, day);
        }
        else{
            ret = 0;
            loge("Invalid data/format: parse faile");
        }
    }
    else{
        loge("Invalid data/format: note match date and format");
    }
    return ret;
}

QString Utils::date2String(qint64 date, const QString& format)
{
    traced;
    int year = 0, month = 0, day = 0;
    year = (date >> 16) & 0xFF;
    month = (date >> 8) & 0xFF;
    day = date & 0xFF;
    QString dateString;
    // TODO: use format
    if (year > 0) dateString += QString::number(year) + "/";
    if (month > 0) dateString += QString::number(month) + "/";
    if (day > 0) dateString += QString::number(day);
    return dateString;
}

void Utils::date2ymd(qint64 date, int *pday, int *pmonth, int *pyear)
{
    traced;
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
}

#define COMMENT '#'
ErrCode Utils::parseCSVFile(const QString &filePath, func_one_csv_item_t cb, void *paramCb, QChar splitBy)
{
    traced;
    ErrCode ret = ErrNone;
    if (!filePath.isEmpty())
    {

        QFile file(filePath);
        if (file.exists()){
            logi("Read file '%s' then parse", file.fileName().toStdString().c_str());
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                QTextStream stream(&file );
                qint32 cnt = 0;
                while(!stream.atEnd()) {
                    QString line = stream.readLine();
                    logd("> Line '%s'", line.toStdString().c_str());
                    line = line.simplified();
                    if (!line.isEmpty() && !line.startsWith(COMMENT))
                    {
                        QStringList items = line.split(splitBy);
                        logd("split int to %lld items", items.length());
                        if (!items.empty())
                        {
                            if (cb != nullptr){
                                ret = cb(items, paramCb);
                            }
                            if (ret != ErrNone){
                                loge("Line %d is invali", cnt);
                                break;
                            }
                        }
                        else {
                            loge("Line %d is invalid", cnt);
                            // TODO: should break or continue???
                        }

                    } // ignore line start with # or empty
                    else {
                        logi("Skip line %d", cnt);
                    }
                    cnt ++;
                }
                file.close();
            }
            else{
                ret = ErrFileRead;
                loge("Read file '%s' failed", filePath.toStdString().c_str());
            }
        }
        else{
            ret = ErrNotExist;
            loge("File %s not exist", filePath.toStdString().c_str());
        }
    }
    else{
        ret = ErrInvalidArg;
        loge("Invalid fielPath");
    }
    logd("ret %d", ret);
    return ret;
}

QString Utils::getPrebuiltFile(const QString &prebuiltName)
{
    // TODO: this is stupid/dump implementation, let's improve it, please
    return QString("%1_%2").arg(prebuiltName, KLanguage);
}
