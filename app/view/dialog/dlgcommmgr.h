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
 * Filename: dlgcommmgr.h
 * Author: Anh, Ngo Huy
 * Created date:10/15/2023
 * Brief:
 */
#ifndef DLGCOMMMGR_H
#define DLGCOMMMGR_H

#include <QDialog>
#include "dlgcommoneditmodel.h"

namespace Ui {
class DlgCommMgr;
}
class Person;
class Community;
class DbModel;
class DlgCommMgr : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgCommMgr)

public:
    explicit DlgCommMgr(QWidget *parent = nullptr);
    ~DlgCommMgr();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);


    /**
     * @brief Set new comm
     * @param newArea New comm, will be cloned to dlg, can be NULL to clear current comm
     * @return ErrNone on success
     */
    ErrCode setCommunity(const Community *comm);

protected:
    virtual QDialogButtonBox* buttonBox();
    virtual DbModel* newModel();
    virtual bool onValidateData(QString& msg);
    void loadCourse();
    void loadRole();
    void loadStatus();
private slots:
    void on_btnSearch_clicked();

private:
    Ui::DlgCommMgr *ui;
    Community* mCommunity;
};

#endif // DLGCOMMMGR_H
