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
#include "logger.h"
#include <QThread>
#include <QQueue>
#include "utils.h"
#include <QFile>
#include "filectl.h"
#include <QDir>
#include <QFile>

#define MAX_LOG_LEN 128

Logger* Logger::gInstance = nullptr;

LogWorker::LogWorker():isRunning(false), mThead(nullptr)
{
    tracein;
}
LogWorker::~LogWorker() {
    tracein;
    stop();
    if (mThead) delete mThead;
    traceout;
}
void LogWorker::doWork() {
    QString result;
    tracein;
    while (isRunning) {
        mMutex.lock();
        mWait.wait(&mMutex);
        mMutex.unlock();
        if (!isRunning) {
            break;
        }
        while (!mQueue.isEmpty()) {
            mMutex.lock();
            QString log = mQueue.dequeue();
            Logger::printLog2File(log);
            mMutex.unlock();
        }
    }

    emit resultReady(result);
    traceout;
}
void LogWorker::printLog(QString log) {
    tracein;
    if (isRunning) {
        mMutex.lock();
        logd("enqueue");
        mQueue.enqueue(log);
        mWait.wakeAll();
        mMutex.unlock();
    } else {
        loge("writing lock thread not started yet");
    }
    traceout;
}

void LogWorker::startRuning()
{
    tracein;
    isRunning = true;
    mThead = new QThread();
    moveToThread(mThead);
    connect(mThead, &QThread::finished, this, &QObject::deleteLater);
    connect(mThead, &QThread::started, this, &LogWorker::doWork);
    mThead->start();
    traceout;
}

void LogWorker::stop()
{
    tracein;
    isRunning = false;
    mWait.notify_all();
    mThead->quit();
    mThead->wait();
    traceout;
}

Logger *Logger::getInstance()
{
    if (!gInstance) {
        gInstance = new Logger();
    }
    return gInstance;
}

Logger::Logger():mWorker(nullptr)
{
    tracein;
}

Logger::~Logger()
{
    tracein;
    if (mWorker) {
        delete mWorker;
    }
    traceout;
}
#define MAX_LOG_FILE (5)
#define MAX_LOG_FILE_SIZE_MB (30)
#define MAX_LOG_FILE_SIZE_BYTE (MAX_LOG_FILE_SIZE_MB*1024*1024)
//#define MAX_LOG_FILE_SIZE_BYTE (100)

#define LOG_FILE_NAME "log"
#define LOG_FILE_NAME_DELIM "_"

void Logger::doInit()
{
    tracein;
    qInstallMessageHandler(Logger::messageHandler);
    QString logDirPath = getLogDirPath();
    logd("logDirPath=%s", STR2CHA(logDirPath));
    QDir logDir(logDirPath);
    QFileInfoList fileList = logDir.entryInfoList();
    qint64 minId = 0;
    qint64 maxId = 0;
    qint64 logid = 0;
    bool ok = false;
    int cntLog = 0;
    QString oldestFile;
    foreach (QFileInfo finfo, fileList) {
        logd("log file '%s' : '%s'", STR2CHA(finfo.fileName()), STR2CHA(finfo.absoluteFilePath()));
        QStringList items = finfo.fileName().split(LOG_FILE_NAME_DELIM);
        if (items.size() > 1) {
            logid = items[1].toInt(&ok);
            if (!ok) {
                logid = 0;
            }
        }
        if (logid) {
            maxId = (logid > maxId)?logid:maxId;
            if ((!minId || (logid < minId))) {
                minId = logid;
                oldestFile = finfo.absoluteFilePath();
            }
            cntLog++;
        }
    }
    logd("minId=%d, maxId=%d, cntLog=%d", minId, maxId, cntLog);
    logd("oldestFile=%s", STR2CHA(oldestFile));
//    QString currLogFPath = getLogFilePath();
    mLogFilePath = getLogFilePath();
    mLogFile.setFileName(mLogFilePath);
    qint64 sz = mLogFile.size();
    logd("current log file size=%ld, MAX_LOG_FILE_SIZE_MB=%d", sz, MAX_LOG_FILE_SIZE_BYTE);
    if (sz > MAX_LOG_FILE_SIZE_BYTE) {
        QString oldLogFname = QString("%1%2%3").arg(LOG_FILE_NAME, LOG_FILE_NAME_DELIM).arg(++maxId);
        QString oldLogFilePath = getLogFilePath(&oldLogFname);
        logd("olLogFilePath=%s", STR2CHA(oldLogFilePath));
        mLogFile.copy(oldLogFilePath);
        mLogFile.resize(0);
    }
    if (cntLog > MAX_LOG_FILE && minId > 0) {
        logd("Delete old file '%s'", STR2CHA(oldestFile));
        QFile file (oldestFile);
        file.remove();
    }
    mLogFile.open(QIODevice::Append | QIODevice::Text);
    QString firstLog = QString("\n--- Start Writing log at '%1' ---\n").arg(QDateTime::currentDateTime().toString());
    mLogFile.write(firstLog.toUtf8());
    logd("open log file '%s'", STR2CHA(mLogFilePath));

    mWorker = new LogWorker();
//    mWorker->moveToThread(&mWorkerThread);
//    connect(&mWorkerThread, &QThread::finished, mWorker, &QObject::deleteLater);
//    connect(&mWorkerThread, &QThread::started, mWorker, &LogWorker::doWork);
//    connect(this, &Logger::operate, worker, &LogWorker::doWork);
    connect(this, &Logger::log2File, mWorker, &LogWorker::printLog);
    connect(mWorker, &LogWorker::resultReady, this, &Logger::handleResults);
    mWorker->startRuning();
//    mWorkerThread.start();
    traceout;
}

