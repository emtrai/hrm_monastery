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
#include <QObject>
#include "dbmodel.h"

#include <QTextDocument>
#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif
#include "mainwindow.h"
#include "modelcontroller.h"
#include "stringdefs.h"
#include "datetimeutils.h"
#include "dialogutils.h"


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
        QString fileName = DialogUtils::saveFileDialog(parent, QObject::tr("Xuất dữ liệu pdf"), initFname, "PDF (*.pdf)");
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

            DialogUtils::showMsgBox(QString(QObject::tr("Xuất dữ liệu ra tập tin: %1")).arg(fileName));
        } else {
            loge("Can not export to pdf, file is empty or cancelled");
        }
    }
    traceout;
    return err;
}

