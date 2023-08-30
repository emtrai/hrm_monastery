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
 * Filename: viewutils.h
 * Author: Anh, Ngo Huy
 * Created date:8/26/2023
 * Brief:
 */
#ifndef VIEWUTILS_H
#define VIEWUTILS_H


#define BUILD_MENU_ACTION_IMPL(title, item, func) \
UITableMenuAction::build(title, this, item) \
           ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode { \
               return this->func(m, a); \
           })

#define BUILD_MENU_SEPARATE UITableMenuAction::buildSeparateAction()

#endif // VIEWUTILS_H
