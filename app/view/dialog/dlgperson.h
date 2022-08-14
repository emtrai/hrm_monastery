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
 * Filename: dlgperson.h
 * Author: Anh, Ngo Huy
 * Created date:7/26/2022
 * Brief:
 */
#ifndef DLGPERSON_H
#define DLGPERSON_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class DlgPerson;
}

class DlgPerson : public QDialog
{
    Q_OBJECT

        public:
                 explicit DlgPerson(QWidget *parent = nullptr);
    ~DlgPerson();

             private slots:
                 void on_btnImport_clicked();

             private:
    Ui::DlgPerson *ui;

             private:
    void setupUI();

//protected:
//    virtual void accept();
private slots:
    void on_buttonBox_clicked( QAbstractButton * button );
    void on_btnEditNation_clicked();
    void on_cbCountry_currentIndexChanged(int index);
};

#endif // DLGPERSON_H
