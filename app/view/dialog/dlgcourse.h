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
 * Filename: dlgcourse.h
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#ifndef DLGCOURSE_H
#define DLGCOURSE_H

#include <QDialog>
#include "errcode.h"
#include "dlgcommoneditmodel.h"
#include "view/widget/uitableview.h"

class Course;

namespace Ui {
class DlgCourse;
}

class DlgCourse : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgCourse)

public:
    explicit DlgCourse(QWidget *parent = nullptr);
    ~DlgCourse();
    virtual void setupUI();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);
protected:
    virtual DbModel* newModel();
    void loadCourseType();
private slots:

    void on_btnChangeNameId_clicked();

    void on_txtName_textChanged(const QString &arg1);

private:
    Ui::DlgCourse *ui;
};

#endif // DLGCOURSE_H
