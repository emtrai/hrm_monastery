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
 * Filename: exception.cpp
 * Author: Anh, Ngo Huy
 * Created date:3/18/2023
 * Brief:
 */
#include "exception.h"
#include <QString>

MyException::MyException(const QString &msg) throw()
{
    mMessage.append(msg.toStdString().c_str());
}

const char *MyException::what() const _NOEXCEPT
{
    return mMessage.data();
}
