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
 * Filename: DlgArea.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef DLGAREA_H
#define DLGAREA_H

#include <QDialog>
#include "errcode.h"
#include "dlgcommoneditmodel.h"
#include <QAbstractButton>

namespace Ui {
class DlgArea;
}

class DlgArea : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgArea)
public:
                 explicit DlgArea(QWidget *parent = nullptr);
    ~DlgArea();
    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);
protected:
    virtual QDialogButtonBox* buttonBox();
    virtual DbModel* newModel();
    void loadData();
    void loadStatus();
public:

private slots:
    void on_txtName_textChanged(const QString &arg1);

    void on_btnChangeNameId_clicked();

private:
    Ui::DlgArea *ui;
};

#endif // DLGAREA_H
