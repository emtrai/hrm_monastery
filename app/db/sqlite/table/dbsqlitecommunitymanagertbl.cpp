/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: dbsqlitecommunitymanagertbl.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/15/2023
 * Brief:
 */
#include "dbsqlitecommunitymanagertbl.h"

#include "dbsqlite.h"

#include "dbsqlitedefs.h"
#include "dbsqliteupdatebuilder.h"
#include "defs.h"
#include "logger.h"
#include "dbsqlitetablebuilder.h"
#include "dbsqliteinsertbuilder.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QHash>
#include "table/dbsqlitepersontbl.h"
#include "person.h"
#include "personctl.h"
#include "communitymanager.h"
#include "communityctl.h"
#include "community.h"
#include "rolectl.h"
#include "course.h"
#include "role.h"
#include "coursectl.h"
#include "utils.h"
#include "dbmodelutils.h"
#include "personctl.h"
#include "modeldefs.h"

#define VERSION_CODE_1 VERSION_CODE(0,0,1)
const qint32 DbSqliteCommunityManagerTbl::KVersionCode = VERSION_CODE_1;

DbSqliteCommunityManagerTbl::DbSqliteCommunityManagerTbl(DbSqlite *db):
    DbSqliteMapTbl(db, KTableCommManager, KTableCommManager, KVersionCode,
                     KModelNameCommManager)
{
    tracein;

    mFieldNameUid1 = KFieldCommunityUid;
    mFieldNameDbId1 = KFieldCommunityDbId;
    mFieldNameUid2 = KFieldPersonUid;
    mFieldNameDbId2 = KFieldPersonDbId;
    traceout;
}

QList<DbModel *> DbSqliteCommunityManagerTbl::getListPerson(const QString &communityUid,
                                                            int status)
{
    tracein;
    logi("getListPerson communityUid '%s', model status 0x%x", STR2CHA(communityUid), status);
    /*
     * SELECT * FROM "KTableCommManager"
     *  JOIN "KTablePerson"
     *  ON "KTableCommManager"."KFieldPersonUid" = "KTablePerson"."KFieldUid"
     *  WHERE "KTableCommManager"."KFieldCommunityUid" = :uid
     */
    QList<DbModel *> list = DbSqliteMapTbl::getListItemsWithUid(name(),
                                                         KTablePerson,
                                                         KFieldPersonUid,
                                                         KFieldUid,
                                                         KFieldCommunityUid,
                                                         &CommunityManager::build,
                                                         communityUid,
                                                         status,
                                                         QString("*, %1.%2 AS %3, %1.%4 AS %5, (%6 || ' ' || %7) AS %8")
                                                             .arg(KTablePerson,
                                                                KFieldNameId,
                                                                KFieldPersonNameId,
                                                                KFieldUid,
                                                                KFieldPersonUid,
                                                                KFieldLastName,
                                                                KFieldFirstName,
                                                                KFieldFullName)
                                                         );

    traceout;
    return list;
}

QList<DbModel *> DbSqliteCommunityManagerTbl::getListPerson(const QString &communityUid,
                                                            const QString &roleUid,
                                                            int status)
{
    tracein;
    QString cond;
    logi("getListPerson communityUid '%s', roleUid '%s', model status 0x%x",
         STR2CHA(communityUid), STR2CHA(roleUid), status);
    if (!communityUid.isEmpty()) {
        cond = QString("%1.%2 = '%3'").arg(name(), KFieldCommunityUid, communityUid);
    } else {
        cond = NULL_FIELD(name(), KFieldCommunityUid);
    }
    if (!roleUid.isEmpty()) {
        if (!cond.isEmpty()) {
            cond += " AND ";
        }
        cond += QString("%1.%2 = '%3'").arg(name(), KFieldRoleUid, roleUid);
    }
    dbgd("cond '%s'", STR2CHA(cond));
    /*
     * SELECT * FROM "KTableCommManager"
     *  JOIN "KTablePerson"
     *  ON "KTableCommManager"."KFieldPersonUid" = "KTablePerson"."KFieldUid"
     *  WHERE "KTableCommManager"."KFieldCommunityUid" = :uid
     */
    QList<DbModel *> list = DbSqliteMapTbl::getListItemsWithCond(name(),
                                                                 KTablePerson,
                                                                 KFieldPersonUid,
                                                                 KFieldUid,
                                                                 &CommunityManager::build,
                                                                 cond,
                                                                 status,
                                                                 QString("*, %1.%2 AS %3, %1.%4 AS %5, (%6 || ' ' || %7) AS %8")
                                                                     .arg(KTablePerson,
                                                                          KFieldNameId,
                                                                          KFieldPersonNameId,
                                                                          KFieldUid,
                                                                          KFieldPersonUid,
                                                                          KFieldLastName,
                                                                          KFieldFirstName,
                                                                          KFieldFullName)
                                                                 );

    traceout;
    return list;

}

