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
 * Filename: defs.h
 * Author: Anh, Ngo Huy
 * Created date:7/28/2022
 * Brief:
 */
#ifndef DEFS_H
#define DEFS_H


#define UNUSED(name) (void)name;

#define VERSION_CODE(major,minor,patch) (major * 1000000 + minor * 1000 + patch)

#define CSV_ITEM_SPLIT ','
#define CSV_SUBITEM_SPLIT ';'

enum Gender{
    FEMALE = 0,
    MALE,

    GENDER_UNKNOWN
};

const char* const KLanguage = "vi";

const char* const KPrebuiltDirName = "prebuilt";
const char* const KPrebuiltSaintCSVFileName = "saints";
const char* const KPrebuiltCountryCSVFileName = "country";
const char* const KPrebuiltEduCSVFileName = "edu";
const char* const KPrebuiltSpecialistCSVFileName = "specialist";
const char* const KPrebuiltProvinceCSVFileName = "province";
const char* const KPrebuiltEthnicCSVFileName = "ethnic";
const char* const KPrebuiltMissionCSVFileName = "mission";
const char* const KPrebuiltAreaCSVFileName = "area";
const char* const KPrebuiltDeptJsonFileName = "dept";
const char* const KPrebuiltCommunityJsonFileName = "community";
const char* const KPrebuiltHomeHtmlFileName = "home";
const char* const KPrebuiltLogoFileName = "logo.png"; // special case, need file extenstion
const char* const KPrebuiltPersonInfoTemplateFileName = "person_info_template";
const char* const KPrebuiltCourseCSVFileName = "course";

const char* const KModelHdlCountry = "country";
const char* const KModelHdlEdu = "education";
const char* const KModelHdlCommunity = "community";
const char* const KModelHdlPerson = "person";
const char* const KModelHdlSaint = "saint";
const char* const KModelHdlSpecialist = "specialist";
const char* const KModelHdlProvince = "province";
const char* const KModelHdlEthnic = "ethnic";
const char* const KModelHdlMission = "mission";
const char* const KModelHdlArea = "area";
const char* const KModelHdlDept = "department";
const char* const KModelHdlCourse = "course";

const char* const KDataFormatList = "QList";
const char* const KDataFormatStringList = "QStringList";
const char* const KDataFormatJson = "QJson";

const char* const KFileTypeCSV = "csv";
const char* const KFileTypeJson = "json";
const char* const KFileTypeExcelX = "xlsx";

const char* const KExportFieldName = "name";
const char* const KExportFieldFirstName = "firstname";
const char* const KExportFieldLastName = "lastname";
const char* const KExportFieldFullName = "fullname";
const char* const KExportFieldBirthday = "birthday";
const char* const KExportFieldBirthplace = "birthplace";
const char* const KExportFieldHollyUids = "holly_uids";
const char* const KExportFieldHollyName = "hollyname";
const char* const KExportFieldNationality = "nationality";
const char* const KExportFieldNationalityUid = "nationality_uid";

// TODO: make those key names configurable????
// ALL must me upper here, but key in file may lower or any, as it'll be converted to upper
// when parsing
static const char* KCsvItemName = "TEN";
static const char* KCsvItemPersonCode = "MA";
static const char* KCsvItemChristenDate = "NGAY_RUA_TOI";
static const char* KCsvItemBirthDay = "NGAY_SINH";
static const char* KCsvItemBirthPlace = "NOI_SINH";
static const char* KCsvItemHollyName = "TEN_THANH";


#endif // DEFS_H
