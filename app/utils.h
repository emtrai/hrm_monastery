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

#define DEFAULT_CSV_FIELD_SPLIT ':'

//TODO: change default split to '|'? '|' is not common used like ','
#define DEFAULT_CSV_ITEM_SPLIT ','
#define CSV_LIST_ITEM_SPLIT '|'
#define NAME_ITEM_SPLIT ','

#define SPECIAL_CHAR "`~!@#$%^&*()_-+={[}]|\\:;\"\'<,>.?/ "

// TODO: just create empty object, is there any other better way???
#define EMPTY_QSTRING QString()
// TODO: is there any better way???
#define QSTRING_EMPTY(str) str.isEmpty()

#define GET_INSTANCE_DECL(className) \
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

#define STR2CHA(val) (!(val).isEmpty()?(val).toStdString().c_str():"(empty)")
#define MODELSTR2CHA(model) (model?STR2CHA(model->toString()):"(null)")
#define MODELNAME2CHA(model) (model?STR2CHA(model->modelName()):"(null)")

// TODO: re-implement this one!!!!!
#define ASSERT(cond, msg) \
do { \
        if (!(cond)) { \
            loge("FATAL ERROR! %s", msg); \
            qFatal(msg); \
    } \
} while(0)

#define ASSERT2(cond) \
do { \
        if (!(cond)) { \
            loge("FATAL ERROR! %s", #cond); \
            qFatal(#cond); \
    } \
} while(0)

#define FAIL(msg) ASSERT(false, msg)

#define DIALOG_SIZE_SHOW(dlg) \
                do {\
                    int h = 0;\
                    int w = 0;\
                    Utils::screenSize(&w, &h);\
                    dlg->resize(w/2, h-100);\
                } while (0)


#define SET_VAL_FROM_WIDGET(widget,func) \
do { \
        QString val = widget->text().trimmed();\
        func(val);\
} while (0)

#define SET_VAL_FROM_CBOX(widget,func, functxt, err) \
    do { \
            QString currtxt = widget->currentText().trimmed();\
            if (!currtxt.isEmpty()){ \
                int index = widget->findText(currtxt);\
                logd("item '%s', index %d", STR2CHA(currtxt), index);\
                if (index >= 0){ \
                    QVariant value = widget->itemData(index);\
                    if (!value.isNull()) {\
                        func(value.toString());\
                        functxt(currtxt);\
                    }\
                } else { \
                    loge("item '%s' not found", STR2CHA(currtxt));\
                    err = ErrNotFound; \
                } \
            }\
    } while (0)


#define SET_VAL_FROM_VAL_CBOX(widget,func, functxt, err) \
do { \
    int index = widget->currentIndex(); \
    QString currtxt = widget->currentText().trimmed();\
    logd("index %d, name %s", index, STR2CHA(currtxt));\
    if (index >= 0){ \
        QVariant value = widget->itemData(index);\
        if (!value.isNull()) {\
            func(value.toString());\
            functxt(currtxt);\
        }\
    } else { \
        dbgd("item '%s' not found, try one more", STR2CHA(currtxt));\
        SET_VAL_FROM_CBOX(widget, func, functxt, err); \
    } \
} while (0)

#define SET_MODEL_FROM_CBOX(widget,func, ctrl, DbModelType, err) \
do { \
    QString currtxt = widget->currentText().trimmed();\
    if (!currtxt.isEmpty()){ \
        int index = widget->findText(currtxt);\
        logd("item '%s', index %d", STR2CHA(currtxt), index);\
        if (index >= 0){ \
            QVariant value = widget->itemData(index);\
            if (!value.isNull()) {\
                QString uid = value.toString();\
                logd("uid %s", STR2CHA(uid));\
                DbModel* model = ctrl->getModelByUid(uid); \
                if (model) { \
                    func((DbModelType*)model); \
                } else { \
                    loge("set model from cbox failed, uid '%s' not found in ctrl '%s'", \
                        STR2CHA(uid), STR2CHA(ctrl->getName()));\
                } \
            }\
        } else { \
                loge("item '%s' not found", STR2CHA(currtxt));\
                err = ErrNotFound; \
        } \
    }\
} while (0)


#define SET_MODEL_FROM_VAL_CBOX(widget,func, ctrl, DbModelType, err) \
    do { \
        int index = widget->currentIndex(); \
        QString currtxt = widget->currentText().trimmed();\
        logd("index %d, name %s", index, STR2CHA(currtxt));\
        if (index >= 0){ \
            QVariant value = widget->itemData(index);\
            if (!value.isNull()) {\
                QString uid = value.toString();\
                logd("uid %s", STR2CHA(uid));\
                DbModel* model = ctrl->getModelByUid(uid); \
                if (model) { \
                    func((DbModelType*)model); \
                } else { \
                        loge("set model from cbox failed, uid '%s' not found in ctrl '%s'", \
                             STR2CHA(uid), STR2CHA(ctrl->getName()));\
                } \
            }\
        }\
    } while (0)

// TODO: should common with above macro???
// TODO: should check if value is actually integer??? exception???
#define SET_INT_VAL_FROM_CBOX(widget,func, functxt) \
do { \
    int index = widget->currentIndex(); \
    QString currtxt = widget->currentText().trimmed();\
    logd("index %d, name %s", index, STR2CHA(currtxt));\
    if (index >= 0){ \
        QVariant value = widget->itemData(index);\
        if (value.isValid()) {\
            func(value.toInt());\
            functxt(currtxt);\
        }\
    }\
} while (0)

#define SET_VAL_FROM_TEXTBOX(widget, itemName, func, functxt) \
do { \
    logd("SET_VAL_FROM_TEXTBOX, itemName %s", itemName);\
    QString currtxt = widget->text().trimmed();\
    if (!currtxt.isEmpty()){ \
        logd("currtxt %s", currtxt.toStdString().c_str());\
        QVariant value = widget->property(itemName);\
        if (!value.isNull()) {\
            logd("value %s", value.toString().toStdString().c_str());\
            func(value.toString());\
            functxt(currtxt);\
        } else {\
            logd("%s has no data", itemName);\
        }\
    }\
} while (0)

#define SET_MODEL_FROM_TEXTBOX(widget, itemName, func, ctrl, DbModelType, err) \
    do { \
        logd("SET_MODEL_FROM_TEXTBOX, itemName %s", itemName);\
        QString currtxt = widget->text().trimmed();\
        if (!currtxt.isEmpty()){ \
            logd("currtxt %s", currtxt.toStdString().c_str());\
            QVariant value = widget->property(itemName);\
            if (!value.isNull()) { \
                QString uid = value.toString();\
                logd("uid %s", STR2CHA(uid));\
                DbModel* model = ctrl->getModelByUid(uid); \
                if (model) { \
                    func((DbModelType*)model); \
                } else { \
                    loge("set model from textbox failed, uid '%s' not found in ctrl '%s'", \
                        STR2CHA(uid), STR2CHA(ctrl->getName()));\
                    err = ErrNotFound; \
                } \
            } else {\
                logd("%s has no data", itemName);\
            }\
        }\
    } while (0)

#define SET_VAL_FROM_EDITBOX(widget, itemName, func, functxt) \
do { \
        logd("SET_VAL_FROM_EDITBOX, itemName %s", itemName);\
        QString currtxt = widget->toPlainText().trimmed();\
        if (!currtxt.isEmpty()){ \
            logd("currtxt %s", currtxt.toStdString().c_str());\
            QVariant value = widget->property(itemName);\
            if (!value.isNull()) {\
                logd("value %s", value.toString().toStdString().c_str());\
                func(value.toString());\
                functxt(currtxt);\
        } else {\
                logd("%s has no data", itemName);\
        }\
    }\
} while (0)

#define SET_TEXTBOX_FROM_VALUE(wget, itemName, value, txt) \
do { \
    logd("SET_TEXTBOX_FROM_VALUE, itemName %s", itemName);\
    if (!txt.isEmpty()){ \
        logd("txt %s", txt.toStdString().c_str());\
        wget->setText(txt);\
        logd("setProperty %s", value.toStdString().c_str());\
        wget->setProperty(itemName, value);\
    } else {\
        logd("%s has no data", itemName);\
    }\
} while (0)

#define GET_VAL_INT_FROM_CB(widget, out) \
    do { \
        QString currtxt = widget->currentText().trimmed();\
        if (!currtxt.isEmpty()){ \
            int index = widget->findText(currtxt);\
            logd("item %s, index %d", currtxt.toStdString().c_str(), index);\
            if (index >= 0){ \
                QVariant value = widget->itemData(index);\
                if (!value.isNull()) { \
                    out = value.toInt(); \
                }\
            }\
        }\
    } while (0)



#define FULLNAME(firstName, lastName) QString("%1 %2").arg(lastName, firstName)


#define RELEASE_LIST(list, T) \
    do { \
    Utils::clearListModel<T>(list); \
    } while(0)


#define RELEASE_LIST_DBMODEL(list) \
    RELEASE_LIST(list, DbModel)

#define RELEASE_HASH(list, K, T) \
    do { \
        Utils::clearListModel<K, T>(list); \
    } while(0)

#define FREE_PTR(ptr) \
do { \
    if (ptr) { \
        delete ptr; \
        ptr = nullptr; \
    } \
} while(0)

#define APPEND_STRING(str, newStr, sep) \
do { \
    if (!newStr.isEmpty()) { \
        if (!str.isEmpty()) str += sep; \
        str += newStr; \
    } \
} while(0)

typedef ErrCode (*func_one_csv_item_t)(const QStringList& items, void* caller, void* param);
typedef ErrCode (*func_one_csv_item_complete_t)(const QHash<QString, QString>& items, void* caller, void* param);
typedef ErrCode (*func_one_csv_field_t)(const QString& key, const QString& value, void* caller, void* param);

enum UidNameConvertType {
    SIMPLIFY_UPPER = 0,
    NO_VN_MARK_UPPER,
    HASH_NAME,
    MAX
};


class QComboBox;
class DbModel;
class ModelController;
class Utils
{
public:
    template<class T>
    static void clearListModel(QList<T*>& list)
    {
        foreach (T* model, list) {
            if  (model) delete model;
        }
        list.clear();
    }
    template<class K, class T>
    static void clearListModel(QHash<K, T*>& list)
    {
        foreach (K key, list.keys()) {
            T* model = list.value(key);
            if  (model) delete model;
        }
        list.clear();
    }
    static Gender genderFromString(const QString& gender);
    /**
    * Y/M/D
    * M/D
    */

    static ErrCode parseCSVFile(const QString& filePath,
                                std::function<
                                    ErrCode(const QStringList& items, void* caller, void* param, quint32 idx)>
                                    cb = nullptr,
                                void* caller = nullptr,
                                void* paramCb = nullptr,
                                QChar splitBy = DEFAULT_CSV_ITEM_SPLIT,
                                qint32* cnt = nullptr,
                                QStringList* outItems = nullptr
                                );
    static ErrCode parseDataFromCSVFile(const QString& filePath,
                                        QList<QHash<QString, QString>>* items = nullptr,
                                        QChar splitBy = DEFAULT_CSV_FIELD_SPLIT,
                                        func_one_csv_field_t oneFieldCB = nullptr,
                                        func_one_csv_item_complete_t oneModelCB = nullptr,
                                        void* caller = nullptr,
                                        void* paramCb = nullptr
                                        );
    static QString getPrebuiltFileByLang(const QString& prebuiltName, bool lang = true);
    static QString UidFromName(const QString& name, UidNameConvertType type = SIMPLIFY_UPPER, bool* isOk = nullptr);
    static bool appendString(QString& target, const QString& value, const QString& sep = "_");
    static QString removeVietnameseSign(const QString& vietnameseString);
    static QString removeSpecialChar(const QString& string,
                                     const QString& replacement = nullptr,
                                     const QString& specialChar = SPECIAL_CHAR);
    static QString readAll(const QString& fpath);

    static ErrCode screenSize(int* w=nullptr, int* h=nullptr);
    static int screenHeight();
    static int getCurrentComboxIndex(const QComboBox *cb);;
    static QString getCurrentComboxDataString(const QComboBox *cb, bool *isOk = nullptr);
    static ErrCode getCurrentComboxDataString(const QComboBox *cb, QString* data, QString* name = nullptr);
    static ErrCode setSelectItemComboxByData(QComboBox *cb, const QVariant& data, int* index = nullptr);
    static ErrCode buildComboxFromModel(QComboBox *cb, const QList<DbModel*>& modelList);
    static ErrCode buildComboxFromModel(QComboBox *cb, ModelController* controller);
    static QString normalizeFileName(const QString& fname, const QChar& replaceChar = '_');

    template<class T>
    static bool isSameList(const QList<T>& l1, const QList<T>& l2);

    static ErrCode saveHtmlToPdf(const QString& htmlPath, const QString& initFname = "xuatdulieu", QWidget *parent = nullptr);
};


template<class T>
bool Utils::isSameList(const QList<T>& l1, const QList<T>& l2)
{
    tracein;
    bool ret = false;
    logd("compare l1 cnt=%lld l2 cnt=%lld", l1.count(), l2.count());
    if (l1.count() != l2.count()) {
        ret = false;
    } else {
        int cnt = l1.count();
        if (cnt != 0) {
            ret = true;
            for (int i = 0; i < cnt; i ++){
                if (l1[i] != l2[i]) { // TODO: pointer issue!!!!
                    ret = false;
                    break;
                }
            }
        } else {
            ret = true;
        }
    }
    logd("is same list: %d", ret);
    traceout;
    return ret;
}
#endif // UTILS_H
