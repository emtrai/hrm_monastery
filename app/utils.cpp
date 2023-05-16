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
#include <QObject>
#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include "dbmodel.h"

#include <QTextDocument>
#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif
#include "mainwindow.h"
#include "modelcontroller.h"
#include "stringdefs.h"

// yymd
#define YMD_TO_INT(y,m,d) (((y) << 16) | ((m) << 8) | (d))
#define YEAR_FROM_INT(date) ((date >> 16) & 0xFFFF)
#define MONTH_FROM_INT(date) ((date >> 8) & 0xFF)
#define DAY_FROM_INT(date) (date & 0xFF)

static const QChar supportedSpli[] = {'/', '-', '.'}; // TODO: make it global????

//void Utils::clearListModel(QList<DbModel *>& list)
//{
//    foreach (DbModel* model, list) {
//        if  (model) delete model;
//    }
//    list.clear();
//}

qint64 Utils::currentTimeMs()
{
    return QDateTime::currentMSecsSinceEpoch();
}

QString Utils::timeMsToDatestring(qint64 timeMs, const QString &format)
{
    tracein;
    logd("conver time ms %ld", timeMs);
    QString time = QDateTime::fromSecsSinceEpoch(timeMs).toString(format);
    logd("time in string %s", time.toStdString().c_str());
    traceout;
    return time;
}

