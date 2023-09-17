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
 * Filename: communityperson.h
 * Author: Anh, Ngo Huy
 * Created date:10/16/2022
 * Brief:
 */
#ifndef COMMUNITYPERSON_H
#define COMMUNITYPERSON_H

#include "mapdbmodel.h"
class Person;
class Community;
class CommunityPerson : public MapDbModel
{
protected:
    CommunityPerson();
public:
    virtual ~CommunityPerson();
    static DbModel* build();
    virtual DbModelBuilder getBuilder() const;
    virtual QString modelName() const;
    virtual void clone(const DbModel* model);
    virtual QString exportHtmlTemplateFile(const QString& name) const;
    virtual void initExportFields();

    const QString &communityUid() const { return uid1(); }
    void setCommunityUid(const QString &newUid1);

    const QString &personUid() const { return uid2(); }
    void setPersonUid(const QString &newUid1);

    Person *person() const;
    void setPerson(const Person *newPerson);

    Community *community() const;
    void setCommunity(const Community *newCommunity);

    virtual QString toString() const;
protected:
    virtual DbModelHandler *getDbModelHandler() const;
protected:
    Person* mPerson;
    Community* mCommunity;
};

#endif // COMMUNITYPERSON_H
