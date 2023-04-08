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
 * Filename: dlgsearchperson.h
 * Author: Anh, Ngo Huy
 * Created date:12/9/2022
 * Brief:
 */
#ifndef DLGSEARCHPERSON_H
#define DLGSEARCHPERSON_H

#include "dlgsearch.h"

class Person;

class DlgSearchPerson : public DlgSearch
{
public:
    ~DlgSearchPerson();
    static DlgSearchPerson* build(QWidget *parent = nullptr, bool isMulti = false);
protected:
    DlgSearchPerson(QWidget *parent = nullptr, bool isMulti = false);

protected:

    virtual QString getTitle();
    virtual void initHeader();

    virtual int onSearch(const QString& keyword);
    virtual void clearAll();
    virtual int onGetAll();
    virtual DbModel* getItemAtIdx(int idx);
    virtual QString getValueOfItemAt(int idx, int col, QString header, DbModel* item);
private:
    QList<DbModel*> mListItems;

};

#endif // DLGSEARCHPERSON_H
