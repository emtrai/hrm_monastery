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
 * Filename: stringdefs.h
 * Author: Anh, Ngo Huy
 * Created date:4/16/2023
 * Brief:
 */
#ifndef STRINGDEFS_H
#define STRINGDEFS_H
#include <QObject>

#define STR_OK          QObject::tr("Đồng ý")
#define STR_CANCEL      QObject::tr("Hủy")
#define STR_SAVE        QObject::tr("Lưu")
#define STR_IGNORE      QObject::tr("Bỏ qua")
#define STR_HELP        QObject::tr("Giúp đỡ")

#define STR_NO_DATA         QObject::tr("Không có dữ liệu")
#define STR_INVALID_DATA     QObject::tr("Dữ liệu không hợp lệ")

#define STR_BIRTHDAY        QObject::tr("Năm sinh")
#define STR_BIRTHPLACE        QObject::tr("Nơi sinh")
#define STR_CONTACT        QObject::tr("Liên hệ")
#define STR_ADDR        QObject::tr("Địa chỉ")
#define STR_TEL         QObject::tr("Điện thoại")
#define STR_EMAIL       QObject::tr("Email")

#define STR_COUNTRY     QObject::tr("Quốc gia")
#define STR_CHURCH     QObject::tr("Giáo xứ")

#define STR_STT         QObject::tr("STT")

#define STR_NAMEID          QObject::tr("Tên định danh")
#define STR_PERSON_NAMEID   QObject::tr("Tên định danh Nữ tu")
#define STR_NU_TU     QObject::tr("Nữ tu")
#define STR_NAME            QObject::tr("Tên")
#define STR_HOLLYNAME       QObject::tr("Tên Thánh")
#define STR_FULLNAME        QObject::tr("Họ tên")

#define STR_NOTE        QObject::tr("Ghi chú")

#define STR_COMMUNITY   QObject::tr("Cộng đoàn")
#define STR_CURRENT_COMMUNITY   QObject::tr("Cộng đoàn hiện tại")
#define STR_PARENT_COMMUNITY     QObject::tr("Cộng đoàn quản lý")
#define STR_COMMUNITY_NAME   QObject::tr("Tên Cộng đoàn")
#define STR_COMMUNITY_NAMEID   QObject::tr("Mã Cộng đoàn")
#define STR_DEPARTMENT   QObject::tr("Phòng ban")
#define STR_DEPARTMENT_NAME   QObject::tr("Tên Phòng ban")
#define STR_DEPARTMENT_NAMEID   QObject::tr("Mã Phòng ban")
#define STR_ROLE        QObject::tr("Vai trò")
#define STR_ROLE_ID        QObject::tr("Mã vai trò")
#define STR_UNKNOWN     QObject::tr("Không rõ")
#define STR_COURSE      QObject::tr("Khóa")
#define STR_COURSE_TERM      QObject::tr("Khóa/Nhiệm kỳ")
#define STR_COURSE_ID      QObject::tr("Mã khóa")
#define STR_TERM      QObject::tr("Nhiệm kỳ")
#define STR_TERM_ID      QObject::tr("Mã nhiệm kỳ")
#define STR_LOP_KHAN      QObject::tr("Lớp khấn")
#define STR_MODELSTATUS QObject::tr("Tình trạng")
#define STR_STATUS      QObject::tr("Trạng thái")
#define STR_STATUS_ID      QObject::tr("Mã trạng thái")

#define STR_DATE        QObject::tr("Ngày")
#define STR_ESTABLISHDATE        QObject::tr("Ngày thành lập")
#define STR_STARTDATE        QObject::tr("Ngày bắt đầu")
#define STR_ENDDATE     QObject::tr("Ngày kết thúc")
#define STR_NGAY_BON_MANG     QObject::tr("Ngày bổn mạng")
#define STR_EVENT     QObject::tr("Sự kiện")
#define STR_TITLE     QObject::tr("Tiêu đề")
#define STR_CHANGE_COMMUNITY     QObject::tr("Đổi cộng đoàn")

#define STR_IMPORT_FROM_FILE     QObject::tr("Nhập từ tập tin")
#define STR_EXPORT_TO_FILE     QObject::tr("Xuất ra tập tin")

#define STR_AREA     QObject::tr("Khu vực")
#define STR_DATA_EXISTED     QObject::tr("Dữ liệu đã tồn tại")
#define STR_DATA_ERROR     QObject::tr("Dữ liệu lỗi")


#define STR_TONG_PHU_TRACH     QObject::tr("Tổng phụ trách")
#define STR_NHIEM_VU_XA_HOI     QObject::tr("Nhiệm vụ xã hội")

#define STR_ERR_EDIT_DATA     QObject::tr("Lỗi chỉnh sửa dữ liệu")
#define STR_XUAT_DANH_SACH_NU_TU     QObject::tr("Xuất danh sách Nữ tu")
#define STR_XUAT_DU_LIEU_THANH_CONG     QObject::tr("Xuất dữ liệu thành công")
#define STR_XUAT_DU_LIEU_THANH_CONG_FILE     QObject::tr("Xuất dữ liệu thành công. Lưu tại %1")
#define STR_INFO     QObject::tr("Thông tin")
#define STR_ERROR     QObject::tr("Lỗi")
#define STR_FAILED     QObject::tr("Thất bại")

#define STR_TYPE     QObject::tr("Loại")

#define STR_CHUYEN_MON     QObject::tr("Chuyên môn")
#define STR_EDUCATION     QObject::tr("Giáo dục")
#define STR_WORK     QObject::tr("Công việc")

#define STR_NATIONALITY     QObject::tr("Dân tộc")
#define STR_ADD_COMMUNITY     QObject::tr("Thêm cộng đoàn")
#define STR_ADD_PERSON_EVENT     QObject::tr("Thêm Sự kiện cho Nữ tu")

#define STR_NONE QObject::tr("Không")
#define STR_MEMBER QObject::tr("Thành viên")

#endif // STRINGDEFS_H
