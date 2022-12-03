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
 * Filename: ethnicctl.h
 * Author: Anh, Ngo Huy
 * Created date:8/14/2022
 * Brief:
 */
#ifndef ETHNICCTL_H
#define ETHNICCTL_H

#include "controller.h"

#include "dbmodel.h"
#include "ethnic.h"
#include <QList>
#include <QHash>

#define ETHNIC (EthnicCtl::getInstance())

class EthnicCtl : public Controller
{
public:
    EthnicCtl();
protected:
    DbModel *buildModel(void *items, const QString &fmt);
public:
    virtual DbModelHandler* getModelHandler();
    const QList<Ethnic*>* getEthnicList(const QString& country);
    const QList<Ethnic*> getAllEthnics();
    const QHash<QString, QList<Ethnic*>*> getEthnicList();
    static EthnicCtl* getInstance();

public slots:
    virtual void onLoad();
private:
    static EthnicCtl* gInstance;
    bool mLoaded;
    QHash<QString, QList<Ethnic*>*> mEthnicList;
};

#endif // ETHNICCTL_H
