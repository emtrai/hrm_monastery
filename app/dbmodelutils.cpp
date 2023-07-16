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
 * Filename: dbmodelutils.cpp
 * Author: Anh, Ngo Huy
 * Created date:6/29/2023
 * Brief:
 */
#include "dbmodelutils.h"
#include "logger.h"
#include "dbctl.h"
#include "dbmodelhandler.h"
#include "dbmodel.h"
#include "utils.h"
#include "dbsqliteupdatebuilder.h"


DbModel* DbModelUtils::getDbModel(const QString& modelHdlName,
                                  const QString& uid,
                                  const QString& nameId,
                                  const QString& name,
                                  bool* ok)
{
    tracein;
    ErrCode err = ErrNone;
    DbModelHandler* hdl = nullptr;
    DbModel* model = nullptr;
    hdl = DB->getModelHandler(modelHdlName);
    if (!hdl) {
        err = ErrInvalidData;
        loge("not found model handler '%s'", STR2CHA(modelHdlName));
    }
    logd("name id to check %s", STR2CHA(nameId));
    if (err == ErrNone && !model && !uid.isEmpty()) {
        logd("get model by uid '%s'", STR2CHA(uid));
        model = hdl->getByUid(uid);
        if (!model) {
            err = ErrNotFound;
            loge("not found uid '%s'", STR2CHA(uid));
        }
    }
    if (err == ErrNone && !model && !nameId.isEmpty()) {
        logd("get model by nameId '%s'", STR2CHA(nameId));
        model = hdl->getByNameId(nameId);
        if (!model) {
            err = ErrNotFound;
            loge("not found nameid '%s'", STR2CHA(nameId));
        }
    }
    if (err == ErrNone && !model && !name.isEmpty()) {
        logd("get model by name '%s'", STR2CHA(name));
        model = hdl->getByName(name);
        if (!model) {
            err = ErrNotFound;
            loge("not found name '%s'", STR2CHA(name));
        }
    }
    if (ok) {
        *ok = (err == ErrNone);
    }
    // TODO: check name?
    traceout;
    return model;
}

ErrCode DbModelUtils::updateField(DbSqliteUpdateBuilder *builder,
                                  const QString& uidFieldname,
                                  const QString& hdlName,
                                  const QString& uid,
                                  const QString& nameid,
                                  const QString& name)
{
    tracein;
    ErrCode err = ErrNone;
    DbModel* model = DbModelUtils::getDbModel(
        hdlName, uid, nameid, name);
    if (model && !(model->uid().isEmpty())) {
        builder->addValue(uidFieldname, model->uid());
    } else {
        err = ErrNotFound;
        loge("not found");
    }
    FREE_PTR(model);
    traceret(err);
    return err;
}
