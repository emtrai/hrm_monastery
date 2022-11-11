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
const char* const KPrebuiltRoleCSVFileName = "role";

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
const char* const KModelHdlRole = "role";
const char* const KModelHdlPersonEvent = "person_event";
const char* const KModelHdlEvent = "event";

const char* const KModelNamePerson = "person";
const char* const KModelNameArea = "person";
const char* const KModelNamePersonEvent = "person_event";
const char* const KModelNameSaintPerson = "saint_person";
const char* const KModelNameCommDept = "comm_dept";
const char* const KModelNameMapDeptMgr = "dept_mgr";
const char* const KModelNameCommPerson = "comm_pers";
const char* const KModelNameMap = "map_model";
const char* const KModelNamePersonDept = "person_dept";
const char* const KModelNameRole = "role";
const char* const KModelNameDepartment = "department";
const char* const KModelNameAreaComm = "area_community";
const char* const KModelNameAreaPerson = "area_person";
const char* const KModelNameSpecialist = "specialist";
const char* const KModelNameSpecialistPerson = "specialist_person";

const char* const KDataFormatList = "QList";
const char* const KDataFormatStringList = "QStringList";
const char* const KDataFormatJson = "QJson";

const char* const KFileTypeCSV = "csv";
const char* const KFileTypeJson = "json";
const char* const KFileTypeExcelX = "xlsx";

const char* const KItemImgPath = "img_path";
const char* const KItemUid = "MA_DINH_DANH";
const char* const KItemGender = "GIOI_TINH";
const char* const KItemPersonCode = "MA_NU_TU";
const char* const KItemName = "TEN";
const char* const KItemFirstName = "TEN_DAU";
const char* const KItemLastName = "HO";
const char* const KItemFullName = "TEN_DAY_DU";
const char* const KItemOriginName = "TEN_GOC";
const char* const KItemBirthday = "NGAY_SINH";
const char* const KItemBirthplace = "NOI_SINH";
const char* const KItemHollyUids = "holly_uids";
const char* const KItemHollyName = "TEN_THANH";
const char* const KItemFeastDay = "NGAY_BON_MANG";
const char* const KItemNationality = "QUOC_TICH";
const char* const KItemNationalityUid = "nationality_uid";
const char* const KItemEthnic = "DAN_TOC";
const char* const KItemIDcard = "CAN_CUOC_CONG_DAN";
const char* const KItemIDcardIssueDate = "NGAY_CAP_CCCD";
const char* const KItemIDcardIssuer = "NOI_CAP_CCCD";
const char* const KItemStatus = "TINH_TRANG";
const char* const KItemRetireDate = "NGAY_NGHI_HUU";
const char* const KItemRetirePlace = "NOI_NGHI_HUU";
const char* const KItemDeadDate = "NGAY_MAT";
const char* const KItemDeadPlace = "NOI_CHON_CAT";
const char* const KItemCountry = "QUOC_GIA";
const char* const KItemProvince = "TINH_TP";
const char* const KItemAddress = "DIA_CHI";
const char* const KItemChurchAddress = "GIAO_XU";
const char* const KItemTel = "DIEN_THOAI";
const char* const KItemEmail = "EMAIL";
const char* const KItemOtherContant = "LIEN_LAC_KHAC";
const char* const KItemEdu = "HOC_VAN";
const char* const KItemEduUid = "MA_HOC_VAN";
const char* const KItemSpeciaist = "CHUYEN_MON";
const char* const KItemSpeciaistUid = "MA_CHUYEN_MON";
const char* const KItemWork = "CONG_VIEC";
const char* const KItemWorkHistory = "LICH_SU_CONG_TAC";
const char* const KItemCommunity = "CONG_DOAN";
const char* const KItemCommunityHistory = "LICH_SU_CONG_DOAN";
const char* const KItemDad = "TEN_CHA"; // TODO: holly name of dad?
const char* const KItemDadBirthday = "NGAY_SINH_CHA";
const char* const KItemDadAddr = "DIA_CHI_CHA";
const char* const KItemMom = "TEN_ME"; // TODO: holly name of mom?
const char* const KItemMomBirthday = "NGAY_SINH_ME";
const char* const KItemMomAddr = "DIA_CHI_ME";
const char* const KItemFamilyHistory = "THONG_TIN_GIA_DINH";
const char* const KItemFamilyContact = "LIEN_HE_GIA_DINH";
const char* const KItemChristenDate = "NGAY_RUA_TOI";
const char* const KItemChristenPlace = "NOI_RUA_TOI";
const char* const KItemEucharistDate = "NGAY_THANH_THE";
const char* const KItemEucharistPlace = "NOI_THANH_THE";
const char* const KItemHollyDate = "NGAY_THEM_SU";
const char* const KItemHollyPlace = "NOI_THEM_SUC";
const char* const KItemCourse = "KHOA";
const char* const KItemJoinDate = "NGAY_GIA_NHAP";
const char* const KItemJoinPIC = "PHU_TRACH_GIA_NHAP";
const char* const KItemPreTrainDate = "NGAY_TIEN_TAP_VIEN";
const char* const KItemPreTrainPIC = "PHU_TRACH_TIEN_TAP_VIEN";
const char* const KItemTrainDate = "NGAY_TAP_VIEN";
const char* const KItemTrainPIC = "PHU_TRACH_TAP_VIEN";
const char* const KItemVowsDate = "NGAY_TIEN_KHAN";
const char* const KItemVowsCEO = "TONG_PHU_TRACH_TIEN_KHAN";
const char* const KItemEternalVowsDate = "NGAY_VINH_KHAN";
const char* const KItemEternalVowsCEO = "TONG_PHU_TRACH_VINH_KHAN";
const char* const KItemEternalVowsPIC = "PHU_TRACH_VINH_KHAN";
const char* const KItemBankDate = "NGAY_NGAN_KHANH";
const char* const KItemBankPlace = "NOI_NGAN_KHANH";
const char* const KItemGoldenDate = "NGAY_KIM_KHANH";
const char* const KItemGoldenPlace = "NOI_KIM_KHANH";
const char* const KItemEternalDate = "NGAY_VINH_KHANH";
const char* const KItemEternalPlace = "NOI_VINH_KHANH";
const char* const KItemRemark = "GHI_CHU";
const char* const KItemHistory = "LICH_SU";

// Special item name
const char* const KItemDbStatus = "DB_STATUS";
const char* const KItemDbHistory = "DB_HISTORY";

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
const char* const JSON_AREA_UID = "areaid";

#endif // DEFS_H
