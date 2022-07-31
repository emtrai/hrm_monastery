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
 * Filename: church.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef CHURCH_H
#define CHURCH_H

#include <QObject>

class Address;

class Church:public QObject
{
    Q_OBJECT
public:
    Church();
    const QString &name() const;
    void setName(const QString &newName);

    const QString &diocesesName() const;
    void setDiocesesName(const QString &newDiocesesName);

    Address *address() const;
    void setAddress(Address *newAddress);

private:
    QString mName; // Ten giao xu
    QString mDiocesesName; // Ten giao phan
    Address* mAddress;

};

#endif // CHURCH_H