QList<DbModel *> DbSqliteCommunityManagerTbl::getListPersonAll(int status)
{
    tracein;
    logi("getListPersonAll model status 0x%x", status);
    /*
     * SELECT * FROM "KTableCommManager"
     *  JOIN "KTablePerson"
     *  ON "KTableCommManager"."KFieldPersonUid" = "KTablePerson"."KFieldUid"
     *  WHERE "KTableCommManager"."KFieldCommunityUid" = :uid
     */
    QList<DbModel *> list = DbSqliteMapTbl::getListItemsWithCond(name(),
                                                                KTablePerson,
                                                                KFieldPersonUid,
                                                                KFieldUid,
                                                                &CommunityManager::build,
                                                                "",
                                                                status,
                                                                QString("*, %1.%2 AS %3, %1.%4 AS %5, (%6 || ' ' || %7) AS %8")
                                                                    .arg(KTablePerson,
                                                                        KFieldNameId,
                                                                        KFieldPersonNameId,
                                                                        KFieldUid,
                                                                        KFieldPersonUid,
                                                                        KFieldLastName,
                                                                        KFieldFirstName,
                                                                        KFieldFullName)
                                                                );

    traceout;
    return list;

}

ErrCode DbSqliteCommunityManagerTbl::insertTableField(DbSqliteInsertBuilder *builder,
                                                      const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item || !builder) {
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        err = DbSqliteMapTbl::insertTableField(builder, item); // TODO: handle error code
    }
    if (err == ErrNone && !IS_MODEL_NAME(item, KModelNameCommManager)) {
        err = ErrInvalidModel;
        loge("invalid model '%s'", MODELSTR2CHA(item));
    }

    if (err == ErrNone) {
        CommunityManager* model = (CommunityManager*) item;

        builder->addValue(KFieldRoleUid, model->roleUid());
        builder->addValue(KFieldCourseUid, model->courseUid());

    }
    traceret(err);
    return err;
}

void DbSqliteCommunityManagerTbl::addTableField(DbSqliteTableBuilder *builder)
{
    tracein;
    DbSqliteMapTbl::addTableField(builder);
    builder->addField(KFieldRoleUid, TEXT);
    builder->addField(KFieldCourseUid, TEXT);

    traceout;
}

