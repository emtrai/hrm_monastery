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
 * Filename: dlgdeptmgr.h
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#ifndef DLGDEPTMGR_H
#define DLGDEPTMGR_H

#include <QDialog>

namespace Ui {
class DlgDeptMgr;
}
class Person;
class PersonDept;

class DlgDeptMgr : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDeptMgr(QWidget *parent = nullptr);
    ~DlgDeptMgr();


    const QList<PersonDept *> &selectedPersons() const;
    void setSelectedPersons(const QList<PersonDept *> &newSelectedPersons);

protected:
    void loadCourse();
    void loadRole();
    virtual void showEvent(QShowEvent *event);
    void accept();
private slots:
    void on_btnSearch_clicked();

private:
    Ui::DlgDeptMgr *ui;
    QList<PersonDept*> mSelectedPersons;
    QList<Person*> mListPerson;
    bool mIsMultiSelection;
};

#endif // DLGDEPTMGR_H