Gender Utils::genderFromString(const QString &gender)
{
    static QHash<QString, Gender> map;
    static bool mapInit = false;
    tracein;

    if (!mapInit)
    {
        logd("Init map gender with string");
        // TODO: should do init somewhere????
        map["MALE"] = Gender::MALE;
        map["NAM"] = Gender::MALE;
        map["FEMALE"] = Gender::FEMALE;
        map["NU"] = Gender::FEMALE;
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

QString Utils::date2String(qint64 date, const QString& format, bool* isOk)
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

void Utils::date2ymd(qint64 date, int *pday, int *pmonth, int *pyear)
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
}

#define COMMENT '#'
#define NEXT_LINE '\\'
ErrCode Utils::parseCSVFile(const QString &filePath,
    std::function<ErrCode(const QStringList& items, void* caller, void* param, quint32 idx)> cb,
    void *caller,
    void *paramCb, QChar splitBy,
    qint32* cnt,
    QStringList* outItems) // TODO: therewill be the case that outIttem.length > 0 but cnt == 0
{
    tracein;
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
                            if (outItems != nullptr) {
                                outItems->append(items);
                            }
                            if (cb != nullptr){
                                ret = cb(items, caller, paramCb, count);
                            }
                            if (ret == ErrNone){
                                count ++;
                            } else {
                                // TODO: check this case again, should continue or break/stop??
                                loge("Line %d is invalid? ret = %d", cnt, ret);
//                                break;
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


#define START_NEW_ITEM "=="
ErrCode Utils::parseDataFromCSVFile(const QString &filePath,
                                    QList<QHash<QString, QString>>* items,
                                    QChar splitBy,
                                    func_one_csv_field_t cb,
                                    func_one_csv_item_complete_t oneModelCB,
                                    void* caller,
                                    void* paramCb)
{
    tracein;
    ErrCode ret = ErrNone;
    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.exists()){
            logi("Read file '%s' then parse", file.fileName().toStdString().c_str());
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                QTextStream stream(&file );
                qint32 cnt = 0;
                bool value_cont = false;
                bool item_ready = false;
                // TODO: catch exception????
                QHash<QString, QString> fields;
                QString key;
                QString value;
                while(!stream.atEnd()) {
                    QString line = stream.readLine();
                    logd("> Line '%s'", line.toStdString().c_str());
                    line = line.simplified();
                    if (!line.isEmpty() || value_cont) {
                        if (value_cont) {
                            if (line.endsWith(NEXT_LINE)) {
                                value += line.first(line.length()-1);
                                value_cont = true;
                                item_ready = false;
                            } else {
                                value += line;
                                value_cont = false;
                                item_ready = true;
                            }
                        } else if (line.startsWith(START_NEW_ITEM)) {
                            if (items != nullptr) {
                                items->append(fields);
                            }
                            if (oneModelCB != nullptr) {
                                oneModelCB(fields, caller, paramCb);
                            }

                            fields.clear();

                            value_cont = false;
                            item_ready = false;
                        } else if (!line.startsWith(COMMENT)) {
                            int idx = line.indexOf(splitBy);
                            logd("idx %d", idx);
                            if (idx >= 0){
                                key = line.mid(0, idx).simplified().toUpper();
                                value = line.mid(idx+1).trimmed();
                                if (value.endsWith(NEXT_LINE)) {
                                    value_cont = true;
                                    item_ready = false;
                                    value.truncate(value.length()-1);
                                } else {
                                    value_cont = false;
                                    item_ready = true;
                                }
                            } else {
                                item_ready = false;
                                value_cont = false;
                            }
                        } else {
                            value_cont = false;
                            item_ready = false;
                        }
                        if (item_ready) {
                            logd("key %s", key.toStdString().c_str());
                            logd("value %s", value.toStdString().c_str());
                            if (items != nullptr) {
                                fields.insert(key, value);
                            }
                            if (cb != nullptr){
                                logd("Parse one CSV field, call callback");
                                ret = cb(key, value, caller, paramCb);
                                if (ret != ErrNone){
                                    loge("Callback return error %d", ret);
                                    break;
                                }
                            }
                            key.clear();
                            value.clear();
                        }
                        // TODO: if not found delimiter, what next???
                        // TODO: handle wrap line?

                    } // ignore line start with # or empty
                    else {
                        logi("Skip line %d", cnt);
                    }
                    cnt ++;
                }
                if (fields.count() > 0) {
                    if (items != nullptr) {
                        items->append(fields);
                    }
                    if (oneModelCB != nullptr) {
                        oneModelCB(fields, caller, paramCb);
                    }
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
    tracein;
    QString normalize = name.simplified();
    QString uid;
    logd("conver type %d", type);
    logd("name %s", STR2CHA(name));
    switch (type){
    case HASH_NAME:
        logd("Calc uid from name with hash");
        uid = Crypto::hashString(normalize.toLower());
        break;
    case SIMPLIFY_UPPER:
        uid = normalize.toUpper();
        break;
    case NO_VN_MARK_UPPER:
        uid = removeVietnameseSign(normalize.toUpper());
        uid = removeSpecialChar(uid, "_");
        break;
    default:
        loge("Not support type %d", type);

        break;
    }
    if (isOk) *isOk = !uid.isEmpty();

    logd("Name '%s' -> uid '%s'", name.toStdString().c_str(),
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
    tracein;
    QString finalString = vietnameseString;
    int numEle = sizeof(VNSigns)/sizeof(QString);
    logd("numEle '%d'", numEle);
    for (int i = 1; i < numEle; i++)
        for (int j = 0; j < VNSigns[i].length(); j++)
            finalString = finalString.replace(VNSigns[i][j], VNSigns[0][i - 1]);

    logd("vietnameseString '%s' -> finalString '%s'",
         STR2CHA(vietnameseString), STR2CHA(finalString));
    traceout;
    return finalString;
}

QString Utils::removeSpecialChar(const QString &string, const QString &replacement, const QString &specialChar)
{
    tracein;
    QString finalString;
    int numEle = string.length();
    logd("numEle '%d'", numEle);
    logd("string '%s'", STR2CHA(string));
    logd("replacement '%s'", STR2CHA(replacement));
    logd("specialChar '%s'", STR2CHA(specialChar));
    bool isSpecial = false;
    for (int i = 0; i < numEle; i++) {
        isSpecial = false;
        for (int j = 0; j < specialChar.length(); j++) {
            if (string[i] == specialChar[j]) {
                if (!replacement.isEmpty()) {
                    finalString += replacement;
                }
                isSpecial = true;
                break;
            }
        }
        if (!isSpecial) {
            finalString += string[i];
        }
    }
    logd("finalString '%s'", finalString.toStdString().c_str());
    traceout;
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

void Utils::showMsgBox(const QString &msg)
{
    QMessageBox msgBox;
    tracein;
    logd("Msg box %s", msg.toStdString().c_str());
    // TODO: title???
    msgBox.setInformativeText(msg);
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
    traceout;
}

void Utils::showErrorBox(const QString &msg)
{
    tracein;
    logd("Error box %s", msg.toStdString().c_str());
    QMessageBox::critical(MainWindow::getInstance(), "Lỗi", msg, QMessageBox::Close);
    traceout;
}

void Utils::showErrorBox(int ret, const QString& msg)
{
    tracein;
    QString errMsg;
    if (!msg.isEmpty()) {
        errMsg.append(msg);
        errMsg.append("\n");
    }
    errMsg.append(QString("Lỗi ! Mã lỗi %1").arg(ret)); // TODO: translation
    showErrorBox(errMsg);
}

bool Utils::showConfirmDialog(QWidget *parent, const QString &title, const QString &message, std::function<void(void)> onAccept)
{
    tracein;
    QMessageBox::StandardButton reply;
    bool ok = false;
    if (!parent) parent = MainWindow::getInstance();
    reply = QMessageBox::question(parent, title, message,
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        ok = true;
        if (onAccept) onAccept();
    } else {
        logd("selected no");
        ok = false;
    }
    traceout;
    return ok;
}

QString Utils::showInputDialog(QWidget *parent, const QString &title, const QString &message, const QString& initInput, bool *isOk)
{
    tracein;
    QString text = QInputDialog::getText(parent, title,
                                         message, QLineEdit::Normal,
                                         initInput, isOk);
    logd("input text: '%s'", STR2CHA(text));
    traceout;
    return text;
}

ErrCode Utils::screenSize(int *w, int *h)
{
    tracein;
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
    tracein;
    int h = 0;
    screenSize(nullptr, &h);
    return h;
}


int Utils::getCurrentComboxIndex(const QComboBox *cb)
{
    tracein;
    QString currtxt = cb->currentText().trimmed();
    int index = cb->findText(currtxt);
    logd("Current text %s", currtxt.toStdString().c_str());
    logd("Index %d", index);
    return index;
}

QString Utils::getCurrentComboxDataString(const QComboBox *cb, bool *isOk)
{
    tracein;
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

ErrCode Utils::getCurrentComboxDataString(const QComboBox *cb, QString *data, QString *name)
{
    tracein;
    int idx = getCurrentComboxIndex(cb);
    logd("current idx %d", idx);
    ErrCode ret = ErrNone;
    if (idx >= 0){

        QString currTxt = cb->itemText(idx);
        QString value = cb->itemData(idx).toString();
        if (name != nullptr)
            *name = currTxt;

        if (!value.isEmpty()) {
            if (data != nullptr)
                *data = value;
        } else {
            ret = ErrNoData;
        }

    } else {
        ret = ErrInvalidData;
    }
    traceret(ret);
    return ret;
}

ErrCode Utils::setSelectItemComboxByData(QComboBox *cb, const QVariant &data, int* index)
{
    tracein;
    int cnt = cb->count();
    logd ("no. item %d", cnt);
    logd("data %s", STR2CHA(data.toString()));
    ErrCode ret = ErrNotFound;
    for (int i = 0; i < cnt; i++) {
        QVariant val = cb->itemData(i);
        // TODO: assume data is string, how about others kind of value????
        if (val.isValid() && data == val) {
            cb->setCurrentIndex(i);
            ret = ErrNone;
            logd("found item at %d: %s", i, data.toString().toStdString().c_str());
            if (index) *index = i;
            break;
        } else {
            cb->setCurrentIndex(0);
        }
    }
    traceret(ret);
    return ret;
}

ErrCode Utils::buildComboxFromModel(QComboBox *cb, const QList<DbModel *> &modelList)
{
    tracein;
    ErrCode err = ErrNone;
    if (!cb) {
        err = ErrInvalidArg;
        loge("Invalid argumet");
    }
    if (err == ErrNone) {
        cb->clear();
        cb->addItem(STR_UNKNOWN, "");
        foreach (DbModel* item, modelList) {
            cb->addItem(item->name(), item->uid());

        }
    }

    traceret(err);
    return err;
}

ErrCode Utils::buildComboxFromModel(QComboBox *cb, ModelController *controller)
{
    tracein;
    ErrCode err = ErrNone;
    if (!controller) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        QList<DbModel *> list = controller->getAllItems();
        err = buildComboxFromModel(cb, list);
        RELEASE_LIST_DBMODEL(list);
    }
    traceret(err);
    return err;
}


void Utils::showDlgUnderDev(const QString &info)
{
    QMessageBox msgBox;
    tracein;
    QString msg = QObject::tr("Tính năng đang phát triển");
    if (!info.isEmpty()) {
        msg += QObject::tr("\n* "); // TODO: translation???
        msg += info;
    }
    logd("Info%s", msg.toStdString().c_str());
    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.exec();
    traceout;
}

QString Utils::saveFileDialog(QWidget *parent,
                              const QString& title,
                              const QString& initFileName,
                              const QString& filter,
                              const QString& initDir)
{
    tracein;
    QString dir = !(initDir.isEmpty())?initDir:QDir::homePath();
    QString name = !(initFileName.isEmpty())?initFileName:"";
    QString fileFilter = !(filter.isEmpty())?filter:"Tất cả loại tập tin (*.*)";
    QString dlgTitle = !(title.isEmpty())?title:"Lưu tập tin";
    logd("dir %s", STR2CHA(dir));
    logd("name %s", STR2CHA(name));
    logd("filter %s", STR2CHA(fileFilter));
    logd("title %s", STR2CHA(dlgTitle));
    QString fpath = QFileDialog::getSaveFileName(parent,
                                                 dlgTitle,
                                                 dir + "/" + name,
                                                 fileFilter);
    logd("save to file '%s'", STR2CHA(fpath));
    traceout;
    return fpath;
}

ErrCode Utils::saveHtmlToPdf(const QString &htmlPath, const QString& initFname, QWidget *parent)
{
    // Export information to file
    tracein;
    ErrCode err = ErrNone;
    logd("export '%s' to html", STR2CHA(htmlPath));
    if (htmlPath.isEmpty() || !QFile::exists(htmlPath)) {
        err = ErrInvalidArg;
        loge("file '%s' is not exist or empty", STR2CHA(htmlPath));
    }
    if (err == ErrNone) {
        QString fileName = Utils::saveFileDialog(parent, QObject::tr("Xuất dữ liệu pdf"), initFname, "PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

            logd("export filename '%s'", STR2CHA(fileName));
            QPrinter printer(QPrinter::PrinterResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setPageSize(QPageSize::A4);
            printer.setOutputFileName(fileName);

            logi("Save html path '%s' to pdf '%s", STR2CHA(htmlPath), STR2CHA(fileName));
            QTextDocument doc;
            doc.setHtml(Utils::readAll(htmlPath));
            doc.print(&printer);

            Utils::showMsgBox(QString(QObject::tr("Xuất dữ liệu ra tập tin: %1")).arg(fileName));
        } else {
            loge("Can not export to pdf, file is empty or cancelled");
        }
    }
    traceout;
    return err;
}

