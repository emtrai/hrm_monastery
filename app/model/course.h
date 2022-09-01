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

class Course : public DbModel
{
public:
    Course();
    virtual ~Course();
    static DbModel *builder();
    qint64 startDate() const;
    void setStartDate(qint64 newStartDate);

    qint64 endDate() const;
    void setEndDate(qint64 newEndDate);

    const QString &period() const;
    void setPeriod(const QString &newPeriod);

    const QString &remark() const;
    void setRemark(const QString &newRemark);

protected:
    virtual DbModelHandler *getDbModelHandler();
private:
    qint64 mStartDate;
    qint64 mEndDate;
    QString mPeriod;
    QString mRemark;
};

#endif // COURSE_H
