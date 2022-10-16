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
 * Filename: dbsqlitedeptmgrtbl.h
 * Author: Anh, Ngo Huy
 * Created date:10/8/2022
 * Brief:
 */
#ifndef DBSQLITEDEPTMGRTBL_H
#define DBSQLITEDEPTMGRTBL_H

#include "dbsqlitemaptbl.h"

class DbSqliteDeptMgrTbl : public DbSqliteMapTbl
{
public:
    DbSqliteDeptMgrTbl(DbSqlite* db);
    DbSqliteDeptMgrTbl(DbSqlite* db, const QString& baseName, const QString& name, qint32 versionCode);


    virtual void addTableField(DbSqliteTableBuilder* builder);
    virtual void insertTableField(DbSqliteInsertBuilder* builder, const DbModel *item);
    virtual void updateModelFromQuery(DbModel* item, const QSqlQuery& qry);
    const QString &roleUid() const;
    void setRoleUid(const QString &newRoleUid);

    const QString &termUid() const;
    void setTermUid(const QString &newTermUid);

protected:
    QString mRoleUid;
    QString mTermUid;
private:
    static const qint32 KVersionCode;
};

#endif // DBSQLITEDEPTMGRTBL_H
