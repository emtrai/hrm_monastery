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
 * Filename: saintctl.h
 * Author: Anh, Ngo Huy
 * Created date:7/29/2022
 * Brief:
 */
#ifndef SAINTCTL_H
#define SAINTCTL_H

#include <modelcontroller.h>
#include "errcode.h"
#include <QStringList>
#include <QHash>
#include "saint.h"
#include "modelcontroller.h"

#define SAINTCTL SaintCtl::getInstance()

class SaintCtl : public ModelController
{
public:
//    QList<Saint*> getListSaints();
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QStringList& items, quint32 idx);
    virtual DbModelBuilder getMainBuilder();
    ErrCode getSaintUidListFromName(const QString& name, QHash<QString, QString>* uidList = nullptr);
    QString getHollyNameFromSaintUidList(const QStringList& uidList);
    QString getHollyNameFromSaintUidList(const QString& uidList);
private:
    SaintCtl();

protected:
    DbModel *buildModel(void *items, const QString &fmt);
    virtual ErrCode parsePrebuiltFile(const QString &fpath, const QString &ftype);
    virtual const char* getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();

public:
    static SaintCtl *getInstance();

///////// MEMBER //////////
private:
    static SaintCtl* gInstance;
    QList<QString> mImportFields;

};

#endif // SAINTCTL_H
