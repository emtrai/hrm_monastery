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


#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define APP_VERSION STRINGIFY(_APP_VERSION)

#define _APP_VERSION VER_MAJOR.VER_MINOR.VER_PATCH

#define UNUSED(name) (void)name;

#define VERSION_CODE(major,minor,patch) ((major << 24) | (minor << 16 )| (patch))

#define APP_VERSION_CODE VERSION_CODE(VER_MAJOR,VER_MINOR,VER_PATCH)

#define CSV_ITEM_SPLIT ','
#define CSV_SUBITEM_SPLIT ';'

// TODO: make it common in other file????
#define HOLLYNAME_SPLIT ","
#define NAME_SPLIT ","

enum Gender{
    FEMALE = 0,
    MALE,

    GENDER_UNKNOWN
};

const char* const KLanguage = "vi";
const char* const KManagementDeptNameId = "QUAN_LY";

const char* const KPrebuiltDirName = "prebuilt";
const char* const KWorkingDirName = "data";
const char* const KImageDirName = "image";
const char* const KPeopleImageDirName = "people";
const char* const KDataDirName = ".appdata";
const char* const KLogDirName = "log";
const char* const KDatabasename = "db.db";
const char* const KDatabaseMetaName = "db.json";

const char* const KDataFormatList = "QList";
const char* const KDataFormatStringList = "QStringList";
const char* const KDataFormatJson = "QJson";

const char* const KFileTypeCSV = "csv";
const char* const KFileTypeJson = "json";
const char* const KFileTypeExcelX = "xlsx";
const char* const KFileTypeHtml = "xlsx";

