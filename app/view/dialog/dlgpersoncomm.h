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
 * Filename: DlgPersonCommunity.h
 * Author: Anh, Ngo Huy
 * Created date:9/15/2022
 * Brief:
 */
#ifndef DLGPERSONCOMM_H
#define DLGPERSONCOMM_H

#include <QDialog>
#include <QHash>
#include "dlgcommoneditmodel.h"
#include "logger.h"

class Community;
class Person;
class CommunityPerson;

namespace Ui {
class DlgPersonCommunity;
}

class DlgPersonCommunity : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgPersonCommunity)
public:
    explicit DlgPersonCommunity(QWidget *parent = nullptr);
    ~DlgPersonCommunity();
    virtual void setupUI();

    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    virtual ErrCode fromModel(const DbModel* model);

    const QList<CommunityPerson*>& getCommunityPersonList();
    void loadCommunity();
    void loadModelStatus(bool skipActive = true);

    template<class T>
    ErrCode setPersonList(const QList<T *>&newList)
    {
        tracein;
        ErrCode err = ErrNone;
        clearPersonList();
        foreach (T* item, newList) {
            err = appendPerson((Person*)item);
            if (err != ErrNone) break;
        }
        traceret(err);
        return err;
    }

    ErrCode setModelStatus(int modelStatus, bool allowChange = true);
    int getModelStatus(bool* ok = nullptr);

    void setCommunity(const Community *newCommunity);

    const QList<CommunityPerson*>& communityPersonList();
    ErrCode appendCommunityPerson(const CommunityPerson* commPer, bool ignoreCommunity = false);
    ErrCode setCommunityPersonList(const QList<CommunityPerson*>& commPer, bool ignoreCommunity = false);
    ErrCode appendPerson(const Person* person);

public:
    static ErrCode addListPeopleToCommunity(QWidget *parent,
                                            const Community* comm,
                                            const QList<Person*>& perList);
    static ErrCode updateCommunityPerson(QWidget *parent, const CommunityPerson* model);
    void setSkipStartDate(bool newSkipStartDate);
    void setEndDate(qint64 timeInMs = 0);
    void setStartDate(qint64 timeInMs = 0);

protected:
    virtual QDialogButtonBox* buttonBox();
    virtual DbModel* newModel();
    void clearPersonList();

    virtual void accept();
private:
    void updateModelStatus(int status);
    void setDate(QLineEdit * txt, qint64 timeInMs = 0);

private slots:
    void on_btnSearchPeople_clicked();

    void on_cbModelStatus_currentIndexChanged(int index);

private:
    Ui::DlgPersonCommunity *ui;
    QList<Person*> mPersonList;
    QList<CommunityPerson*> mCommunityPersonList;
    Community* mCommunity;
    bool mSkipStartDate;
};

#endif // DLGPERSONCOMM_H
