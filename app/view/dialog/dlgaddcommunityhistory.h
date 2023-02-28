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
 * Filename: dlgaddcommunityhistory.h
 * Author: Anh, Ngo Huy
 * Created date:8/16/2022
 * Brief:
 */
#ifndef DLGADDCOMMUNITYHISTORY_H
#define DLGADDCOMMUNITYHISTORY_H

#include <QDialog>
#include <QAbstractButton>
#include "dbmodel.h"

class Community;

namespace Ui {
class DlgAddCommunityHistory;
}

class DlgAddCommunityHistory : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddCommunityHistory(QWidget *parent = nullptr);
    ~DlgAddCommunityHistory();

    const Community *getCommunity();
    qint64 getDate();
    QString getRemark();
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void accept();
private slots:
//    void on_buttonBox_clicked( QAbstractButton * button );
private:
    Ui::DlgAddCommunityHistory *ui;
    QList<DbModel*> mListCommunity;
};

#endif // DLGADDCOMMUNITYHISTORY_H
