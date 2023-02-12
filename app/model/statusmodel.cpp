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
 * Filename: statusmodel.cpp
 * Author: Anh, Ngo Huy
 * Created date:2/4/2023
 * Brief:
 */
#include "statusmodel.h"
#include "logger.h"
#include "errcode.h"

QHash<int, QString> *StatusModelCtl::getStatusIdNameMap()
{
    traced;
    static bool isInitiedStatusNameMap = false;
    static QHash<int, QString> map;
    if (!isInitiedStatusNameMap) {
        map.insert(STATUS_NONE, "Không rõ");
        map.insert(STATUS_INACTIVE, "Không hoạt động");
        map.insert(STATUS_ACTIVE, "Đang hoạt động");
        map.insert(STATUS_BUILDING, "Đang xây dựng");
        isInitiedStatusNameMap = true;
    }
    // TODO: make it as static to load once only???
    tracede;
    return &map;
}

QString StatusModelCtl::status2Name(StatusModel status)
{
    QHash<int, QString>* statuses = getStatusIdNameMap();
    QString ret;
    traced;
    logd("Status to get name %d", status);
    if (statuses->contains(status)){
        ret = statuses->value(status);
    } else {
        loge("invalid status %d", status);
        ret = "Không rõ"; // TODO: translate???
    }
    tracede;
    return ret;
}
