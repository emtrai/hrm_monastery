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
 * Filename: CourseCtl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#include "coursectl.h"

#include "logger.h"
#include "course.h"
#include "dbctl.h"
#include "utils.h"
#include "defs.h"

GET_INSTANCE_IMPL(CourseCtl)

CourseCtl::CourseCtl():CommonCtl(KModelHdlCourse)
{
    traced;
}

// Format: Name id, Name,period,startdate,enddate,remark
DbModel *CourseCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Course* item = new Course();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setNameId(itemList->at(idx++));
    item->setName(itemList->at(idx++));
    item->setPeriod(itemList->at(idx++));

    QString startDate = itemList->at(idx++).trimmed();
    if (!startDate.isEmpty())
        item->setStartDate(Utils::dateFromString(startDate));

    QString endDate = itemList->at(idx++).trimmed();
    if (!endDate.isEmpty())
        item->setEndDate(Utils::dateFromString(endDate));

    // TODO: validate if toInt ok or not ok
    qint32 courseType = itemList->at(idx++).trimmed().toInt();
    item->setCourseType(courseType);

    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}

const char *CourseCtl::getPrebuiltFileName()
{
    return KPrebuiltCourseCSVFileName;
}

const char *CourseCtl::getPrebuiltFileType()
{
    return KFileTypeCSV;
}

DbModelBuilder CourseCtl::getMainBuilder()
{
    return &Course::build;
}
