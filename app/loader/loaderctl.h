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
 * Filename: loader.h
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef LOADERCTL_H
#define LOADERCTL_H

#include <QList>
#include "controller.h"

#include <QObject>
#include <QString>
#include <QList>
#include "errcode.h"

#define LOADERCTL LoaderCtl::getInstance()

typedef void (*OnFinishLoadListener_t)(int result, void* data);

class LoaderListener {
public:
    virtual void onLoadController (Controller* ctl) = 0;
    virtual void onUnloadController (Controller* ctl) = 0;
};

class LoaderCtl: public QObject
{
        Q_OBJECT
public:
    LoaderCtl();
    static LoaderCtl* getInstance();

    void setOnFinishLoadListener(LoaderListener* listener, void* data = nullptr);
private:

    void add2Loader(Controller* ctl);
    void add2PreLoader(Controller* ctl);
    void registerAll();
    void runLoader(QList<Controller*>& list);
    void unloadLoader(QList<Controller*>& list);
private:
    static LoaderCtl* gInstance;

    QList<Controller*> mListCtl;
    QList<Controller*> mPreLoadListCtl;

//    OnFinishLoadListener_t mListener;
    LoaderListener* mListener;
    void* mListenerData;

//public slots:
public:
    virtual void preLoad();
    virtual void onLoad();
    virtual void onUnload();
};

#endif // LOADERCTL_H
