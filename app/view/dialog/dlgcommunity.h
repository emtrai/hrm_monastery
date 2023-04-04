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
 * Filename: dlgcommunity.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef DLGCOMMUNITY_H
#define DLGCOMMUNITY_H

#include <QDialog>
#include "errcode.h"
#include "dlgcommoneditmodel.h"
#include <QAbstractButton>

namespace Ui {
class DlgCommunity;
}

class Community;
class DlgCommunity : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgCommunity)
public:
                 explicit DlgCommunity(QWidget *parent = nullptr);
    ~DlgCommunity();
    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);
protected:
    virtual DbModel* newModel();
    void loadData();
    void loadStatus();
public:

private slots:
    void on_btnSearchCEO_clicked();

    void on_txtName_textChanged(const QString &arg1);

    void on_btnChangeNameId_clicked();

private:
    Ui::DlgCommunity *ui;
};

#endif // DLGCOMMUNITY_H
