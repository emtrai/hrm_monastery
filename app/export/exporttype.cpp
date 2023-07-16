/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: exporttype.cpp
 * Author: Anh, Ngo Huy
 * Created date:1/31/2023
 * Brief:
 */

#include "exporttype.h"
#include "logger.h"
#include "utils.h"
#include "stringdefs.h"

static void getListExportTypeName(QHash<int, QString>* list)
{
    static bool initialized = false;
    static QHash<int, QString> s_ExportTypeName;
    tracein;
    logd("initialized = %d", initialized);
    if (!initialized) {
        logd("not init initExportTypeName, init it");

        s_ExportTypeName.insert(EXPORT_HTML, QObject::tr("HTML"));
        s_ExportTypeName.insert(EXPORT_CSV, QObject::tr("CSV"));
        s_ExportTypeName.insert(EXPORT_CSV_LIST, QObject::tr("CSV_LIST"));
        s_ExportTypeName.insert(EXPORT_DOCX, QObject::tr("DOCX"));
        s_ExportTypeName.insert(EXPORT_XLSX, QObject::tr("XLSX"));

        initialized = true;
    }
    *list = s_ExportTypeName;
    traceout;
}

ErrCode getExportTypeName(uint32_t exportTypes, QHash<int, QString>& exportTypeName)
{
    tracein;
    ErrCode err = ErrNone;
    QHash<int, QString> list;
    getListExportTypeName(&list);
    logd("exportTypes 0x%x", exportTypes);
    if (exportTypes) {
        foreach (int type, list.keys()) {
            logd("type 0x%x", type);
            if (type & exportTypes) {
                QString name = list.value(type);
                logd("found type 0x%x=%s", type, STR2CHA(name));
                exportTypeName.insert(type, name);
            }
        }
    } else {
        logi("empty exportTypes"); // TODO: assert it to make if failed, as it's abnormal case
    }
    logd("exportTypeName cnt %d", exportTypeName.count());
    traceret(err);
    return err;
}


QString typeToExt(ExportType type, bool *isOk)
{
    static bool initialized = false;
    static QHash<int, QString> s_type2String;
    QString extName;
    tracein;
    logd("initialized = %d", initialized);
    if (!initialized) {
        logd("not init s_type2String, init it");

        s_type2String.insert(EXPORT_HTML, QObject::tr("html"));
        s_type2String.insert(EXPORT_CSV, QObject::tr("csv"));
        s_type2String.insert(EXPORT_CSV_LIST, QObject::tr("csv"));
        s_type2String.insert(EXPORT_DOCX, QObject::tr("docx"));
        s_type2String.insert(EXPORT_XLSX, QObject::tr("xlsx"));

        initialized = true;
    }
    logd("type %d", type);
    if (s_type2String.contains(type)) {
        extName = s_type2String.value(type);
        if (isOk) *isOk = true;

        logd("extName %s", STR2CHA(extName));
    } else {
        if (isOk) *isOk = false;
        loge("Unknown export type: %d", type);
    }
    traceout;
    return extName;
}

