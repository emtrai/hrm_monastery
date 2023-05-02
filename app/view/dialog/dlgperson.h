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
#include "dlgcommoneditmodel.h"

class Person;
class QComboBox;
class DbModel;
class PersonEvent;
class QLineEdit;
namespace Ui {
class DlgPerson;
}


class DlgPerson : public QDialog, public UIMultiComboxViewListener, public CommonEditModelListener
{
    Q_OBJECT

    enum Mode {
        NEW = 0,
        EDIT,
        MAX
    };

        protected:
                 explicit DlgPerson(QWidget *parent = nullptr);
        public:
                    ~DlgPerson();

                    Person *person(bool newone=false);

                public:
                    static DlgPerson* buildDlg(QWidget *parent = nullptr, Person* per = nullptr, bool isNew = true);

//                    implemete it
//                    add model name to common edit model, pass it to onNewModel
                    virtual QString getName();
                    virtual void onDbModelReady(ErrCode ret, DbModel* model, DlgCommonEditModel* dlg);
                    virtual DbModel* onNewModel(const QString& modelName);
             private:
                Ui::DlgPerson *ui;

             private:
                void setupUI();
                ErrCode buildPerson(Person* person);
                ErrCode fromPerson(const Person* person);
                void multiComboxItemUpdate(UIMultiComboxView *cb, QLineEdit* txt = nullptr);
             public:
                virtual ErrCode onComboxNewItem(UIMultiComboxView* ui, const QString& value, bool silent);
                virtual void onComboxItemAdded(UIMultiComboxView* ui, const QString& name, const QVariant& value);
                virtual void onComboxItemDeleted(UIMultiComboxView* ui, const QString& name, const QVariant& value);
                virtual void onComboxClearAll();

//protected:
                //    virtual void accept();
                DlgPerson::Mode editMode() const;
                void setEditMode(DlgPerson::Mode newEditMode);

                bool isNew() const;
                void setIsNew(bool newIsNew);

                bool isSelfSave() const;
                void setIsSelfSave(bool newIsSelfSave);

            private:
                void loadPersonCode();
                void loadEdu();
                void loadSaints();
                void loadSpecialist();
                void loadEthnic();
                void loadCourse();
                void loadCountry();
#ifndef SKIP_PERSON_PROVINE
                void loadProvince();
#endif
                void loadWork();
                void loadCommunity();
                void loadStatus();

                void loadEvent(bool reloadAll = false);
                void cleanEvent();
                void searchPerson(QLineEdit* wget);
private slots:
    void on_btnImport_clicked();
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

    void on_btnAddEvent_clicked();

    void on_btnDelEvent_clicked();

    void on_btnSearchJoinPIC_clicked();

    void on_btnSearchPreTrainJoinPIC_clicked();

    void on_btnSearchTrainPIC_clicked();

    void on_btnSearchVowsCEO_clicked();

    void on_btnSearchEternalVowsPIC_clicked();

    void on_btnSearchEternalVowsCEO_clicked();


private:
    Person* mPerson;
    UIMultiComboxView *cbSaints;
    UIMultiComboxView *cbSpecialist;
    DlgPerson::Mode mEditMode;

    QList<PersonEvent*> mListPersonEvent;
    bool mIsNew; // true: add new, false: edit/update
    bool mIsSelfSave; //true: auto save on accept, false: not save
    bool mInitDone;
};

#endif // DLGPERSON_H
