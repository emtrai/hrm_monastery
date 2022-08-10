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
typedef enum TableFieldDatatype {
    TEXT = 0,
    INT32,
    INT64
}TableFieldDatatype_t;

const char* const KFieldUuid = "uuid";
const char* const KFieldNameId = "nameid";
const char* const KFieldName = "name";
const char* const KFieldRecordStatus = "record_status";
const char* const KFieldFirstName = "first_name";
const char* const KFieldLastName = "last_name";
const char* const KFieldFullName = "full_name";
const char* const KFieldPersonCode = "person_code";
const char* const KFieldId = "id";
const char* const KFieldPersonId = "person_id";
const char* const KFieldEventId = "event_id";
const char* const KFieldDate = "date";
const char* const KFieldTitle = "title";
const char* const KFieldRemark = "remark";
const char* const KFieldBirthDay = "birthday";
const char* const KFieldLevel = "level";
const char* const KFieldUid = "uid";
const char* const KFieldParentUid = "parent_uid";
const char* const KFieldCreateDate = "create_date";
const char* const KFieldDateFormat = "date_format";
const char* const KFieldStatus = "status";
const char* const KFieldCEOUid = "ceo_uid";
const char* const KFieldChurchUid = "church_uid";
const char* const KFieldMissionUid = "mission_uid";
const char* const KFieldAreaUid = "area_uid";
const char* const KFieldPreset = "preset";
const char* const KFieldGender = "gender";
const char* const KFieldFeastDay = "feast_day";
const char* const KFieldCountry = "country";
const char* const KFieldHistory = "history";

// TABLES NAME
const char* const KTablePerson = "person"; // Person
const char* const KTableCommunity = "community"; // cong doan
const char* const KTableDepartment = "department"; // BQL
const char* const KTableEvent = "event";
const char* const KTableSaint = "saint"; // thanh
const char* const KTableEdu = "edu"; // Education like primary school, high school, etc.
const char* const KTableSpecialist = "specialist"; // chuyen mon
const char* const KTableWork = "work"; // cong viec
const char* const KTableChurch = "church";
const char* const KTableMission = "mission"; // su vu cua cong doan

// TODO: move to class???
QString getDateTypeString(TableFieldDatatype_t dataType);

#endif // DBSQLITEDEFS_H
