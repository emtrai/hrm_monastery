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
const char* const KFieldParentUid = "parent_uid";
const char* const KFieldCreateDate = "createdate";
const char* const KFieldDateFormat = "dateformat";
const char* const KFieldStatus = "status";
const char* const KFieldDbStatus = "dbstatus";
const char* const KFieldCEOUid = "ceouid";
const char* const KFieldChurchUid = "church_uid";
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
const char* const KFieldCountryDbId = "country_dbid";
const char* const KFieldCountryShortName = "country_shortname";
const char* const KFieldCountryUid = "country_uid";
const char* const KFieldEthnic = "ethnic"; // dan toc
const char* const KFieldState = "state"; // tinh/thanh/bang
const char* const KFieldProvince = "province"; // tinh/thanh/bang
const char* const KFieldProvinceUid = "province_uid"; // tinh/thanh/bang
const char* const KFieldParentDbId = "parent_dbid";
const char* const KFieldCommunityDbId = "community_dbid";
const char* const KFieldCommunityUid = "community_uid";
const char* const KFieldAddr = "addr";
const char* const KFieldTel = "tel";
const char* const KFieldEmail = "email";
const char* const KFieldContact = "contact";
const char* const KFieldBrief = "brief";
const char* const KFieldImgId = "img_id";
const char* const KFieldImgPath = "img_path";
const char* const KFieldIDCard = "IDCard";
const char* const KFieldIDCardIssueDate = "IDCard_issue_date";
const char* const KFieldIDCardIssuePlace = "IDCard_issue_place";
const char* const KFieldHollyName = "hollyname";
const char* const KFieldNationalityName = "nationality_name";
const char* const KFieldNationalityUid = "nationality_uid";
const char* const KFieldEthnicName = "ethnic_name";
const char* const KFieldEthnicUid = "ethnic_uid";
const char* const KFieldIEduName = "edu_name";
const char* const KFieldIEduUid = "edu_uid";
const char* const KFieldPeriod = "period";
const char* const KFieldStartDate = "start_date";
const char* const KFieldEndDate = "end_date";
const char* const KFieldChristenDate = "christen_date"; // bi tich rua toi
const char* const KFieldChristenPlace = "christen_place";
const char* const KFieldEucharistDate = "eucharist_date"; // bi tich thanh the
const char* const KFieldEucharistPlace = "eucharist_place";
const char* const KFieldCourseUid = "course_uid";
const char* const KFieldChurchAddr = "church_addr";
const char* const KFieldDadName = "dad_name";
const char* const KFieldDadBirthDay = "dad_birthday";
const char* const KFieldDadAddr = "dad_addr";
const char* const KFieldMomName = "mom_name";
const char* const KFieldMomBirthDay = "mom_birthday";
const char* const KFieldMomAddr = "mom_addr";
const char* const KFieldFamilyHistory = "family_history";
const char* const KFieldFamilyContact = "family_contact";
const char* const KFieldHollyDate = "holly_date"; // them suc
const char* const KFieldHollyPlace = "holly_place"; // them suc
const char* const KFieldJoinDate = "join_date"; // ngay nhap tu
const char* const KFieldJoinPICUid = "join_pic_uid"; // nguoi dac trach nhap tu
const char* const KFieldPreTrainDate = "pre_train_date"; // ngay gia nhap tien tap vien
const char* const KFieldPreTrainPICUid = "pre_train_pic_uid"; // nguoi dac trach
const char* const KFieldTrainDate = "train_date"; // ngay gia nhap tap vien
const char* const KFieldTrainPICUid = "train_pic_uid"; // giam su tap vien
const char* const KFieldVowsDate = "vows_date"; // ngay tien khan
const char* const KFieldVowsCEOUid = "vows_ceo_uid"; // tong phu trach
const char* const KFieldEternalVowsDate = "eternal_vows_date"; // ngay vinh khan
const char* const KFieldEternalVowsPICUid = "eternal_vows_pic_uid"; // giam su
const char* const KFieldEternalVowsCEOUid = "eternal_vows_ceo_uid"; // tong phu trach
const char* const KFieldBankDate = "bank_date"; // ngan khanh
const char* const KFieldBankPlace = "bank_place";
const char* const KFieldGoldenDate = "golden_date"; // kim khanh
const char* const KFieldGoldenPlace = "golden_place";
const char* const KFieldEternalDate = "eternal_date"; // vinh khanh
const char* const KFieldEternalPlace = "eternal_place";
const char* const KFieldStatusUid = "status_uid";
const char* const KFieldRetireDate = "retire_date";
const char* const KFieldRetirePlace = "retire_place";
const char* const KFieldDeadDate = "dead_date";
const char* const KFieldDeadPlace = "dead_place"; // noi chon cat


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
