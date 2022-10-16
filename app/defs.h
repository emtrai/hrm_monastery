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
const char* const KPrebuiltDeptJsonFileName = "department";
const char* const KPrebuiltCommunityJsonFileName = "community";
const char* const KPrebuiltHomeHtmlFileName = "home";
const char* const KPrebuiltLogoFileName = "logo.png"; // special case, need file extenstion
const char* const KPrebuiltPersonInfoTemplateFileName = "person_info_template";
const char* const KPrebuiltCourseCSVFileName = "course";
const char* const KPrebuiltWorkCSVFileName = "work";
const char* const KPrebuiltStatusCSVFileName = "status";
const char* const KPrebuiltEventCSVFileName = "event";

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
const char* const KModelHdlWork = "work";
const char* const KModelHdlStatus = "status";
const char* const KModelHdlPersonEvent = "person_event";
const char* const KModelHdlEvent = "event";

const char* const KModelNamePerson = "person";
const char* const KModelNamePersonEvent = "person_event";
const char* const KModelNameSaintPerson = "saint_person";
const char* const KModelNameCommDept = "comm_dept";
const char* const KModelNameDeptMgr = "dept_mgr";
const char* const KModelNameCommPerson = "comm_pers";
const char* const KModelNameMap = "map_model";

const char* const KDataFormatList = "QList";
const char* const KDataFormatStringList = "QStringList";
const char* const KDataFormatJson = "QJson";

const char* const KFileTypeCSV = "csv";
const char* const KFileTypeJson = "json";
const char* const KFileTypeExcelX = "xlsx";

