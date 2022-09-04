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
#include "logger.h"
#include "errcode.h"
#include <QFile>
#include "crypto.h"
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QRect>
#include <QComboBox>

// yymd
#define YMD_TO_INT(y,m,d) (((y) << 16) | ((m) << 8) | (d))

qint64 Utils::getCurrentTimeMs()
{
    return 0;
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

qint64 Utils::dateFromString(const QString &date, const QString &f, bool* isOk)
{
    QStringList listFormat;
    QStringList listDate;
    qint64 ret = 0;
    QString format = f.toUpper().simplified();
    bool ok = false;
    traced;
    static QChar supportedSpli[] = {'/', '-', '.'}; // TODO: make it global????
    logd("Convert date '%s' to int, format '%s'",
         date.toStdString().c_str(),
         format.toStdString().c_str()
         );
    int len = sizeof(supportedSpli)/sizeof(supportedSpli[0]);

//    for (int i = 0; i < len; i++){
//        if (format.contains(supportedSpli[i]) && date.contains(supportedSpli[i])){
//            listFormat = format.split(supportedSpli[i]);
//            listDate = date.split(supportedSpli[i]);
//            break;
//        }
//    }
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
    logd("listFormat length %d", listFormat.length());
    logd("listDate length %d", listDate.length());
    if (!listDate.empty() && !listFormat.empty()
        && (listDate.length() >= 1)){
//        && (listDate.length() > 1)
//        && (listDate.length() == listFormat.length())){

        qint32 year = 0, month = 0, day = 0;

        int idx = 0;
        logd("parse each item in date");
        foreach (QString item, listFormat){
            if (idx >= listDate.length())
                break;
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
    } else{
        loge("Invalid data/format: not match date and format");
    }
    if (isOk) *isOk = ok;
    logd("result date 0x%x, ok %d", (quint32) ret, ok);
    return ret;
}

QString Utils::date2String(qint64 date, const QString& format)
{
    traced;
    int year = 0, month = 0, day = 0;
    year = (date >> 16) & 0xFFFF;
    month = (date >> 8) & 0xFF;
    day = date & 0xFF;
    QString dateString;
    logd("Date 0x%x", (quint32)date);
    // TODO: use format
    if (year > 0) dateString += QString::number(year);

    if (month > 0){
        if (!dateString.isEmpty()) dateString += "/";
        dateString += QString::number(month);
    }

    if (day > 0) {
        if (!dateString.isEmpty()) dateString += "/";
        dateString += QString::number(day);
    }
    logd("DateString %s", dateString.toStdString().c_str());
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
ErrCode Utils::parseCSVFile(const QString &filePath,
    std::function<ErrCode(const QStringList& items, void* caller, void* param, quint32 idx)> cb,
    void *caller,
    void *paramCb, QChar splitBy,
    qint32* cnt)
{
    traced;
    ErrCode ret = ErrNone;
    qint32 count = 0;
    if (!filePath.isEmpty())
    {

        QFile file(filePath);
        if (file.exists()){
            logi("Read file '%s' then parse", file.fileName().toStdString().c_str());
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                QTextStream stream(&file );
                qint32 cnt = 0;
                // TODO: catch exception????
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
                                ret = cb(items, caller, paramCb, count);
                            }
                            if (ret == ErrNone){
                                count ++;
                            } else {
                                // TODO: check this case again, should continue or break/stop??
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
    if (cnt != nullptr) *cnt = count;
    logd("ret %d", ret);
    return ret;
}

ErrCode Utils::parseDataFromCSVFile(const QString &filePath,
                                    QHash<QString, QString>* items,
                                    QChar splitBy,
                                    func_one_csv_field_t cb,
                                    void* caller,
                                    void* paramCb)
{
    traced;
    ErrCode ret = ErrNone;
    if (!filePath.isEmpty() && (items != nullptr))
    {
        QFile file(filePath);
        if (file.exists()){
            logi("Read file '%s' then parse", file.fileName().toStdString().c_str());
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                QTextStream stream(&file );
                qint32 cnt = 0;
                // TODO: catch exception????
                while(!stream.atEnd()) {
                    QString line = stream.readLine();
                    logd("> Line '%s'", line.toStdString().c_str());
                    line = line.simplified();
                    if (!line.isEmpty() && !line.startsWith(COMMENT))
                    {
                        int idx = line.indexOf(splitBy);
                        QString key;
                        QString value;
                        logd("idx %d", idx);
                        if (idx >= 0){
                            key = line.mid(0, idx).simplified().toUpper();
                            value = line.mid(idx+1).trimmed();
                            logd("key %s", key.toStdString().c_str());
                            logd("value %s", value.toStdString().c_str());
                            if (items != nullptr) {
                                items->insert(key, value);
                            }
                            if (cb != nullptr){
                                logd("Parse one CSV field, call callback");
                                ret = cb(key, value, caller, paramCb);
                                if (ret != ErrNone){
                                    loge("Callback return error %d", ret);
                                    break;
                                }
                            }
                        }
                        // TODO: if not found delimiter, what next???
                        // TODO: handle wrap line?

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

QString Utils::getPrebuiltFileByLang(const QString &prebuiltName, bool lang)
{
    // TODO: this is stupid/dump implementation, let's improve it, please
    // TODO: KLanguate can be updatable to update language at runtime???
    if (lang)
        return QString("%1_%2").arg(prebuiltName, KLanguage);
    else
        return prebuiltName;
}


QString Utils::UidFromName(const QString &name, UidNameConvertType type, bool* isOk)
{
    traced;
    QString normalize = name.simplified();
    QString uid;
    logd("conver type %d", type);
    switch (type){
    case HASH_NAME:
        logd("Calc uid from name with hash");
        uid = Crypto::hashString(normalize.toLower());
        break;
    case SIMPLIFY_UPPER:
        uid = normalize.toUpper();
        break;
    case NO_VN_MARK_UPPER:
        uid = removeVietnameseSign(normalize.replace(' ', '_').toUpper());
        break;
    default:
        loge("Not support type %d", type);

        break;
    }
    if (isOk) *isOk = !uid.isEmpty();

    logd("Name %s -> uid %s", name.toStdString().c_str(),
         uid.toStdString().c_str());
    return uid;
}

// FIXME: if those char is ok if putting on code???
static const QString VNSigns[] =
{

    "aAeEoOuUiIdDyY",

    "áàạảãâấầậẩẫăắằặẳẵ",

    "ÁÀẠẢÃÂẤẦẬẨẪĂẮẰẶẲẴ",

    "éèẹẻẽêếềệểễ",

    "ÉÈẸẺẼÊẾỀỆỂỄ",

    "óòọỏõôốồộổỗơớờợởỡ",

    "ÓÒỌỎÕÔỐỒỘỔỖƠỚỜỢỞỠ",

    "úùụủũưứừựửữ",

    "ÚÙỤỦŨƯỨỪỰỬỮ",

    "íìịỉĩ",

    "ÍÌỊỈĨ",

    "đ",

    "Đ",

    "ýỳỵỷỹ",

    "ÝỲỴỶỸ"
};

QString Utils::removeVietnameseSign(const QString &vietnameseString)
{
    traced;
    QString finalString = vietnameseString;
    int numEle = sizeof(VNSigns)/sizeof(QString);
    logd("numEle '%d'", numEle);
    for (int i = 1; i < numEle; i++)
        for (int j = 0; j < VNSigns[i].length(); j++)
            finalString = finalString.replace(VNSigns[i][j], VNSigns[0][i - 1]);

    logd("finalString '%s'", finalString.toStdString().c_str());
    return finalString;
}

QString Utils::readAll(const QString &fpath)
{
    QFile file(fpath);
    // TODO: handle error
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    return in.readAll();
}

void Utils::showErrorBox(const QString &msg)
{
    QMessageBox msgBox;
    traced;
    logd("Error box %s", msg.toStdString().c_str());
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.addButton(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.exec();
}

ErrCode Utils::screenSize(int *w, int *h)
{
    traced;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    if (w) *w = width;
    if (h) *h = height;
    logd("w %d h %d", height, width);
    // TODO: screen is screen showing app??? should check to know exact screen in multiscreen
    return ErrNone; // TODO; check if valid data is returned

}

int Utils::screenHeight()
{
    traced;
    int h = 0;
    screenSize(nullptr, &h);
    return h;
}


int Utils::getCurrentComboxIndex(const QComboBox *cb)
{
    traced;
    QString currtxt = cb->currentText().trimmed();
    int index = cb->findText(currtxt);
    logd("Current text %s", currtxt.toStdString().c_str());
    logd("Index %d", index);
    return index;
}

QString Utils::getCurrentComboxDataString(const QComboBox *cb, bool *isOk)
{
    traced;
    int idx = getCurrentComboxIndex(cb);
    logd("current idx %d", idx);
    QString ret;
    if (idx >= 0){
        if (isOk) *isOk = true;
        ret = cb->itemData(idx).toString();
        if (ret.isEmpty())
            if (isOk) *isOk = false;
    } else {
        if (isOk) *isOk = false;
    }
    return ret;
}
