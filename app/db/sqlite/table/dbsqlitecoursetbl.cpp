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
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>

const qint32 DbSqliteCourseTbl::KVersionCode = VERSION_CODE(0,0,1);

DbSqliteCourseTbl::DbSqliteCourseTbl(DbSqlite* db)
    : DbSqliteTbl(db, KTableCourse, KTableCourse, KVersionCode)
{

}

void DbSqliteCourseTbl::addTableField(DbSqliteTableBuilder *builder)
{
    traced;
    DbSqliteTbl::addTableField(builder);
    builder->addField(KFieldPeriod, TEXT);
    builder->addField(KFieldStartDate, INT64);
    builder->addField(KFieldEndDate, INT64);
    builder->addField(KFieldCourseType, INT64);
    builder->addField(KFieldRemark, TEXT);
}

ErrCode DbSqliteCourseTbl::insertTableField(DbSqliteInsertBuilder *builder, const DbModel *item)
{
    traced;
    DbSqliteTbl::insertTableField(builder, item); // TODO: handle error code
    Course* course = (Course*) item;
    builder->addValue(KFieldPeriod, course->name());
    builder->addValue(KFieldStartDate, course->startDate());
    builder->addValue(KFieldEndDate, course->endDate());
    builder->addValue(KFieldCourseType, course->courseType());
    builder->addValue(KFieldRemark, course->remark());
    tracede;
    return ErrNone;
}

void DbSqliteCourseTbl::updateModelFromQuery(DbModel *item, const QSqlQuery &qry)
{
    traced;
    DbSqliteTbl::updateModelFromQuery(item, qry);
    Course* course = (Course*) item;
    course->setStartDate(qry.value(KFieldStartDate).toInt());
    course->setEndDate(qry.value(KFieldEndDate).toInt());
    course->setPeriod(qry.value(KFieldPeriod).toString());
    course->setCourseType(qry.value(KFieldCourseType).toInt());
    course->setRemark(qry.value(KFieldRemark).toString());
}
