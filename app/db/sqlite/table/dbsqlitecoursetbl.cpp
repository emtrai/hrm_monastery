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
 * Filename: dbsqlitecoursetbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#include "dbsqlitecoursetbl.h"
#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "course.h"
#include "dbsqliteinsertbuilder.h"
#include "dbsqliteupdatebuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "utils.h"

const qint32 DbSqliteCourseTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCourseTbl::DbSqliteCourseTbl(DbSqlite* db)
    : DbSqliteTbl(db, KTableCourse, KTableCourse, KVersionCode,
                  KModelNameCourse)
{
    traced;
}

DbSqliteCourseTbl::~DbSqliteCourseTbl()
{
    traced;
}

void DbSqliteCourseTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldPeriod, TEXT);
    builder->addField(KFieldStartDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldCourseType, INT64);
    traceout;
}

ErrCode DbSqliteCourseTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    dbg(LOG_DEBUG, "insertTableField model '%s'", MODELSTR2CHA(item));
    if (err == ErrNone) {
        err = DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    }
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameCourse)) {
        loge("not supported model '%s', expect '%s'",
             MODELSTR2CHA(item), KModelNameCourse);
        err = ErrNotSupport;
    }

    if (err == ErrNone) {
        Course* course = (Course*) item;
        builder->addValue(KFieldPeriod, course->name());
        builder->addValue(KFieldStartDate, course->startDate());
        builder->addValue(KFieldEndDate, course->endDate());
        builder->addValue(KFieldCourseType, course->courseType());
    }
    traceout;
    return err;
}

ErrCode DbSqliteCourseTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    dbg(LOG_DEBUG, "updateDbModelDataFromQuery model '%s'", MODELSTR2CHA(item));
    ErrCode err = ErrNone;
    err = DbSqliteTbl::updateDbModelDataFromQuery(item, qry);
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameCourse)) {
        loge("not supported model '%s', expect '%s'",
             MODELSTR2CHA(item), KModelNameCourse);
        err = ErrNotSupport;
    }

    if (err == ErrNone) {
        Course* course = (Course*) item;
        course->setStartDate(qry.value(KFieldStartDate).toInt());
        course->setEndDate(qry.value(KFieldEndDate).toInt());
        course->setPeriod(qry.value(KFieldPeriod).toString());
        course->setCourseType(qry.value(KFieldCourseType).toInt());
    }
    traceout;
    return err;
}

ErrCode DbSqliteCourseTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder, const QString &field, const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        if (item->modelName() == KModelNameCourse) {
            Course* comm = (Course*) item;
            if (field == KItemStartDate) {
                builder->addValue(KFieldStartDate, comm->startDate());

            } else if (field == KItemEndDate) {
                builder->addValue(KFieldEndDate, comm->endDate());

            } else if (field == KItemPeriod) {
                builder->addValue(KFieldPeriod, comm->period());

            } else if (field == KItemCourseType) {
                builder->addValue(KFieldCourseType, comm->courseType());

            } else {
                err = DbSqliteTbl::updateBuilderFieldFromModel(builder, field, item);
            }
        } else {
            loge("Model '%s' is no support",
                 MODELSTR2CHA(item));
            err = ErrNotSupport;
        }
    }
    traceout;
    return err;
}
