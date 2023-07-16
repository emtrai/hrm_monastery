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
 * Filename: importtype.h
 * Author: Anh, Ngo Huy
 * Created date:7/1/2023
 * Brief:
 */
#ifndef IMPORTTYPE_H
#define IMPORTTYPE_H

enum ImportTarget {
    IMPORT_TARGET_PERSON = 0,
    IMPORT_TARGET_COMMUNITY,
    IMPORT_TARGET_AREA, // Area contact person
    IMPORT_TARGET_AREA_CONTACT, // Area contact person

    IMPORT_TARGET_MAX
};

enum ImportItemType {
    IMPORT_ITEM_TYPE_NONE       = 0, // just for display, not import
    IMPORT_ITEM_TYPE_MATCH_DB   = (1 << 0), // value must found in db, and will update to db
    IMPORT_ITEM_TYPE_MANDATORY  = (1 << 1), // value is mandatory
    IMPORT_ITEM_TYPE_UPDATE_DB  = (1 << 2) // value will be update to DB
};

#endif // IMPORTTYPE_H
