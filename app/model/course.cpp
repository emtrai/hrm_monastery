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
 * Filename: course.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#include "course.h"
#include "logger.h"
#include "errcode.h"
#include "province.h"
#include "filectl.h"
#include "utils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"

Course::Course():
    mStartDate(0),
    mEndDate(0)
{
    traced;
}

Course::~Course()
{
    traced;
}

DbModel *Course::build()
{
    traced;
    Course* model = new Course();
    model->init();
    return model;

}

DbModelHandler *Course::getDbModelHandler()
{
    return DB->getModelHandler(KModelHdlCourse);
}

qint32 Course::courseType() const
{
    return mCourseType;
}

void Course::setCourseType(qint32 newCourseType)
{
    mCourseType = newCourseType;
}


const QString &Course::period() const
{
    return mPeriod;
}

void Course::setPeriod(const QString &newPeriod)
{
    mPeriod = newPeriod;
}

qint64 Course::endDate() const
{
    return mEndDate;
}

void Course::setEndDate(qint64 newEndDate)
{
    mEndDate = newEndDate;
}

qint64 Course::startDate() const
{
    return mStartDate;
}

void Course::setStartDate(qint64 newStartDate)
{
    mStartDate = newStartDate;
}