QString exportItem2Name(const QString &item, bool* isOk)
{
    static bool initialized = false;
    static QHash<QString, QString> s_item2Name;
    QString name;
    tracein;
    logd("initialized = %d", initialized);
    if (!initialized) {
        logd("not init s_type2String, init it");

        s_item2Name.insert(KItemNameId, QObject::tr("Mã định danh"));
        s_item2Name.insert(KItemName, QObject::tr("Tên"));
        s_item2Name.insert(KItemFullName, QObject::tr("Họ Tên"));
        s_item2Name.insert(KItemCountry, QObject::tr("Quốc gia"));
        s_item2Name.insert(KItemTel, QObject::tr("Điện thoại"));
        s_item2Name.insert(KItemParentCommunity, QObject::tr("Cộng đoàn chủ quản"));
        s_item2Name.insert(KItemParentCommunityNameId, QObject::tr("Mã Cộng đoàn chủ quản"));
        s_item2Name.insert(KItemEstablishDate, QObject::tr("Ngày thành lập"));
        s_item2Name.insert(KItemCloseDate, QObject::tr("Ngày dừng hoạt động"));
        s_item2Name.insert(KItemFeastDay, QObject::tr("Ngày bổn mạng"));
        s_item2Name.insert(KItemStatus, QObject::tr("Tình trạng"));
        s_item2Name.insert(KItemBrief, QObject::tr("Tóm tắt"));
        s_item2Name.insert(KItemArea, QObject::tr("Khu vực"));
        s_item2Name.insert(KItemAreaNameId, QObject::tr("Mã khu vực"));
        s_item2Name.insert(KItemIntro, QObject::tr("Giới thiệu"));
        s_item2Name.insert(KItemOtherContact, QObject::tr("Liên lạc khác"));
        s_item2Name.insert(KItemMission, QObject::tr("Sứ vụ"));
        s_item2Name.insert(KItemMissionNameId, QObject::tr("Mã sứ vụ"));
        s_item2Name.insert(KItemCEONameId, QObject::tr("Mã Tổng Phụ Trách"));
        s_item2Name.insert(KItemCEO, QObject::tr("Tổng Phụ Trách"));
        s_item2Name.insert(KItemRemark, QObject::tr("Ghi chú"));
        s_item2Name.insert(KItemIDcard, QObject::tr("Căn cước công dân"));
        s_item2Name.insert(KItemIDcardIssueDate, QObject::tr("Ngày cấp CCCD"));
        s_item2Name.insert(KItemIDcardIssuer, QObject::tr("Nơi cấp CCCD"));
        s_item2Name.insert(KItemCourse, QObject::tr("Khóa/Lớp khấn"));
        s_item2Name.insert(KItemEduNameId, QObject::tr("Mã học vấn"));
        s_item2Name.insert(KItemAddress, QObject::tr("Địa chỉ"));
        s_item2Name.insert(KItemChurchAddress, QObject::tr("Giáo xứ"));
        s_item2Name.insert(KItemEmail, QObject::tr("Email"));
        s_item2Name.insert(KItemHollyName, QObject::tr("Tên Thánh"));
        s_item2Name.insert(KItemBirthday, QObject::tr("Ngày sinh"));
        s_item2Name.insert(KItemBirthplace, QObject::tr("Nơi sinh"));
        s_item2Name.insert(KItemJoinDate, QObject::tr("Ngày nhập tu"));
        s_item2Name.insert(KItemPreTrainDate, QObject::tr("Ngày gia nhập Tiền Tập Viện"));
        s_item2Name.insert(KItemTrainDate, QObject::tr("Ngày gia nhập Tập Viện"));
        s_item2Name.insert(KItemVowsDate, QObject::tr("Ngày Tiên Khấn"));
        s_item2Name.insert(KItemEternalVowsDate, QObject::tr("Ngày Vĩnh Khấn"));
        s_item2Name.insert(KItemDeadDate, QObject::tr("Ngày an nghỉ"));
        s_item2Name.insert(KItemRetireDate, QObject::tr("Ngày nghỉ hưu"));
        s_item2Name.insert(KItemBankDate, QObject::tr("Ngày Ngân Khánh"));
        s_item2Name.insert(KItemGoldenDate, QObject::tr("Ngày Kim khánh"));
        s_item2Name.insert(KItemEternalDate, QObject::tr("Ngày Vĩnh khánh"));
        s_item2Name.insert(KItemPersonNameId, QObject::tr("Mã định danh Nữ tu"));
        s_item2Name.insert(KItemPersonEvent, QObject::tr("Sự kiện Nữ tu"));
        s_item2Name.insert(KItemDate, QObject::tr("Ngày"));
        s_item2Name.insert(KItemTerm, STR_TERM);
        s_item2Name.insert(KItemTermNameId, STR_TERM_ID);
        s_item2Name.insert(KItemContact, STR_CONTACT);
        s_item2Name.insert(KItemLevel, QObject::tr("Cấp"));
        s_item2Name.insert(KItemRole, STR_ROLE);
        s_item2Name.insert(KItemRoleNameId, STR_ROLE_ID);
        s_item2Name.insert(KItemEndDate, STR_ENDDATE);
        s_item2Name.insert(KItemStartDate, STR_STARTDATE);
        s_item2Name.insert(KItemStatusId, STR_STATUS_ID);

        initialized = true;
    }
    logd("item %s", STR2CHA(item));
    if (s_item2Name.contains(item)) {
        name = s_item2Name.value(item);
        if (isOk) *isOk = true;

        logd("name %s", STR2CHA(name));
    } else {
        if (isOk) *isOk = false;
        loge("Unknown export item: %s", STR2CHA(item));
    }
    traceout;
    return name;
}
