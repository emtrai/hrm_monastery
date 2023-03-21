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
#include <QTime>
#include <QFile>
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

#ifndef THISFILE
#define THIS_FILE __FILE__
#endif

#define NO_OP do {} while (0)
// TODO: check debug macro


#define THREAD_ID static_cast<long>(reinterpret_cast<intptr_t>(QThread::currentThreadId()))
#define CUR_TIME_STR QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str()
#define CUR_DATE_STR QDate::currentDate().toString("yyyyMMdd").toStdString().c_str()

#define logit(logger, fmt,...) \
    do{ \
        logger("%s@%s %s %s:%d : " fmt, CUR_DATE_STR, CUR_TIME_STR, THIS_FILE, __func__, __LINE__, ##__VA_ARGS__); \
    } while(0)

#ifdef DEBUG_LOG
// TODO: add process id???
#define logd(fmt,...) logit(qDebug, fmt, ##__VA_ARGS__)


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
#define loge(fmt, ...) logit(qCritical, fmt, ##__VA_ARGS__)

#define logi(fmt, ...) logit(qInfo, fmt, ##__VA_ARGS__)

/* Warning something */
#define logw(fmt,...) logit(qWarning, fmt, ##__VA_ARGS__)

class Logger;

class LogWorker : public QObject
{
    Q_OBJECT
public:
    LogWorker();
        virtual ~LogWorker();
public slots:
    void doWork();
    void printLog(QString);
    void startRuning();
    void stop();
signals:
    void resultReady(QString);
private:
    QQueue<QString> mQueue;
    QMutex mMutex;
    QWaitCondition mWait;
    bool isRunning;
    QThread* mThead;
};


class Logger: public QObject {
    Q_OBJECT
//    QThread mWorkerThread;
private:
    static Logger* getInstance();
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
public:
    static void init();
    static void printLog2File(const QString& log);
    static void reqWriteLog(const QString& log);
private:
    Logger();
    ~Logger();
    void doInit();
    QString getLogDirPath();
    QString getLogFilePath();
    void doHandleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void doPrintLog2File(const QString& log);
    void doReqWriteLog(const QString& log);
public slots:
    void handleResults(QString);
signals:
    void operate(QString &);
    void log2File(QString);
private:
    static Logger* gInstance;
    QString mLogFilePath;
    QFile mLogFile;
    LogWorker* mWorker;
};

#endif // LOGGER_H
