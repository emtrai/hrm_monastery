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
 * Filename: filter.h
 * Author: Anh, Ngo Huy
 * Created date:10/31/2022
 * Brief:
 */
#ifndef FILTER_H
#define FILTER_H


enum FilterField {
    FILTER_FIELD_NAME = 0,
    FILTER_FIELD_FULL_NAME,
    FILTER_FIELD_HOLLY_NAME,
    FILTER_FIELD_ADDRESS,
    FILTER_FIELD_AREA,
    FILTER_FIELD_COMMUNITY,
    FILTER_FIELD_DEPARTMENT,
    FILTER_FIELD_WORK,
    FILTER_FIELD_EDUCATION,
    FILTER_FIELD_SPECIALIST,
    FILTER_FIELD_MISSON,

    FILTER_FIELD_MAX
};

enum FilterOperation {
    FILTER_OP_EQUAL = 0,
    FILTER_OP_NOT_EQUAL,
    FILTER_OP_LESS,
    FILTER_OP_LESS_EQ,
    FILTER_OP_GREATER,
    FILTER_OP_GREATER_EQ,
    FILTER_OP_CONTAIN,
    FILTER_OP_NOT_CONTAIN,
    FILTER_OP_MAX,
    };



#endif // FILTER_H
