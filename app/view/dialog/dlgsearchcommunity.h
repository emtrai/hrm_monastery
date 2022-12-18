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
 * Filename: dlgsearchcommunity.h
 * Author: Anh, Ngo Huy
 * Created date:12/12/2022
 * Brief:
 */
#ifndef DLGSEARCHCOMMUNITY_H
#define DLGSEARCHCOMMUNITY_H

#include "dlgsearch.h"

class DbModel;

class DlgSearchCommunity : public DlgSearch
{
public:
    ~DlgSearchCommunity();
    static DlgSearchCommunity* build(QWidget *parent = nullptr, bool isMulti = false);
protected:
    DlgSearchCommunity(QWidget *parent = nullptr, bool isMulti = false);

protected:

    virtual QString getTitle();

    virtual int onSearch(const QString& keyword);
    virtual int onGetAll();
    virtual void clearAll();
    virtual DbModel* getItemAtIdx(int idx);
private:
    QList<DbModel*> mListItems;
};

#endif // DLGSEARCHCOMMUNITY_H
