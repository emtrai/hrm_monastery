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
 * Filename: courectl.h
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#ifndef COURSECTL_H
#define COURSECTL_H

#include "controller.h"
#include "utils.h"
#include <QList>

class Course;

class CourseCtl : public Controller
{
    GET_INSTALCE_DECL(CourseCtl);
public:
    CourseCtl();
protected:
    DbModel *buildModel(void *items, const QString &fmt);
public:
    const QList<Course*> getCourseList();

public:
    virtual ErrCode reloadDb();

public slots:
    virtual void onLoad();
private:
    QList<Course*> mCourseList;
};

#endif // COURSECTL_H
