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
 * Filename: dlgconfirmupdatepeoplecomm.h
 * Author: Anh, Ngo Huy
 * Created date:6/13/2023
 * Brief:
 */
#ifndef DLGCONFIRMUPDATEPEOPLECOMM_H
#define DLGCONFIRMUPDATEPEOPLECOMM_H

#include <QDialog>
#include <QList>
#include <QAbstractButton>


namespace Ui {
class DlgConfirmUpdatePeopleComm;
}

class Community;
class DbModel;
class DlgConfirmUpdatePeopleComm : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConfirmUpdatePeopleComm(QWidget *parent = nullptr);
    ~DlgConfirmUpdatePeopleComm();
    const QList<DbModel *> personList() const;
    void setPersonList(const QList<DbModel *> &newPersonList);

    const Community *community() const;
    void setCommunity(const Community *newCommunity);

    bool addCommunityHistory();
    bool addPersonEvent();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DlgConfirmUpdatePeopleComm *ui;
    QList<DbModel*> mPersonList;
    Community* mCommunity;
};

#endif // DLGCONFIRMUPDATEPEOPLECOMM_H
