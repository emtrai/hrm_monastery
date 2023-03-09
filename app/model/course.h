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
 * Filename: course.h
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#ifndef COURSE_H
#define COURSE_H

#include <dbmodel.h>
typedef enum CourseType {
    COURSE_TYPE_COURSE = 0, // khoa hoc
    COURSE_TYPE_TERM, // nhiem ki
    COURSE_TYPE_OTHERS, // Khác
    COURSE_TYPE_MAX
};

class Course : public DbModel
{
public:
    static const QHash<int, QString>* getCourseTypeNameMap();
    static QString courseType2Name(DbModelStatus type);
protected:
    Course();
public:
    virtual ~Course();
    static DbModel *build();

    virtual DbModelBuilder getBuilder();
    qint64 startDate() const;
    void setStartDate(qint64 newStartDate);

    qint64 endDate() const;
    void setEndDate(qint64 newEndDate);

    const QString &period() const;
    void setPeriod(const QString &newPeriod);

    qint32 courseType() const;
    void setCourseType(qint32 newCourseType);

protected:
    virtual DbModelHandler *getDbModelHandler();
private:
    qint64 mStartDate;
    qint64 mEndDate;
    QString mPeriod;
    qint32 mCourseType;
};

#endif // COURSE_H
