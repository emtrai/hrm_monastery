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
 * Filename: specialistctl.h
 * Author: Anh, Ngo Huy
 * Created date:8/9/2022
 * Brief:
 */
#ifndef SPECIALISTCTL_H
#define SPECIALISTCTL_H

#include <modelcontroller.h>
#include "errcode.h"
#include <QStringList>
#include "specialist.h"
#include "modelcontroller.h"

#define SPECIALISTCTL SpecialistCtl::getInstance()
class SpecialistCtl : public ModelController
{
    GET_INSTANCE_DECL(SpecialistCtl);
private:
    SpecialistCtl();
    virtual ~SpecialistCtl();
protected:
    virtual DbModelBuilder getMainBuilder();
    virtual const char* getPrebuiltFileName();
public:
    virtual QList<DbModel*> getListPerson(const QString& specialistUid);

};

#endif // SPECIALISTCTL_H
