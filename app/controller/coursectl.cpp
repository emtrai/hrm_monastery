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
#include "prebuiltdefs.h"

GET_INSTANCE_CONTROLLER_IMPL(CourseCtl)

CourseCtl::CourseCtl():ModelController(KModelHdlCourse)
{
    tracein;
}

// Format: Name id, Name,course type, period,startdate,enddate,remark
DbModel *CourseCtl::buildModel(void *items, const QString &fmt)
{
    tracein;
    ErrCode err = ErrNone;
    Course* item = NULL;
    QStringList* itemList = NULL;
    qint32 idx = 0;
    qint32 noItem = 0;
    QString nameId;
    QString name;

    if (!items) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone && fmt != KDataFormatStringList) {
        err = ErrNotSupport;
        loge("invalid data format '%s", STR2CHA(fmt));
    }
    if (err == ErrNone) {
        itemList = (QStringList*) items;
        noItem = itemList->length();
        logd("noItem %d", noItem);
    }

    if (err == ErrNone && (noItem < 2)) { // require name id, name
        err = ErrShortData;
        loge("Not enouth field, %d < 3", noItem);
    }

    if (err == ErrNone) {
        DbModelBuilder builder = getMainBuilder();
        if (builder) {
            item = (Course*)builder();
            if (!item) {
                loge("No memory");
                err = ErrNoMemory;
            }
        } else {
            loge("No main builder");
            err = ErrNotAvail;
        }
    }

    logd("noItem %d", noItem);
    if (err == ErrNone) {
        nameId = itemList->at(idx++);
        name = itemList->at(idx++);
        logd("nameId '%s'", STR2CHA(nameId));
        logd("name '%s'", STR2CHA(name));
        if (!nameId.isEmpty() && !name.isEmpty()) {
            item->setName(name);
            item->setNameId(nameId);
        } else {
            err = ErrInvalidData;
            loge("data is invalid, nameid/countrynameid or name is empty");

        }
    }

    if (err == ErrNone) {
        bool ok = false;
        qint32 courseType = itemList->at(idx++).trimmed().toInt(&ok);
        if (ok && courseType < COURSE_TYPE_MAX) {
            item->setCourseType(courseType);
        } else {
            loge("Invalid course type = %d ok = %d", courseType, ok);
            err = ErrInvalidData;
        }
    }

    if (err == ErrNone) {
        item->setPeriod(itemList->at(idx++).trimmed());
    }

    if (err == ErrNone) {
        QString startDate = itemList->at(idx++).trimmed();
        if (!startDate.isEmpty())
            item->setStartDate(Utils::dateFromString(startDate));
    }

    if (err == ErrNone) {
        QString endDate = itemList->at(idx++).trimmed();
        if (!endDate.isEmpty())
            item->setEndDate(Utils::dateFromString(endDate));
    }

    if (err == ErrNone) {
        if (noItem > idx) {
            QString remark = itemList->mid(idx).join(DEFAULT_CSV_ITEM_SPLIT);
            if (!remark.isEmpty())
                item->setRemark(remark);
        }
    }

    if (err != ErrNone && item) {
        delete item;
        item = NULL;
    }
    traceout;
    return item;
}

const char *CourseCtl::getPrebuiltFileName()
{
    return KPrebuiltCourseCSVFileName;
}

DbModelBuilder CourseCtl::getMainBuilder()
{
    return &Course::build;
}