const char* const KItemImgPath = "img_path";
const char* const KItemUid = "UID";
const char* const KItemGender = "GIOI_TINH";
const char* const KItemPersonCode = "MA_NU_TU";
const char* const KItemPerson = "NU_TU";
const char* const KItemName = "TEN";
const char* const KItemFirstName = "TEN_DAU";
const char* const KItemLastName = "HO";
const char* const KItemFullName = "TEN_DAY_DU";
const char* const KItemOriginName = "TEN_GOC";
const char* const KItemBirthday = "NGAY_SINH";
const char* const KItemBirthplace = "NOI_SINH";
const char* const KItemHollyUids = "holly_uids";
const char* const KItemHollyName = "TEN_THANH";
const char* const KItemHolly = "THANH";
const char* const KItemFeastDay = "NGAY_BON_MANG";
const char* const KItemArea = "KHU_VUC";
const char* const KItemAreaNameId = "MA_KHU_VUC";
const char* const KItemNationality = "QUOC_TICH";
const char* const KItemNationalityUid = "nationality_uid";
const char* const KItemEthnicUID = "MA_DAN_TOC";
const char* const KItemEthnic = "DAN_TOC";
const char* const KItemIDcard = "CAN_CUOC_CONG_DAN";
const char* const KItemIDcardIssueDate = "NGAY_CAP_CCCD";
const char* const KItemIDcardIssuer = "NOI_CAP_CCCD";
const char* const KItemStatus = "TINH_TRANG";
const char* const KItemStatusId = "MA_TINH_TRANG";
const char* const KItemRetireDate = "NGAY_NGHI_HUU";
const char* const KItemRetirePlace = "NOI_NGHI_HUU";
const char* const KItemDeadDate = "NGAY_MAT";
const char* const KItemDeadPlace = "NOI_CHON_CAT";
const char* const KItemCountry = "QUOC_GIA";
const char* const KItemCountryUid = "UID_QUOC_GIA";
const char* const KItemCountryNameId = "MA_QUOC_GIA";
const char* const KItemProvince = "TINH_TP";
const char* const KItemAddress = "DIA_CHI";
const char* const KItemChurchAddress = "GIAO_XU";
const char* const KItemTel = "DIEN_THOAI";
const char* const KItemEmail = "EMAIL";
const char* const KItemOtherContact = "LIEN_LAC_KHAC";
const char* const KItemEdu = "HOC_VAN";
const char* const KItemEduDetail = "CHI_TIET_HOC_VAN";
const char* const KItemEduNameId = "MA_HOC_VAN";
const char* const KItemSpecialist = "CHUYEN_MON";
const char* const KItemSpecialistInfo = "CHI_TIET_CHUYEN_MON";
const char* const KItemSpecialistUid = "MA_CHUYEN_MON";
const char* const KItemWork = "CONG_VIEC";
const char* const KItemWorkId = "MA_CONG_VIEC";
const char* const KItemWorkHistory = "LICH_SU_CONG_TAC";
const char* const KItemDepartment = "PHONG_BAN";
const char* const KItemDepartmentName = "TEN_PHONG_BAN";
const char* const KItemDepartmentNameId = "MA_PHONG_BAN";
const char* const KItemDepartmentDbId = "PHONG_BAN_DBID";
const char* const KItemCommunity = "CONG_DOAN";
const char* const KItemCommunityName = "TEN_CONG_DOAN";
const char* const KItemCommunityDbId = "CONG_DOAN_DBID";
const char* const KItemCommunityDept = "CONG_DOAN_PHONG_BAN";
const char* const KItemCommunityNameId = "MA_CONG_DOAN";
const char* const KItemParentCommunity = "CONG_DOAN_CHA";
const char* const KItemParentCommunityNameId = "MA_CONG_DOAN_CHA";
const char* const KItemCommunityHistory = "LICH_SU_CONG_DOAN";
const char* const KItemParentModel = "CHU_QUAN";
const char* const KItemDad = "TEN_CHA"; // TODO: holly name of dad?
const char* const KItemDadBirthday = "NGAY_SINH_CHA";
const char* const KItemDadAddr = "DIA_CHI_CHA";
const char* const KItemMom = "TEN_ME"; // TODO: holly name of mom?
const char* const KItemMomBirthday = "NGAY_SINH_ME";
const char* const KItemMomAddr = "DIA_CHI_ME";
const char* const KItemBrief = "TOM_TAT";
const char* const KItemFamilyHistory = "THONG_TIN_GIA_DINH";
const char* const KItemFamilyContact = "LIEN_HE_GIA_DINH";
const char* const KItemChristenDate = "NGAY_RUA_TOI";
const char* const KItemChristenPlace = "NOI_RUA_TOI";
const char* const KItemEucharistDate = "NGAY_THANH_THE";
const char* const KItemEucharistPlace = "NOI_THANH_THE";
const char* const KItemHollyDate = "NGAY_THEM_SU";
const char* const KItemHollyPlace = "NOI_THEM_SUC";
const char* const KItemCourse = "KHOA";
const char* const KItemCourseType = "LOAI_KHOA"; // course type like nhiem ky, nam tu, khoa tu, etc.
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
const char* const KItemChangeHistory = "LICH_SU_THAY_DOI";
const char* const KItemEducation = "HOC_VAN";
const char* const KItemEvent = "SU_KIEN";
const char* const KItemImg = "HINH_ANH";
const char* const KItemEstablishDate = "NGAY_THANH_LAP";
const char* const KItemCloseDate = "NGAY_DUNG_HOAT_DONG";
const char* const KItemIntro = "GIOI_THIEU";
const char* const KItemFullIntro = "THONG_TIN_CHI_TIET";
const char* const KItemCEO = "TONG_PHU_TRACH";
const char* const KItemCEONameId = "MA_TONG_PHU_TRACH";
const char* const KItemNameId = "MA_DINH_DANH";
const char* const KItemMission = "SU_VU";
const char* const KItemMissionNameId = "MA_SU_VU";
const char* const KItemLastUpdateTime = "THOI_GIAN_CAP_NHAT_CUOI";
const char* const KItemCreateTime = "NGAY_TAO";
const char* const KItemStartDate = "NGAY_BAT_DAU";
const char* const KItemEndDate = "NGAY_KET_THUC";
const char* const KItemType = "LOAI";
const char* const KItemPeriod = "CHU_KY";
const char* const KItemRole = "VAI_TRO";
const char* const KItemRoleNameId = "MA_VAI_TRO";
const char* const KItemLevel = "CAP";
const char* const KItemContact = "LIEN_HE";
const char* const KItemTerm = "NHIEM_KY";
const char* const KItemTermNameId = "MA_NHIEM_KY";
const char* const KItemPersonNameId = "MA_DINH_DANH_NU_TU";
const char* const KItemPersonName = "TEN_NU_TU";
const char* const KItemPersonEvent= "SU_KIEN_NU_TU";
const char* const KItemDate= "NGAY_THANG";

// Special item name
const char* const KItemDbStatus = "DB_STATUS";
const char* const KItemDbHistory = "DB_HISTORY";

// TODO: make those key names configurable????
// ALL must me upper here, but key in file may lower or any, as it'll be converted to upper
// when parsing
const char* const KCsvItemName = "TEN";
const char* const KCsvItemPersonCode = "MA";
const char* const KCsvItemNameId = "MA_DINH_DANH";
const char* const KCsvItemChristenDate = "NGAY_RUA_TOI";
const char* const KCsvItemBirthDay = "NGAY_SINH";
const char* const KCsvItemBirthPlace = "NOI_SINH";
const char* const KCsvItemHollyName = "TEN_THANH";


#endif // DEFS_H
