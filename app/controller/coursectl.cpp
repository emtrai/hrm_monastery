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

CourseCtl::CourseCtl()
{
    traced;
}

// Format: Name,period,startdate,enddate,remark
DbModel *CourseCtl::buildModel(void *items, const QString &fmt)
{
    traced;
    Course* item = new Course();
    QStringList* itemList = (QStringList*) items;
    qint32 idx = 0;
    qint32 sz = itemList->length();
    logd("sz %d", sz);
    item->setName(itemList->at(idx++));
    item->setNameId(item->name());
    item->setPeriod(itemList->at(idx++));
    QString startDate = itemList->at(idx++).trimmed();
    if (!startDate.isEmpty())
        item->setStartDate(Utils::dateFromString(startDate));
    QString endDate = itemList->at(idx++).trimmed();
    if (!endDate.isEmpty())
        item->setEndDate(Utils::dateFromString(endDate));

    if (sz > idx) {
        QString remark = itemList->at(idx++);
        if (!remark.isEmpty())
            item->setRemark(remark);
    }
    tracede;
    return item;
}


const QList<Course *> CourseCtl::getCourseList()
{
    traced;

    return mCourseList;
}

ErrCode CourseCtl::reloadDb()
{
    traced;
    mCourseList.clear();
    // TODO: loop to delete each element????

    QList items = DB->getModelHandler(KModelHdlCourse)->getAll(&Course::builder);
    //    mItemList.append();
    foreach (DbModel* model, items){
        Course* item = (Course*)model;
        mCourseList.append(item);
    }
    return ErrNone;
}

void CourseCtl::onLoad()
{
    traced;
    ErrCode ret = ErrNone;
    ret = check2UpdateDbFromPrebuiltFile(KPrebuiltCourseCSVFileName, KFileTypeCSV);
    // TODO: should do lazy load??? load all consume much memory
    reloadDb();
    tracede;
}

