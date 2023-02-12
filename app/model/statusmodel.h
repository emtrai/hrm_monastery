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
 * Filename: statusmodel.h
 * Author: Anh, Ngo Huy
 * Created date:2/4/2023
 * Brief:
 */
#ifndef STATUSMODEL_H
#define STATUSMODEL_H
#include <QHash>
#include <QString>


// COMMON STATUS OF MODELS

// BE WARE, ANY CHANGE TO THIS STATUS WILL IMPACT TO DB
// THIS VALUE IS WRITTEN DIRECTLY TO DB
// TODO: to do what to improve this???
enum StatusModel {
    STATUS_NONE = 0, // unknown
    STATUS_INACTIVE = (1 << 0), // inactive, not operating
    STATUS_ACTIVE = (1 << 1), // active, operating
    STATUS_BUILDING = (1 << 2), // building phase, not ready


    STATUS_MAX
};

class StatusModelCtl
{
public:
    static QHash<int, QString>* getStatusIdNameMap();
    static QString status2Name(StatusModel status);
};

#endif // STATUSMODEL_H
