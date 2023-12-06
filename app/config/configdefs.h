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
 * Filename: configfield.h
 * Author: Anh, Ngo Huy
 * Created date:9/30/2023
 * Brief:
 */
#ifndef CONFIGDEFS_H
#define CONFIGDEFS_H

#define DEFAULT_CODE_LENGTH (8)
const char* const CONFIG_FNAME = "config.ini";
const char* const DEFAULT_PREFIX_PERSON = "NUTU_";
const char* const CONFIG_PREFIX = "codeprefix";
const char* const CONFIG_CODE_LENGTH = "codelength";
const char* const CONFIG_LOG_LEVEL = "loglevel";
const char* const CONFIG_PERSON_CODE_VAL_MAX = "person_code_max";

#endif // CONFIGDEFS_H
