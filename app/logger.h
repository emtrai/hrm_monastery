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
 * Filename: personctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#ifndef LOGGER_H
#define LOGGER_H
#include <QDebug>

#ifndef THISFILE
#define THIS_FILE __FILE__
#endif


// TODO: add process id???
#define logd(fmt,...) \
    do{ \
        qDebug("DEBUG %s %s[%d] " fmt, THIS_FILE, __func__, __LINE__,##__VA_ARGS__); \
    }\
    while(0)

#define traced logd("")
#define tracedr(ret) logd("Return %d", ret)

// TODO: push log to file, make separate thread to writing log, to avoid impact
// to performance of application
// TODO: Show crash/critical error on dialog
#define loge(fmt, ...) \
    do{ \
            qDebug("ERROR %s %s[%d] " fmt, THIS_FILE, __func__, __LINE__,##__VA_ARGS__); \
    }\
    while(0)

#define logi(fmt, ...) \
do{ \
        qDebug("INFO %s %s[%d] " fmt, THIS_FILE, __func__, __LINE__,##__VA_ARGS__); \
}\
    while(0)

#endif // LOGGER_H
