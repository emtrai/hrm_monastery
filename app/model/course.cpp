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
#include "datetimeutils.h"
#include "dbctl.h"
#include "defs.h"
#include "dbmodel.h"
#include "prebuiltdefs.h"
#include "modeldefs.h"

const QHash<int, QString> *Course::getCourseTypeNameMap()
{
    tracein;
    static bool isInited = false;
    static QHash<int, QString> map;
    if (!isInited) {
        map.insert(COURSE_TYPE_COURSE, QObject::tr("Lớp Khấn"));
        map.insert(COURSE_TYPE_TERM, QObject::tr("Nhiệm kỳ quản lý"));
        map.insert(COURSE_TYPE_OTHERS, QObject::tr("Khác"));
        isInited = true;
    }
    traceout;
    return &map;
}

QString Course::courseType2Name(CourseType type)
{
    const QHash<int, QString>* courseMap = getCourseTypeNameMap();
    QString ret;
    tracein;
    logd("type %d", type);
    if (courseMap->contains(type)){
        ret = courseMap->value(type);
    } else {
        loge("invalid type %d", type);
        ret = QObject::tr("Không rõ"); // TODO: translate???
    }
    traceout;
    return ret;
}

Course::Course():DbModel(),
    mStartDate(0),
    mEndDate(0),
    mCourseType(COURSE_TYPE_MAX)
{
    traced;
}

Course::~Course()
{
    tracein;
}

DbModel *Course::build()
{
    tracein;
    Course* model = new Course();
    model->init();
    return model;

}

void Course::_copyData(const DbModel& model)
{
    tracein;
    const Course* course = static_cast<const Course*>(&model);
    setCourseType(course->courseType());
    setStartDate(course->startDate());
    setEndDate(course->endDate());
    setPeriod(course->period());

    traceout;
}

QString Course::modelName() const
{
    return KModelNameCourse;
}

void Course::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    mExportCallbacks.insert(KItemPeriod, [](const DbModel* model, const QString& item){
        return ((Course*)model)->period();
    });
    mExportCallbacks.insert(KItemType, [](const DbModel* model, const QString& item){
        return ((Course*)model)->courseTypeName();
    });
    mExportCallbacks.insert(KItemStartDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Course*)model)->startDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemEndDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Course*)model)->endDate(), DEFAULT_FORMAT_YMD);
    });
    // TODO: implement more
    traceout;
}

ErrCode Course::exportTemplatePath(FileExporter *exporter,
                                   const QString& name,
                                   QString& fpath,
                                   QString *ftype) const
{
    tracein;
    ErrCode err = ErrNone;
    if (exporter) {
        ExportType type = exporter->getExportType();
        switch (type) {
        case EXPORT_HTML:
            fpath = FileCtl::getPrebuiltDataFilePath(KPrebuiltCourseTemplateFileName);
            break;
        default:
            loge("export type %d not support", type);
            break;
        };
    } else {
        loge("invalid exporter");
        err = ErrInvalidArg;
    }
    traceret(err);
    return err;
}

DbModelBuilder Course::getBuilder() const
{
    return &Course::build;
}

DbModelHandler *Course::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlCourse);
}

void Course::setCourseTypeName(const QString &newCourseTypeName)
{
    mCourseTypeName = newCourseTypeName;
}

QString Course::toString() const
{
    QString str = DbModel::toString();
    str += QString(":courseType('%1')").arg(courseType());
    str += QString(":courseTypeName('%1')").arg(courseTypeName());
    str += QString(":endDate('%1')").arg(endDate());
    str += QString(":startDate('%1')").arg(startDate());
    str += QString(":period('%1')").arg(period());
    return str;
}

qint32 Course::courseType() const
{
    return mCourseType;
}

QString Course::courseTypeName() const
{
    if (mCourseTypeName.isEmpty()) {
        return courseType2Name((CourseType)mCourseType);
    } else {
        return mCourseTypeName;
    }
}

void Course::setCourseType(qint32 newCourseType)
{
    CHECK_MODIFIED_THEN_SET(mCourseType, newCourseType, KItemCourseType);
}


const QString &Course::period() const
{
    return mPeriod;
}

void Course::setPeriod(const QString &newPeriod)
{
    CHECK_MODIFIED_THEN_SET(mPeriod, newPeriod, KItemPeriod);
}

qint64 Course::endDate() const
{
    return mEndDate;
}

void Course::setEndDate(qint64 newEndDate)
{
    CHECK_MODIFIED_THEN_SET(mEndDate, newEndDate, KItemEndDate);
}

qint64 Course::startDate() const
{
    return mStartDate;
}

void Course::setStartDate(qint64 newStartDate)
{
    CHECK_MODIFIED_THEN_SET(mStartDate, newStartDate, KItemStartDate);
}
