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
 * Filename: dlgcommonmodel.h
 * Author: Anh, Ngo Huy
 * Created date:12/29/2022
 * Brief:
 */
#ifndef DLGCOMMONEDITMODEL_H
#define DLGCOMMONEDITMODEL_H
#include "errcode.h"
#include "logger.h"
#include <QDialog>
#include "modelcontroller.h"
#include <QComboBox>
#include <QLineEdit>

#define DLG_BUILDER(className) \
public: \
inline static className* build(QWidget *parent = nullptr, \
                    bool isSelfSave = true, \
                    DbModel* model = nullptr, \
                    CommonEditModelListener *listener = nullptr) { \
    tracein; \
    className* dlg = nullptr; \
    dlg = new className(parent); \
    dlg->setupUI();\
    dlg->setIsSelfSave(isSelfSave); \
    dlg->setListener(listener); \
    if (model != nullptr) { \
        dlg->setIsNew(false); \
        dlg->fromModel(model); \
    } else { \
        dlg->setIsNew(true); \
    } \
    traceout; \
    return dlg; \
}

class DbModel;
class DlgCommonEditModel;
class QDialogButtonBox;

class CommonEditModelListener
{
public:
    virtual QString getName() = 0;
    virtual void onDbModelReady(ErrCode ret, DbModel* model, DlgCommonEditModel* dlg) = 0;
    virtual DbModel* onNewModel() = 0;
};

class DlgCommonEditModel: public QDialog
{
public:
    DlgCommonEditModel(QWidget *parent);
    virtual ~DlgCommonEditModel();
    virtual void setupUI();
    virtual bool isNew() const;
    virtual void setIsNew(bool newIsNew);

    virtual bool isSelfSave() const;
    virtual void setIsSelfSave(bool newIsSelfSave);
    /**
     * @brief build data for model, using dialog/wiget data
     * @return
     */
    virtual ErrCode buildModel(DbModel* model, QString& errMsg) = 0;
    /**
     * @brief build model from input model object
     * @param model
     * @return
     */
    virtual ErrCode fromModel(const DbModel* model);

    CommonEditModelListener *listener() const;
    void setListener(CommonEditModelListener *newListener);

    void setModel(DbModel *newModel);

protected:
    virtual QDialogButtonBox* buttonBox();
    /**
     * @brief Create new model object, must be implemeted by derived class
     * @param model: if null, create new one completely, else create base on this
     * @return
     */
    virtual DbModel* newModel() = 0;
    /**
     * @brief return current model object, create new if not created yes
     * @return
     */
    virtual DbModel* model();

    virtual void accept();
    /**
     * @brief Validate data of dialog
     * @param msg: output message if validate is not ok
     * @return
     */
    virtual bool onValidateData(QString& msg);

    virtual ErrCode loadList(QComboBox* cb, ModelController* ctrl);
    virtual void onChangeNameIdTxt(QLineEdit* txt, const QString &arg1);
    virtual void onEditnameId(QLineEdit* txt);
protected:

    DbModel* mModel;
    bool mIsNew; // true: add new, false: edit/update
    bool mIsSelfSave; //true: auto save on accept, false: not save
    CommonEditModelListener* mListener;
    bool mCustomNameId;
};

#endif // DLGCOMMONEDITMODEL_H
