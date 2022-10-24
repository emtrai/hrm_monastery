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
 * Filename: deptmgr.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef DEPTMGR_H
#define DEPTMGR_H

#include "mapdbmodel.h"

// Department managers
class DeptMgr : public MapDbModel
{
public:
    DeptMgr();
    static DbModel* build();
    virtual QString modelName() const;
    const QString &roleUid() const;
    void setRoleUid(const QString &newRoleUid);

    const QString &courseUid() const;
    void setCourseUid(const QString &newCourseUid);

protected:
    virtual DbModelHandler *getDbModelHandler();
protected:
    QString mRoleUid;
    QString mCourseUid; // Term
};

#endif // DEPTMGR_H
