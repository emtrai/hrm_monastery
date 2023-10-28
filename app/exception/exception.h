/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: exception.h
 * Author: Anh, Ngo Huy
 * Created date:3/18/2023
 * Brief:
 */
#ifndef EXCEPTION_H
#define EXCEPTION_H
#include "logger.h"
#include <QException>
#include <QByteArray>
#include <QThread>

#define THROWEX(fmt,...) \
do {\
    loge("EXCEPTION!! " fmt, ##__VA_ARGS__);\
    QThread::sleep(1); \
    throw MyException(QString().asprintf("%s %s:%d" fmt, THIS_FILE, __func__, __LINE__, ##__VA_ARGS__));\
} while(0)


#define TRY try


#define CATCH(ret, fmt, ...) \
catch (MyException& ex) { \
        loge("MyException! %s " fmt, ex.what(),  ##__VA_ARGS__); \
        ret = ErrException; \
} catch(const std::runtime_error& ex) { \
    loge("Runtime Exception! %s. " fmt, ex.what(), ##__VA_ARGS__); \
    ret = ErrException; \
} catch (const std::exception& ex) { \
    loge("Exception! %s" fmt, ex.what(), ##__VA_ARGS__); \
    ret = ErrException; \
}catch (...) { \
    loge("Exception! Unknown. " fmt, ##__VA_ARGS__); \
    ret = ErrException; \
}

#ifdef Q_OS_WIN
#define _NOEXCEPT _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
#endif

class MyException : public QException
{
public:
    MyException(const QString& msg) throw();
    void raise() const override { throw *this; }
    MyException *clone() const override { return new MyException(*this); }

    virtual const char* what() const _NOEXCEPT override;
private:
    QByteArray mMessage;
};
#endif // EXCEPTION_H
