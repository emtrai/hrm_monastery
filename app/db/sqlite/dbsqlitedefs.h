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
 * Filename: dbsqlitedefs.h
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */
#ifndef DBSQLITEDEFS_H
#define DBSQLITEDEFS_H

#include <QString>
typedef enum {
    TEXT = 0,
    INT32,
    INT64
}TableFieldDatatype_t;

static const char* KFieldNameId = "nameid";
static const char* KFieldName = "name";
static const char* KFieldFirstName = "first_name";
static const char* KFieldLastName = "last_name";
static const char* KFieldPersonCode = "person_code";
static const char* KFieldId = "id";
static const char* KFieldPersonId = "person_id";
static const char* KFieldEventId = "event_id";
static const char* KFieldDate = "date";
static const char* KFieldTitle = "title";
static const char* KFieldRemark = "remark";
static const char* KFieldBirthDay = "birthday";
static const char* KFieldLevel = "level";
static const char* KFieldUid = "uid";
static const char* KFieldParentUid = "parent_uid";
static const char* KFieldCreateDate = "create_date";
static const char* KFieldDateFormat = "date_format";
static const char* KFieldStatus = "status";
static const char* KFieldCEOUid = "ceo_uid";
static const char* KFieldChurchUid = "church_uid";
static const char* KFieldMissionUid = "mission_uid";
static const char* KFieldAreaUid = "area_uid";
static const char* KFieldPreset = "preset";
static const char* KFieldGender = "gender";
static const char* KFieldFeastDay = "feast_day";
static const char* KFieldCountry = "country";
static const char* KFieldHistory = "history";

// TABLES NAME
static const char* KTablePerson = "person";
static const char* KTableCommunity = "community";
static const char* KTableEvent = "event";
static const char* KTableSaint = "saint";
static const char* KTableEdu = "edu";
static const char* KTableWork = "work";
static const char* KTableChurch = "church";
static const char* KTableMission = "mission";

// TODO: move to class???
QString getDateTypeString(TableFieldDatatype_t dataType);

#endif // DBSQLITEDEFS_H
