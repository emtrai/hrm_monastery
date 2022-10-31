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

QString getDateTypeString(TableFieldDatatype_t dataType){
    static QMap<TableFieldDatatype_t, QString> map;
    map[TEXT] = "TEXT";
    map[INT32] = "INTEGER";
    map[INT64] = "INTEGER";

    return map[dataType];
}

QString getFieldNameFromId(int fieldId, bool* isOk)
{
    static QMap<int, QString> fieldMap;
    if (isOk) *isOk = false;
    fieldMap[FILTER_FIELD_NAME] = KFieldName;
    fieldMap[FILTER_FIELD_FULL_NAME] = KFieldFullName;
    fieldMap[FILTER_FIELD_HOLLY_NAME] = KFieldHollyName;
    fieldMap[FILTER_FIELD_ADDRESS] = KFieldAddr;
    fieldMap[FILTER_FIELD_AREA] = KFieldAreaName;
    fieldMap[FILTER_FIELD_COMMUNITY] = KFieldCommunityName;
    fieldMap[FILTER_FIELD_DEPARTMENT] = KFieldDepartmentName;
    fieldMap[FILTER_FIELD_WORK] = KFieldWorkName;
    fieldMap[FILTER_FIELD_EDUCATION] = KFieldIEduName;
    fieldMap[FILTER_FIELD_SPECIALIST] = KFieldSpecialistName;
    fieldMap[FILTER_FIELD_MISSON] = KFieldMissionName;

    if (fieldMap.contains(fieldId)) {
        if (isOk) *isOk = true;
        return fieldMap[fieldId];
    } else {
        return QString();
    }
}