QString Logger::getLogDirPath()
{
    QString logDir = FileCtl::getAppDataDir(KLogDirName);
    QDir dir(logDir);
    logd("log dir path '%s'", STR2CHA(logDir));
    if (!dir.exists()){
        dir.mkpath(logDir);
    }
    return logDir;
}

QString Logger::getLogFilePath(const QString* fname)
{
//    return QDir(getLogDirPath()).filePath(QString("log_%1").arg(QDate::currentDate().toString("yyyyMMdd")));
    return QDir(getLogDirPath()).filePath(fname?*fname:QString(LOG_FILE_NAME));
}

void Logger::doHandleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString log;
    bool writeFile = false;
    switch (type) {
    case QtDebugMsg:
        log = QString("Debug: %1\n").arg(msg);
        break;
    case QtInfoMsg:
        log = QString("Info: %1\n").arg(msg);
        writeFile = true;
        break;
    case QtWarningMsg:
        log = QString("Warning: %1\n").arg(msg);
        writeFile = true;
        break;
    case QtCriticalMsg:
        log = QString("Error: %1\n").arg(msg);
        writeFile = true;
        break;
    case QtFatalMsg:
        log = QString("Fatal: %1\n").arg(msg);
        writeFile = true;
        break;
    }

//    fprintf(stderr, "%s", log.size() < MAX_LOG_LEN?STR2CHA(log):STR2CHA(log.chopped(MAX_LOG_LEN)));
    fprintf(stderr, "%s", STR2CHA(log));

    if (writeFile) {
//        logd("Request to write log");
//        emit log2File(log);
        reqWriteLog(log);
        // TODO: why emit signal not effect???
    }
}

void Logger::doPrintLog2File(const QString &log)
{
    tracein;
    logd("doPrintLog2File");
    // TODO: check to change file & delete old file???
    if (!log.isEmpty()) {
        logd("write log '%s' to logfile", STR2CHA(log));
        mLogFile.write(log.toUtf8());
        mLogFile.flush();
    }
    traceout;
}

void Logger::doReqWriteLog(const QString &log)
{
//    tracein;
    mWorker->printLog(log);
//    traceout;
}

void Logger::printLog2File(const QString &log)
{
    getInstance()->doPrintLog2File(log);
}

void Logger::reqWriteLog(const QString &log)
{
    getInstance()->doReqWriteLog(log);
}

QString Logger::logDirPath()
{
    return getInstance()->getLogDirPath();
}

void Logger::init()
{
    getInstance()->doInit();
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    getInstance()->doHandleMessage(type, context, msg);
}


void Logger::handleResults(QString)
{

}
