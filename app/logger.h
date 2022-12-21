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
#include <QThread>

#ifndef THISFILE
#define THIS_FILE __FILE__
#endif

#define NO_OP do {} while (0)
// TODO: check debug macro

#ifdef DEBUG_LOG
// TODO: add process id???
#define logd(fmt,...) \
    do{ \
        qDebug("D %ld %s %s[%d] " fmt, (long)QThread::currentThreadId(), THIS_FILE, __func__, __LINE__,##__VA_ARGS__); \
    }\
    while(0)

#else // !DEBUG_LOG

#define logd(fmt,...) NO_OP

#endif // DEBUG_LOG

#ifdef DEBUG_TRACE
#define traced logd("IN>> %s", __func__)
#define tracedr(ret) logd("<<END %s with ret=%d", __func__, (int)ret)
#define tracede logd("<<END %s", __func__)

#else // DEBUG_TRACE

#define traced NO_OP
#define tracedr(ret) NO_OP
#define tracede NO_OP
#endif // DEBUG_TRACE

// TODO: push log to file, make separate thread to writing log, to avoid impact
// to performance of application
// TODO: Show crash/critical error on dialog
#define loge(fmt, ...) \
    do{ \
            qDebug("E %ld %s %s[%d] " fmt, (long)QThread::currentThreadId(), THIS_FILE, __func__, __LINE__,##__VA_ARGS__); \
    }\
    while(0)

#define logi(fmt, ...) \
do{ \
        qDebug("I %ld %s %s[%d] " fmt, (long)QThread::currentThreadId(), THIS_FILE, __func__, __LINE__,##__VA_ARGS__); \
}\
    while(0)

#endif // LOGGER_H
