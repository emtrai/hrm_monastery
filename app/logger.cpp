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
        if (!isRunning) {
            break;
        }
        if (!mQueue.isEmpty()) {
            QString log = mQueue.dequeue();
            Logger::printLog2File(log);
        }
        mMutex.unlock();
    }

    emit resultReady(result);
    traceout;
}
void LogWorker::printLog(QString log) {
    tracein;
    if (isRunning) {
//        mMutex.lock();
        mQueue.enqueue(log);
//        mMutex.unlock();
        mWait.wakeAll();
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

void Logger::doInit()
{
    tracein;
    qInstallMessageHandler(Logger::messageHandler);
    mLogFilePath = getLogFilePath();
    mLogFile.setFileName(mLogFilePath);
    mLogFile.open(QIODevice::Append | QIODevice::Text);
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
    if (!dir.exists()){
        dir.mkpath(logDir);
    }
    return logDir;
}

QString Logger::getLogFilePath()
{
    return QDir(getLogDirPath()).filePath(QString("log_%1").arg(QDate::currentDate().toString("yyyyMMdd")));
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
    mLogFile.write(log.toUtf8());
    mLogFile.flush();
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
