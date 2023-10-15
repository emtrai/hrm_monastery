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
#include <QAtomicInt>

#ifndef THISFILE
#define THIS_FILE __FILE__
#endif

#define NO_OP do {} while (0)
// TODO: check debug macro

#define LOG_ERROR (0)
#define LOG_INFO (1)
#define LOG_VERBOSE (2)
#define LOG_DEBUG (3)

//#define THREAD_ID static_cast<long>(reinterpret_cast<intptr_t>(QThread::currentThreadId()))
#define THREAD_ID static_cast<long>(reinterpret_cast<intptr_t>(QThread::currentThread()))
#define CUR_TIME_STR QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().c_str()
#define CUR_DATE_STR QDate::currentDate().toString("yyyyMMdd").toStdString().c_str()

#define logit(logger, fmt,...) \
    do { \
        logger("0x%08lx %s@%s %s %s:%d : " fmt, THREAD_ID, CUR_DATE_STR, CUR_TIME_STR, THIS_FILE, __func__, __LINE__, ##__VA_ARGS__); \
    } while(0)



#ifdef DEBUG_LOG
// TODO: add process id???
#define logd(fmt,...) logit(qDebug, fmt, ##__VA_ARGS__)

#define dbg(level, fmt, ...) logd(fmt, ##__VA_ARGS__)

#else // !DEBUG_LOG

#define logd(fmt,...) NO_OP

#define dbg(level, fmt, ...) \
    do { \
        if (level <= Logger::getLogLevel()) { \
            logi(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

#endif // DEBUG_LOG

#ifdef DEBUG_TRACE
#define traced logd("CALL << %s >>", __func__)
#define tracein logd("CALL IN>> %s", __func__)
#define traceret(ret) logd("<<CALL OUT %s with ret=%d", __func__, (int)ret)
#define traceout logd("<<CALL OUT %s", __func__)
#define tracelog(fmt, ...) logd(fmt, ##__VA_ARGS__)

#else // DEBUG_TRACE

#define traced NO_OP
#define tracein NO_OP
#define traceout NO_OP
#define traceret(ret) NO_OP
#define tracelog(fmt, ...) NO_OP
#endif // DEBUG_TRACE


// TODO: push log to file, make separate thread to writing log, to avoid impact
// to performance of application
// TODO: Show crash/critical error on dialog
#define loge(fmt, ...) logit(qCritical, fmt, ##__VA_ARGS__)

// print log if error, else do nothing
#define logife(err, fmt, ...) \
    do { if (err != ErrNone) loge(fmt ". err=%d", ##__VA_ARGS__, err); } while (0)

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
    QMutex mMutexWrite;
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
    static QString logDirPath();
    static void setLogLevel(int level);
    static QAtomicInt getLogLevel();
private:
    Logger();
    ~Logger();
    void doInit();
    QString getLogDirPath();
    QString getLogFilePath(const QString* fname = nullptr);
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
    static QAtomicInt gLogLevel;
    QString mLogFilePath;
    QFile mLogFile;
    LogWorker* mWorker;
};

#endif // LOGGER_H
