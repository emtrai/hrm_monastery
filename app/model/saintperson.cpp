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
 * Filename: saintperson.cpp
 * Author: Anh, Ngo Huy
 * Created date:10/4/2022
 * Brief:
 */
#include "saintperson.h"
#include "logger.h"

#include "utils.h"
#include "dbctl.h"
#include "dbmodel.h"
#include "modeldefs.h"

SaintPerson::SaintPerson():DbModel()
{
    traced;
}

DbModel *SaintPerson::build()
{
    return new SaintPerson();
}

DbModelBuilder SaintPerson::getBuilder() const
{
    return &SaintPerson::build;
}

QString SaintPerson::buildUid(const QString *seed)
{
    traced;
    return Utils::UidFromName(personUid() + saintUid(), UidNameConvertType::HASH_NAME);
}

QString SaintPerson::modelName() const
{
    return KModelNameSaintPerson;
}

const QString &SaintPerson::personUid() const
{
    return mPersonUid;
}

void SaintPerson::setPersonUid(const QString &newPersonUid)
{
    mPersonUid = newPersonUid;
}

qint64 SaintPerson::personDbId() const
{
    return mPersonDbId;
}

void SaintPerson::setPersonDbId(qint64 newPersonDbId)
{
    mPersonDbId = newPersonDbId;
}

DbModelHandler *SaintPerson::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlPerson);
}

void SaintPerson::_copyData(const DbModel& model)
{
    tracein;
    const SaintPerson* per = static_cast<const SaintPerson*>(&model);
    setPersonUid(per->personUid());
    setPersonDbId(per->personDbId());
    setSaintUid(per->saintUid());
    setSaintDbId(per->saintDbId());
    traceout;
}

qint64 SaintPerson::saintDbId() const
{
    return mSaintDbId;
}

void SaintPerson::setSaintDbId(qint64 newSaintDbId)
{
    mSaintDbId = newSaintDbId;
}

QString SaintPerson::toString() const
{
    QString str = DbModel::toString();
    str += QString(":saintUid(%1):personUid('%2')")
               .arg(saintUid())
               .arg(personUid());
    return str;
}

const QString &SaintPerson::saintUid() const
{
    return mSaintUid;
}

void SaintPerson::setSaintUid(const QString &newSaintUid)
{
    mSaintUid = newSaintUid;
}