const char* const KExportFieldImgPath = "img_path";
const char* const KExportFieldUid = "MA_DINH_DANH";
const char* const KExportFieldGender = "GIOI_TINH";
const char* const KExportFieldPersonCode = "MA_NU_TU";
const char* const KExportFieldName = "TEN";
const char* const KExportFieldFirstName = "TEN_DAU";
const char* const KExportFieldLastName = "HO";
const char* const KExportFieldFullName = "TEN_DAY_DU";
const char* const KExportFieldOriginName = "TEN_GOC";
const char* const KExportFieldBirthday = "NGAY_SINH";
const char* const KExportFieldBirthplace = "NOI_SINH";
const char* const KExportFieldHollyUids = "holly_uids";
const char* const KExportFieldHollyName = "TEN_THANH";
const char* const KExportFieldFeastDay = "NGAY_BON_MANG";
const char* const KExportFieldNationality = "QUOC_TICH";
const char* const KExportFieldNationalityUid = "nationality_uid";
const char* const KExportFieldEthnic = "DAN_TOC";
const char* const KExportFieldIDcard = "CAN_CUOC_CONG_DAN";
const char* const KExportFieldIDcardIssueDate = "NGAY_CAP_CCCD";
const char* const KExportFieldIDcardIssuer = "NOI_CAP_CCCD";
const char* const KExportFieldStatus = "TINH_TRANG";
const char* const KExportFieldRetireDate = "NGAY_NGHI_HUU";
const char* const KExportFieldRetirePlace = "NOI_NGHI_HUU";
const char* const KExportFieldDeadDate = "NGAY_MAT";
const char* const KExportFieldDeadPlace = "NOI_CHON_CAT";
const char* const KExportFieldCountry = "QUOC_GIA";
const char* const KExportFieldProvince = "TINH_TP";
const char* const KExportFieldAddress = "DIA_CHI";
const char* const KExportFieldChurchAddress = "GIAO_XU";
const char* const KExportFieldTel = "DIEN_THOAI";
const char* const KExportFieldEmail = "EMAIL";
const char* const KExportFieldOtherContant = "LIEN_LAC_KHAC";
const char* const KExportFieldEdu = "HOC_VAN";
const char* const KExportFieldSpeciaist = "CHUYEN_MON";
const char* const KExportFieldWork = "CONG_VIEC";
const char* const KExportFieldWorkHistory = "LICH_SU_CONG_TAC";
const char* const KExportFieldCommunity = "CONG_DOAN";
const char* const KExportFieldCommunityHistory = "LICH_SU_CONG_DOAN";
const char* const KExportFieldDad = "TEN_CHA"; // TODO: holly name of dad?
const char* const KExportFieldDadBirthday = "NGAY_SINH_CHA";
const char* const KExportFieldDadAddr = "DIA_CHI_CHA";
const char* const KExportFieldMom = "TEN_ME"; // TODO: holly name of mom?
const char* const KExportFieldMomBirthday = "NGAY_SINH_ME";
const char* const KExportFieldMomAddr = "DIA_CHI_ME";
const char* const KExportFieldFamilyHistory = "THONG_TIN_GIA_DINH";
const char* const KExportFieldFamilyContact = "LIEN_HE_GIA_DINH";
const char* const KExportFieldChristenDate = "NGAY_RUA_TOI";
const char* const KExportFieldChristenPlace = "NOI_RUA_TOI";
const char* const KExportFieldEucharistDate = "NGAY_THANH_THE";
const char* const KExportFieldEucharistPlace = "NOI_THANH_THE";
const char* const KExportFieldHollyDate = "NGAY_THEM_SU";
const char* const KExportFieldHollyPlace = "NOI_THEM_SUC";
const char* const KExportFieldCourse = "KHOA";
const char* const KExportFieldJoinDate = "NGAY_GIA_NHAP";
const char* const KExportFieldJoinPIC = "PHU_TRACH_GIA_NHAP";
const char* const KExportFieldPreTrainDate = "NGAY_TIEN_TAP_VIEN";
const char* const KExportFieldPreTrainPIC = "PHU_TRACH_TIEN_TAP_VIEN";
const char* const KExportFieldTrainDate = "NGAY_TAP_VIEN";
const char* const KExportFieldTrainPIC = "PHU_TRACH_TAP_VIEN";
const char* const KExportFieldVowsDate = "NGAY_TIEN_KHAN";
const char* const KExportFieldVowsCEO = "TONG_PHU_TRACH_TIEN_KHAN";
const char* const KExportFieldEternalVowsDate = "NGAY_VINH_KHAN";
const char* const KExportFieldEternalVowsCEO = "TONG_PHU_TRACH_VINH_KHAN";
const char* const KExportFieldEternalVowsPIC = "PHU_TRACH_VINH_KHAN";
const char* const KExportFieldBankDate = "NGAY_NGAN_KHANH";
const char* const KExportFieldBankPlace = "NOI_NGAN_KHANH";
const char* const KExportFieldGoldenDate = "NGAY_KIM_KHANH";
const char* const KExportFieldGoldenPlace = "NOI_KIM_KHANH";
const char* const KExportFieldEternalDate = "NGAY_VINH_KHANH";
const char* const KExportFieldEternalPlace = "NOI_VINH_KHANH";
const char* const KExportFieldRemark = "GHI_CHU";

// TODO: make those key names configurable????
// ALL must me upper here, but key in file may lower or any, as it'll be converted to upper
// when parsing
const char* const KCsvItemName = "TEN";
const char* const KCsvItemPersonCode = "MA";
const char* const KCsvItemChristenDate = "NGAY_RUA_TOI";
const char* const KCsvItemBirthDay = "NGAY_SINH";
const char* const KCsvItemBirthPlace = "NOI_SINH";
const char* const KCsvItemHollyName = "TEN_THANH";

const char* const JSON_COMMUNITYLIST = "communities";
const char* const JSON_DEPARTMENTS = "departments";
const char* const JSON_ID = "id";
const char* const JSON_NAME = "name";
const char* const JSON_CHURCH = "church";
const char* const JSON_ADDR = "addr";
const char* const JSON_TEL = "tel";
const char* const JSON_EMAIL = "email";
const char* const JSON_BRIEF = "brief";
const char* const JSON_REMARK = "remark";
const char* const JSON_STATUS = "status";
const char* const JSON_FEASTDAY = "feastday";
const char* const JSON_ESTABLISH = "establishday";
const char* const JSON_COMMUNITY_UID = "communityid";

#endif // DEFS_H
