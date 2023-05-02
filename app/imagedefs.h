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

#endif // IMAGEDEFS_H
