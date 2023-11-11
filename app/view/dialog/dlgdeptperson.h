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
 * Filename: dlgdeptperson.h
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#ifndef DLGDEPTPERSON_H
#define DLGDEPTPERSON_H

#include <QDialog>
#include "dlgcommoneditmodel.h"

namespace Ui {
class DlgDeptPerson;
}
class Person;
class PersonDept;
class DbModel;

class DlgDeptPerson : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgDeptPerson)

public:
    explicit DlgDeptPerson(QWidget *parent = nullptr);
    ~DlgDeptPerson();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);

    void setCommDeptUid(const QString &newCommDeptUid);

    void setCommDeptNameId(const QString &newCommDeptNameId);

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
    Ui::DlgDeptPerson *ui;
    QString mCommDeptUid;
    QString mCommDeptNameId;
};

#endif // DLGDEPTPERSON_H
