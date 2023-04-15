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
    virtual void setupUI();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);

    /**
     * @brief return selected item. Caller must free data after use
     * @return list of PersonDept object
     */
    QList<DbModel *> selectedPersons() const;

//    void setCommDeptUid(const QString &newCommDeptUid);

//    void setCommDept(DbModel *newCommDept);

    void setCommDeptUid(const QString &newCommDeptUid);

    void setCommDeptNameId(const QString &newCommDeptNameId);

protected:
    virtual DbModel* newModel();
    virtual bool onValidateData(QString& msg);
    void loadCourse();
    void loadRole();
    void loadStatus();
//    virtual void showEvent(QShowEvent *event);
//    void accept();
private slots:
    void on_btnSearch_clicked();

//    void on_btnAllPeople_clicked();
//    void updatePeopleList(const QList<DbModel*>& list);

private:
    Ui::DlgDeptPerson *ui;
    QList<DbModel*> mSelectedPersons;
    QList<DbModel*> mListPerson;
    bool mIsMultiSelection;
    QString mCommDeptUid;
    QString mCommDeptNameId;
};

#endif // DLGDEPTPERSON_H
