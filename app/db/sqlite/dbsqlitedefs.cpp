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
 * Filename: dbsqlitedefs.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/22/2022
 * Brief:
 */
#include "dbsqlitedefs.h"
#include <QMap>
#include "filter.h"
#include "errcode.h"
#include "logger.h"
#include "defs.h"

static bool s_fieldMapInit = false;
static QMap<int, QString> s_fieldMap;


static bool s_itemFieldMapInit = false;
static QMap<QString, QString> s_itemFieldMap;

static void initFieldMap()
{
    tracein;
    logd("s_fieldMapInit %d", s_fieldMapInit);
    if (!s_fieldMapInit) {
        logd("Init field map");
        s_fieldMap[FILTER_FIELD_NAME] = KFieldName;
        s_fieldMap[FILTER_FIELD_FULL_NAME] = KFieldFullName;
        s_fieldMap[FILTER_FIELD_HOLLY_NAME] = KFieldHollyName;
        s_fieldMap[FILTER_FIELD_ADDRESS] = KFieldAddr;
        s_fieldMap[FILTER_FIELD_AREA] = KFieldAreaName;
        s_fieldMap[FILTER_FIELD_COMMUNITY] = KFieldCommunityName;
        s_fieldMap[FILTER_FIELD_DEPARTMENT] = KFieldDepartmentName;
        s_fieldMap[FILTER_FIELD_WORK] = KFieldWorkName;
        s_fieldMap[FILTER_FIELD_EDUCATION] = KFieldEduName;
        s_fieldMap[FILTER_FIELD_SPECIALIST] = KFieldSpecialistName;
        s_fieldMap[FILTER_FIELD_MISSON] = KFieldMissionName;
        s_fieldMapInit = true;
    }

    traceout;
}


static void initItemFieldMap()
{
    tracein;
    logd("s_itemFieldMapInit %d", s_itemFieldMapInit);
    if (!s_itemFieldMapInit) {
        logd("Init item field map");
        s_itemFieldMap[KItemName] = KFieldName;
        s_itemFieldMap[KItemUid] = KFieldUid;
        // TODO: add more and more???
        s_fieldMapInit = true;
    }

    traceout;
}

QString getDateTypeString(TableFieldDatatype_t dataType){
    static QMap<TableFieldDatatype_t, QString> map;
    map[TEXT] = "TEXT";
    map[INT32] = "INTEGER";
    map[INT64] = "INTEGER";

    return map[dataType];
}

QString getFieldNameFromId(int fieldId, bool* isOk)
{
    initFieldMap();
    if (isOk) *isOk = false;

    if (s_fieldMap.contains(fieldId)) {
        if (isOk) *isOk = true;
        return s_fieldMap[fieldId];
    } else {
        return QString();
    }
}

QString getFieldNameFromItemName(const QString &itemName, bool *isOk)
{

}
