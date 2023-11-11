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
 * Filename: dlgethnic.h
 * Author: Anh, Ngo Huy
 * Created date:8/28/2022
 * Brief:
 */
#ifndef DLGETHNIC_H
#define DLGETHNIC_H

#include <QDialog>
#include "dlgcommoneditmodel.h"
class Ethnic;

namespace Ui {
class DlgEthnic;
}

class DlgEthnic : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgEthnic)

public:
    explicit DlgEthnic(QWidget *parent = nullptr);
    ~DlgEthnic();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);
protected:
    virtual DbModel* newModel();
    void loadCountry();
    virtual QDialogButtonBox* buttonBox() ;
private slots:
    void on_txtName_textChanged(const QString &arg1);

    void on_txtChange_clicked();

private:
    Ui::DlgEthnic *ui;
    Ethnic* mEthnic;
};

#endif // DLGETHNIC_H
