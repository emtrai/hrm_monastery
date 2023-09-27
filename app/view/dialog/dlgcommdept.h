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
 * Filename: dlgcommdept.h
 * Author: Anh, Ngo Huy
 * Created date:4/8/2023
 * Brief:
 */
#ifndef DLGCOMMDEPT_H
#define DLGCOMMDEPT_H

#include <QDialog>

#include "errcode.h"
#include "dlgcommoneditmodel.h"
#include <QAbstractButton>

#include "community.h"

namespace Ui {
class DlgCommDept;
}


class DlgCommDept : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgCommDept)

public:
    explicit DlgCommDept(QWidget *parent = nullptr);
    ~DlgCommDept();
    virtual void setupUI();
    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);
protected:
    virtual DbModel* newModel();
    void loadDept();
    void loadStatus();
public:
    DbModel *community() const;
    void setCommunity(const DbModel *newCommunity);

private slots:
    void on_cbDept_currentIndexChanged(int index);

private:
    Ui::DlgCommDept *ui;
    DbModel* mCommunity;
};

#endif // DLGCOMMDEPT_H
