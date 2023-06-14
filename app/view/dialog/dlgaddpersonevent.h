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
 * Filename: dlgaddpersonevent.h
 * Author: Anh, Ngo Huy
 * Created date:9/15/2022
 * Brief:
 */
#ifndef DLGADDPERSONEVENT_H
#define DLGADDPERSONEVENT_H

#include <QDialog>
#include <QHash>
#include "dlgcommoneditmodel.h"
class PersonEvent;
class Event;
class Person;

namespace Ui {
class DlgAddPersonEvent;
}

class DlgAddPersonEvent : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgAddPersonEvent)

public:
    explicit DlgAddPersonEvent(QWidget *parent = nullptr);
    ~DlgAddPersonEvent();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);

    void loadEvent();

    void setSelectedEvent(const QString& eventUid);
    ErrCode setPerson(const Person *newPerson);

    void setEvenInfoOnly(const QList<DbModel*>* listPerson = nullptr);
    const Event* getSelectedEvent() const;

protected:
    virtual QDialogButtonBox* buttonBox();
    virtual DbModel* newModel();

    void setSelectedEvent(int index);
    void updateNameId();
    virtual bool onValidateData(QString& msg);
private slots:
    void on_btnSearch_clicked();

    void on_btnChangeNameId_clicked();

    void on_txtDate_textChanged(const QString &arg1);

    void on_cbEvent_currentIndexChanged(int index);

private:
    Ui::DlgAddPersonEvent *ui;
    Person* mPerson;
    Event* mEvent;
    QList<DbModel*> mEventList;
    bool mEvenInfoOnly;
};

#endif // DLGADDPERSONEVENT_H
