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
 * Filename: provincectl.h
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#ifndef PROVINCECTL_H
#define PROVINCECTL_H

#include "modelcontroller.h"
#include "dbmodel.h"
#include "province.h"
#include <QHash>
#include <QList>
#include "utils.h"

#define PROVINCE (ProvinceCtl::getInstance())

class ProvinceCtl : public ModelController
{
    GET_INSTANCE_DECL(ProvinceCtl);
private:
    ProvinceCtl();
public:
    const QList<Province *> *getProvinceList(const QString &country);
protected:
    DbModel *buildModel(void *items, const QString &fmt);
    // prebuilt data file name
    virtual const char* getPrebuiltFileName();

    virtual DbModelBuilder getMainBuilder();
};

#endif // PROVINCECTL_H
