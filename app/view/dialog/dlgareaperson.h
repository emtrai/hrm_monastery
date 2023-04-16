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
 * Filename: DlgAreaPerson.h
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#ifndef DLGAREAPERSON_H
#define DLGAREAPERSON_H

#include <QDialog>
#include "dlgcommoneditmodel.h"

namespace Ui {
class DlgAreaPerson;
}
class Person;
class Area;
class DbModel;

class DlgAreaPerson : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgAreaPerson)

public:
    explicit DlgAreaPerson(QWidget *parent = nullptr);
    ~DlgAreaPerson();
    virtual void setupUI();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);

    void setAreaUid(const QString &id);

    void setAreaNameId(const QString &id);

    void setArea(Area *newArea);

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
    Ui::DlgAreaPerson *ui;
    QString mAreaUid;
    QString mAreaNameId;
    Area* mArea;
};

#endif // DLGAREAPERSON_H
