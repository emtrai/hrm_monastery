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
 * Filename: modeldefs.h
 * Author: Anh, Ngo Huy
 * Created date:4/22/2023
 * Brief:
 */

#ifndef MODELDEFS_H
#define MODELDEFS_H

enum DbModelType {
    MODEL_UNKNOWN = 0,
    MODEL_MAP = 1 << 0,
    MODEL_TYPE_MAX
};

const char* const KModelHdlCountry = "modelhdl@country";
const char* const KModelHdlEdu = "modelhdl@education";
const char* const KModelHdlCommunity = "modelhdl@community";
const char* const KModelHdlPerson = "modelhdl@person";
const char* const KModelHdlSaint = "modelhdl@saint";
const char* const KModelHdlSpecialist = "modelhdl@specialist";
const char* const KModelHdlProvince = "modelhdl@province";
const char* const KModelHdlEthnic = "modelhdl@ethnic";
const char* const KModelHdlMission = "modelhdl@mission";
const char* const KModelHdlArea = "modelhdl@area";
const char* const KModelHdlDept = "modelhdl@department";
const char* const KModelHdlCommDept = "modelhdl@community_dept";
const char* const KModelHdlCourse = "modelhdl@course";
const char* const KModelHdlWork = "modelhdl@work";
const char* const KModelHdlPersonStatus = "modelhdl@person_status";
const char* const KModelHdlRole = "modelhdl@role";
const char* const KModelHdlPersonEvent = "modelhdl@person_event";
const char* const KModelHdlEvent = "modelhdl@event";

const char* const KModelNamePerson = "model@person";
const char* const KModelNameProvince = "model@province";
const char* const KModelNameCountry = "model@country";
const char* const KModelNameArea = "model@area";
const char* const KModelNamePersonEvent = "model@person_event";
const char* const KModelNameSaintPerson = "model@saint_person";
const char* const KModelNameCommDept = "model@comm_dept";
const char* const KModelNameMapDeptMgr = "model@dept_mgr";
const char* const KModelNameCommPerson = "model@comm_pers";
const char* const KModelNameCommManager = "model@comm_manager";
const char* const KModelNameMap = "model@map_model";
const char* const KModelNamePersonDept = "model@person_dept";
const char* const KModelNameRole = "model@role";
const char* const KModelNameDepartment = "model@department";
const char* const KModelNameAreaComm = "model@area_community";
const char* const KModelNameAreaPerson = "model@area_person";
const char* const KModelNameSpecialist = "model@specialist";
const char* const KModelNameSpecialistPerson = "model@specialist_person";
const char* const KModelNameCommunity = "model@community";
const char* const KModelNameSaint = "model@saint";
const char* const KModelNameCourse = "model@course";
const char* const KModelNameEthnic = "model@ethnic"; // dan toc
const char* const KModelNameEvent = "model@event"; // su kien
const char* const KModelNameEducation = "model@education"; //
const char* const KModelNameMission = "model@mission"; //
const char* const KModelNamePersonStatus = "model@person_status"; //
const char* const KModelNameWork = "model@work"; //

const char* const KModelNameIdNone = "KHONG";
const char* const KModelNameIdNone2 = "VN_KHONG";
const char* const KModelNameIdRootCommunity = "HOI_DONG";

const char* const KUidNone = "";
const char* const KNameIdUnkown = "KHONG_XAC_DINH";

const char* const KRoleCEODefaultNameId = "TONG_PHU_TRACH";
#endif // MODELDEFS_H
