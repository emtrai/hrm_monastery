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

const char* const KFieldUid = "uid";
const char* const KFieldNameId = "nameid";
const char* const KFieldName = "name";
const char* const KFieldRecordStatus = "record_status";
const char* const KFieldFirstName = "first_name";
const char* const KFieldLastName = "lastname";
const char* const KFieldFullName = "fullname";
const char* const KFieldPersonCode = "personcode";
const char* const KFieldId = "id";
const char* const KFieldPersonId = "personid";
const char* const KFieldEventId = "eventid";
const char* const KFieldDate = "date";
const char* const KFieldTitle = "title";
const char* const KFieldRemark = "remark";
const char* const KFieldBirthDay = "birthday";
const char* const KFieldBirthPlace = "birthplace";
const char* const KFieldLevel = "level";
const char* const KFieldParentUid = "parentuid";
const char* const KFieldCreateDate = "createdate";
const char* const KFieldDateFormat = "dateformat";
const char* const KFieldStatus = "status";
const char* const KFieldDbStatus = "dbstatus";
const char* const KFieldCEOUid = "ceouid";
const char* const KFieldChurchUid = "churchuid";
const char* const KFieldMissionUid = "missionuid";
const char* const KFieldAreaUid = "areauid";
const char* const KFieldPreset = "preset";
const char* const KFieldGender = "gender";
const char* const KFieldFeastDay = "feastday";
const char* const KFieldCountry = "country";
const char* const KFieldHistory = "history";
const char* const KFieldRegion= "region";
const char* const KFieldContinent = "continent";
const char* const KFieldShortName = "shortname";
const char* const KFieldCountryDbId = "countrydbid";
const char* const KFieldCountryShortName = "countryshortname";
const char* const KFieldEthnic = "ethnic"; // dan toc
const char* const KFieldState = "state"; // tinh/thanh/bang
const char* const KFieldProvince = "province"; // tinh/thanh/bang
const char* const KFieldParentDbId = "parentdbid";
const char* const KFieldCommunityDbId = "communitydbid";
const char* const KFieldCommunityUid = "communityuid";
const char* const KFieldAddr = "addr";
const char* const KFieldTel = "tel";
const char* const KFieldEmail = "email";
const char* const KFieldBrief = "brief";
const char* const KFieldImgPath = "imgpath";
const char* const KFieldIDCard = "IDCard";
const char* const KFieldIDCardIssueDate = "IDCardissuedate";
const char* const KFieldIDCardIssuePlace = "IDCardissueplace";
const char* const KFieldIHollyName = "hollyname";
const char* const KFieldINationalityName = "nationalityname";
const char* const KFieldIEthnicName = "ethnicname";
const char* const KFieldIEduName = "eduname";
const char* const KFieldPeriod = "period";
const char* const KFieldStartDate = "startdate";
const char* const KFieldEndDate = "enddate";


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
const char* const KTableCountry = "country"; //
const char* const KTableProvince = "province"; //
const char* const KTableEthnic = "ethnic"; //Dan toc
const char* const KTableArea = "area"; //Dan toc
const char* const KTableCourse = "course"; //Dan toc

// TODO: move to class???
QString getDateTypeString(TableFieldDatatype_t dataType);

#endif // DBSQLITEDEFS_H
