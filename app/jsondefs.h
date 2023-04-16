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
 * Filename: jsondefs.h
 * Author: Anh, Ngo Huy
 * Created date:7/28/2022
 * Brief:
 */

#ifndef JSONDEFS_H
#define JSONDEFS_H

const char* const JSON_APPVER = "appversion";
const char* const JSON_APPVERSTR = "appversionstr";
const char* const JSON_DBVER = "dbversion";
const char* const JSON_TABLES = "tables";
const char* const JSON_TABLE = "table";
const char* const JSON_VERSION = "version";

const char* const JSON_COMMUNITYLIST = "communities";
const char* const JSON_ITEMS = "items";
const char* const JSON_DEPARTMENTS = "departments";
const char* const JSON_DEPARTMENT_NAMEID = "department";
const char* const JSON_ID = "id";
const char* const JSON_NAMEID = "nameid";
const char* const JSON_CODE = "code";
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
const char* const JSON_CLOSEDATE = "closeday";
const char* const JSON_COMMUNITY_UID = "community";
const char* const JSON_AREA_UID = "areaid";
const char* const JSON_AREA_NAMEID = "area";
const char* const JSON_PARENT_NAMEID = "parent";
const char* const JSON_MISSION_NAMEID = "mission";
const char* const JSON_COUNTRY_NAMEID = "country";


#define JSON_GET_TO_SET_STR(jobj, key ,func) \
do { \
        if (jobj.contains(key)){ \
            QString tmp = jobj[key].toString().trimmed(); \
            if (!tmp.isEmpty()) { \
                logd("Set %s", key);\
                func(tmp); \
        } else {\
                logw("%s defined but no data", key);\
        } \
    } \
} while (0)

#define JSON_GET_STR(jobj, key, value) \
do { \
    if (jobj.contains(key)){ \
        QString tmp = jobj[key].toString().trimmed(); \
        if (!tmp.isEmpty()) { \
            value = tmp; \
        } else {\
                logw("%s defined but no data", key);\
        } \
    } \
} while (0)

#define JSON_GET_INT(jobj, key, value, defaultValue) \
do { \
    if (jobj.contains(key)){ \
        value = jobj[key].toInteger(defaultValue);\
    } else {\
        logw("%s defined but no data", key);\
    } \
} while (0)

#endif // JSONDEFS_H
