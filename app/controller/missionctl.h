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
 * Filename: missionctl.h
 * Author: Anh, Ngo Huy
 * Created date:8/15/2022
 * Brief:
 */
#ifndef MISSIONCTL_H
#define MISSIONCTL_H

#include "controller.h"

#include "dbmodel.h"
#include "mission.h"


class MissionCtl : public Controller
{
public:
    MissionCtl();
protected:
    DbModel *buildModel(void *items, const QString &fmt);
public:
    QList<Mission*> getCountryList();
    static MissionCtl* getInstance();

public slots:
    virtual void onLoad();
private:
    static MissionCtl* gInstance;
    bool mLoaded;
    QList<Mission*> mMissionList;
};

#endif // MISSIONCTL_H
