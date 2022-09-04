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
#include "view/widget/uimulticomboxview.h"
class Person;
class QComboBox;

namespace Ui {
class DlgPerson;
}

class DlgPerson : public QDialog, public UIMultiComboxViewListener
{
    Q_OBJECT

        public:
                 explicit DlgPerson(QWidget *parent = nullptr);
                    ~DlgPerson();

                    Person *person(bool newone=false);

             private slots:
                 void on_btnImport_clicked();

             private:
                Ui::DlgPerson *ui;

             private:
                void setupUI();
                Person *buildPerson();
                ErrCode fromPerson(const Person* person);

             public:
                virtual ErrCode onNewItem(UIMultiComboxView* ui, const QString& value, bool silent);
                virtual void onItemAdded(UIMultiComboxView* ui, const QString& name, const QVariant& value);
                virtual void onItemDeleted(UIMultiComboxView* ui, const QString& name, const QVariant& value);
//protected:
//    virtual void accept();
            private:
                void loadEdu();
                void loadEthnic();
                void loadCourse();
                void loadCountry();
                void loadProvince();
                void loadWork();
                void loadEvent();

private slots:
    void on_buttonBox_clicked( QAbstractButton * button );
    void on_btnEditNation_clicked();
    void on_cbCountry_currentIndexChanged(int index);
    void on_btnAddCommunityHistory_clicked();
    void on_tblClearCommunity_clicked();
    void on_btnPreview_clicked();
    void on_btnImg_clicked();

    void on_btnClearImage_clicked();

    void on_btnAddCountry_clicked();

    void on_btnAddEthnic_clicked();

    void on_btnAddEdu_clicked();

    void on_btnAddProvince_clicked();

    void on_btnAddCommunity_clicked();

    void on_btnAddCourse_clicked();

    void on_btnAddWork_clicked();

private:
    Person* mPerson;
    UIMultiComboxView *cbSaints;
    UIMultiComboxView *cbSpecialist;
};

#endif // DLGPERSON_H
