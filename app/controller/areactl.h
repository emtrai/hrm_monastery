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
 * Filename: areactl.h
 * Author: Anh, Ngo Huy
 * Created date:8/19/2022
 * Brief:
 */
#ifndef AREACTL_H
#define AREACTL_H

#include "commonctl.h"
#include "utils.h"
#include <QList>
#include "dbmodel.h"

#define AREACTL (AreaCtl::getInstance())

class DbModel;

class AreaCtl : public CommonCtl
{
    GET_INSTANCE_DECL(AreaCtl);
private:
    AreaCtl();
protected:

    // build model from data
    virtual DbModel *buildModel(void *items, const QString &fmt);

    // prebuilt data file name
    virtual const char* getPrebuiltFileName();

    virtual DbModelBuilder getMainBuilder();

};

#endif // AREACTL_H
