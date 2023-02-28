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
 * Filename: eductl.h
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#ifndef EDUCTL_H
#define EDUCTL_H

#include <commonctl.h>
#include "errcode.h"
#include <QStringList>
#include "education.h"

#define EDUCTL EduCtl::getInstance()

class EduCtl : public CommonCtl
{
    GET_INSTANCE_DECL(EduCtl);
private:
    EduCtl();

protected:
    DbModel *buildModel(void *items, const QString &fmt);
    virtual DbModelBuilder getMainBuilder();
    virtual const char *getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();
private:

};


#endif // EDUCTL_H