ErrCode DbSqliteCommunityManagerTbl::updateDbModelDataFromQuery(DbModel *item, const QSqlQuery &qry)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        err = DbSqliteMapTbl::updateDbModelDataFromQuery(item, qry);
    }
    if (err == ErrNone) {
        QString modelName = item->modelName();
        if (modelName == KModelNameCommManager) {
            CommunityManager* commmgr = (CommunityManager*)item;
            DbSqlitePersonTbl* tblPerson =
                dynamic_cast<DbSqlitePersonTbl*>(DbSqlite::table(KTablePerson));

            if (!qry.isNull(KFieldRoleUid)) {
                QString roleUid = qry.value(KFieldRoleUid).toString();
                if (!roleUid.isEmpty()){
                    logd("search role uid '%s'", STR2CHA(roleUid));
                    DbModel* model = ROLECTL->getModelByUid(roleUid);
                    if (model) {
                        commmgr->setRole(static_cast<Role*>(model));
                        FREE_PTR(model);
                    } else {
                        logw("not found role uid '%s'", STR2CHA(roleUid));
                    }
                } else {
                    logw("Role uid empty");
                }
            } else {
                dbg(LOG_DEBUG, "no roleuid in table '%s'", STR2CHA(name()));
            }

            if (!qry.isNull(KFieldCourseUid)) {
                QString courseUid = qry.value(KFieldCourseUid).toString();
                if (!courseUid.isEmpty()){
                    logd("search courseUid '%s'", STR2CHA(courseUid));
                    DbModel* model = COURSECTL->getModelByUid(courseUid);
                    if (model) {
                        commmgr->setCourse(static_cast<Course*>(model));
                        FREE_PTR(model);
                    } else {
                        logw("not found courseUid '%s'", STR2CHA(courseUid));
                    }
                } else {
                    logw("courseUid empty");
                }
            } else {
                dbg(LOG_DEBUG, "no courseUid in table '%s'", STR2CHA(name()));
            }

            if (!qry.isNull(KFieldCommunityUid)) {
                QString commUid = qry.value(KFieldCommunityUid).toString();
                if (!commUid.isEmpty()){
                    logd("search commUid '%s'", STR2CHA(commUid));
                    DbModel* model = COMMUNITYCTL->getModelByUid(commUid);
                    if (model) {
                        commmgr->setCommunity(static_cast<Community*>(model));
                        FREE_PTR(model);
                    } else {
                        logw("not found commUid '%s'", STR2CHA(commUid));
                    }
                } else {
                    logw("commUid empty");
                }
            } else {
                dbg(LOG_DEBUG, "no commUid in table '%s'", STR2CHA(name()));
            }

            if (!qry.isNull(KFieldPersonUid)) {
                QString personUid = qry.value(KFieldPersonUid).toString();
                if (!personUid.isEmpty()){
                    logd("search personUid '%s'", STR2CHA(personUid));
                    DbModel* model = PERSONCTL->getModelByUid(personUid);
                    if (model) {
                        commmgr->setPerson(static_cast<Person*>(model));
                    } else {
                        logw("not found personUid '%s'", STR2CHA(personUid));
                    }
                } else {
                    logw("personUid empty");
                }
            } else {
                dbg(LOG_DEBUG, "no personUid in table '%s'", STR2CHA(name()));
            }
        } else {
            loge("Invalid mapp model '%s', do nothing", modelName.toStdString().c_str());
            err = ErrInvalidData;
        }
    }
    traceout;
    return err;
}

ErrCode DbSqliteCommunityManagerTbl::updateBuilderFieldFromModel(DbSqliteUpdateBuilder *builder,
                                                        const QString &field,
                                                        const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    logd("update table field '%s' for model '%s'", STR2CHA(field), MODELSTR2CHA(item));
    if (!builder || !item || field.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    if (err == ErrNone) {
        if (item->modelName() == KModelNameCommManager) {
            CommunityManager* comm = (CommunityManager*) item;
            if (field == KItemRole) {
                err = DbModelUtils::updateField(builder,
                                                KFieldRoleUid,
                                                KModelHdlRole,
                                                comm->roleUid());
            } else if (field == KItemCourse) {
                err = DbModelUtils::updateField(builder,
                                                KFieldCourseUid,
                                                KModelHdlCourse,
                                                comm->courseUid());
            } else if (field == KItemPerson) {

                err = DbModelUtils::updateField(builder,
                                                KFieldPersonUid,
                                                KModelHdlPerson,
                                                comm->personUid());
            } else if (field == KItemCommunity) {

                err = DbModelUtils::updateField(builder,
                                                KFieldCommunityUid,
                                                KModelHdlCommunity,
                                                comm->communityUid());
            } else {
                err = DbSqliteMapTbl::updateBuilderFieldFromModel(builder, field, item);
            }
            if (err == ErrNotFound) {
                logw("not found model for field '%s', skip to update, model '%s'",
                     STR2CHA(field), MODELSTR2CHA(item));
                err = ErrNone;
            }
        } else {
            loge("Model '%s' is no support",
                 MODELSTR2CHA(item));
            err = ErrNotSupport;
        }
    }
    traceret(err);
    return err;
}

QString DbSqliteCommunityManagerTbl::getSearchQueryStringWithTag(const QString &cond, const QString &tag)
{
    tracein;
    QString queryStr = getListItemsQueryString(name(),
                                               KTablePerson,
                                               KFieldPersonUid,
                                               KFieldUid,
                                               cond,
                                               MODEL_STATUS_MAX, // TODO: status?
                                               QString("*, %1.%2 AS %3, %1.%4 AS %5, (%6 || ' ' || %7) AS %8")
                                                   .arg(KTablePerson,
                                                       KFieldNameId,
                                                       KFieldPersonNameId,
                                                       KFieldUid,
                                                       KFieldPersonUid,
                                                       KFieldLastName,
                                                       KFieldFirstName,
                                                       KFieldFullName)
                                               );
    traceout;
    return queryStr;
}
