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
 * Filename: eventctl.h
 * Author: Anh, Ngo Huy
 * Created date:9/19/2022
 * Brief:
 */
#ifndef EVENTCTL_H
#define EVENTCTL_H

#include "controller.h"
#include "utils.h"
#include <QList>


class Event;

class EventCtl : public Controller
{
    GET_INSTANCE_DECL(EventCtl);
public:
    EventCtl();
    virtual ~EventCtl();
protected:
    DbModel *buildModel(void *items, const QString &fmt);
public:
    const QList<Event*> getEventList();

public:
    virtual ErrCode reloadDb();

public slots:
    virtual void onLoad();
private:
    QList<Event*> mList;
};

#endif // EVENTCTL_H
