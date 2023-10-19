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
 * Filename: communitymanager.h
 * Author: Anh, Ngo Huy
 * Created date:10/15/2023
 * Brief:
 */
#ifndef COMMUNITYMANAGER_H
#define COMMUNITYMANAGER_H

#include "mapdbmodel.h"

class Person;
class Community;
class Role;
class Course;
class CommunityManager : public MapDbModel
{
protected:
    CommunityManager();
public:
    virtual ~CommunityManager();
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
    QString personNameId() const;
    QString personName() const;
    QString personEmail() const;
    QString personTel() const;

    Community *community() const;
    void setCommunity(const Community *newCommunity);
    QString communityNameId() const;

    virtual QString toString() const;

    Course *course() const;
    void setCourse(Course *newCourse);
    QString courseUid() const;
    QString courseName() const;

    Role *role() const;
    void setRole(Role *newRole);
    QString roleUid() const;
    QString roleName() const;


protected:
    virtual DbModelHandler *getDbModelHandler() const;
protected:
    Person* mPerson;
    Community* mCommunity;
    Role* mRole;
    Course* mCourse;
};

#endif // COMMUNITYMANAGER_H
