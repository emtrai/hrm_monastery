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
 * Filename: mapdbmodel.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef MAPDBMODEL_H
#define MAPDBMODEL_H

#include <dbmodel.h>

#define SAVE_MAP_MODEL(MODEL, field1, field2, status, startdate, enddate, remark) \
do { \
        MODEL* model = new MODEL(); \
        model->setDbId1(field1->dbId()); \
        model->setUid1(field1->uid()); \
        model->setDbId2(field2->dbId()); \
        model->setUid2(field2->uid()); \
        model->setStatus(status); \
        model->setStartDate(startdate); \
        model->setEndDate(enddate); \
        if (!remark.isEmpty()) \
        model->setRemark(remark); \
        logd("Add to db"); \
        err = model->save(); \
        delete model; \
        model = nullptr; \
} while (0)

class MapDbModel : public DbModel
{
public:
    MapDbModel();

    virtual QString buildUid(const QString* seed = nullptr);

    const QString &uid1() const;
    void setUid1(const QString &newUid1);

    qint64 dbId1() const;
    void setDbId1(qint64 newDbId1);

    qint64 startDate() const;
    void setStartDate(qint64 newStartDate);

    const QString &uid2() const;
    void setUid2(const QString &newUid2);

    qint64 dbId2() const;
    void setDbId2(qint64 newDbId2);

    qint64 endDate() const;
    void setEndDate(qint64 newEndDate);

    qint32 status() const;
    void setStatus(qint32 newStatus);

    const QString &remark() const;
    void setRemark(const QString &newRemark);

    const QString &parentUid() const;
    void setParentUid(const QString &newParentUid);

    virtual QString modelName() const;
    virtual int modelType() const;

    const QString &changeHistory() const;
    void setChangeHistory(const QString &newChangeHistory);

protected:
    QString mUid1;
    qint64 mDbId1;
    QString mUid2;
    qint64 mDbId2;
    qint64 mStartDate;
    qint64 mEndDate;
    qint32 mStatus;
    QString mRemark;
    QString mParentUid;
    QString mChangeHistory;
};

#endif // MAPDBMODEL_H
