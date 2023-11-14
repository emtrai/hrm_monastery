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
 * Filename: dlgimportcommunitylistresult.h
 * Author: Anh, Ngo Huy
 * Created date:1/24/2023
 * Brief:
 */
#ifndef DLGIMPORTCOMMUNITYLISTRESULT_H
#define DLGIMPORTCOMMUNITYLISTRESULT_H

#include "dlgimportlistresult.h"

class DlgImportCommunityListResult : public DlgImportListResult
{
public:
    DlgImportCommunityListResult(QWidget *parent = nullptr);
protected:
    virtual void initHeader();
    /**
     * @brief get List of import item
     *        NOTICE: Caller must free returned items after use
     * @param ok
     * @return List of import item. Caller must FREE after use
     */
    virtual QList<UIImportItem*> getItems(bool* ok = nullptr);
};

#endif // DLGIMPORTCOMMUNITYLISTRESULT_H
