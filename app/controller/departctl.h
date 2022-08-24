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
 * Filename: departctl.h
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#ifndef DEPARTCTL_H
#define DEPARTCTL_H

#include "controller.h"
#include "dbmodel.h"
#include "department.h"
#include <QHash>
#include <QList>

#define AREA (AreaCtl::getInstance())

class DepartCtl : public Controller
{
public:
    DepartCtl();
protected:
    DbModel *buildModel(void *items, const QString &fmt);
public:
    static DepartCtl* getInstance();
    const QList<Department*>* getDeptList();


public slots:
    virtual void onLoad();
private:
    static DepartCtl* gInstance;
    QList<Department*> mDeptList;
};

#endif // DEPARTCTL_H
