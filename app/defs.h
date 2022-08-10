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

#endif // DEFS_H
