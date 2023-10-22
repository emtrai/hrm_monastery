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
 * Filename: imagedefs.h
 * Author: Anh, Ngo Huy
 * Created date:4/29/2023
 * Brief:
 */

#ifndef IMAGEDEFS_H
#define IMAGEDEFS_H

#define IMG_THUMB_H (60)
#define IMG_THUMB_W (60)

#define IMG_CTL_NAME "imgctl"

#define IMG_TAG_PEOPLE "people" // WARNING: any change on this may impact to location to store image!!!

#define DEFAULT_IMAGE_FORMAT "png"

#define THUMB_IMG_PATH(fullPath) QString("%1.thumb").arg(fullPath)


#define ICON_ROOT_PATH      ":/icon/icon"
#define ICON_PATH(name)     ICON_ROOT_PATH "/" name
#define ICON_COURSE         ICON_PATH("icons8-unit-80")
#define ICON_ETHNIC         ICON_PATH("icons8-ethnic-people-100")
#define ICON_WORK           ICON_PATH("icons8-nurse-80")
#define ICON_EDU            ICON_PATH("icons8-education-100")
#define ICON_SPECIALIST     ICON_PATH("icons8-doctor-female-96")
#define ICON_MISSION        ICON_PATH("icons8-hospital-100")
#define ICON_COUNTRY        ICON_PATH("icons8-vietnam-96")
#define ICON_ROLE           ICON_PATH("icons8-unit-80")
#define ICON_SAINT          ICON_PATH("icons8-saint-64")
#define ICON_DEPT           ICON_PATH("icons8-diversity-64.png")
#define ICON_ADD_COMMUNITY  ICON_PATH("icons8-community-64")
#define ICON_ADD_PERSON_EVENT ICON_PATH("icons8-add-64")
#define ICON_ADD            ICON_PATH("icons8-add-64")
#define ICON_PERSON         ICON_PATH("icons8-nun-64")

#endif // IMAGEDEFS_H
