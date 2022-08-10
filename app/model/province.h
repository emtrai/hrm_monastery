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
 * Filename: province.h
 * Author: Anh, Ngo Huy
 * Created date:8/1/2022
 * Brief:
 */
#ifndef PROVINCE_H
#define PROVINCE_H

#include <QString>
#include <QList>
#include <QObject>

enum ProvinceType{
    PROVINCE = 0,
    CITY,
    STATE
};

class Province: public QObject
{
public:
    Province();
    const QString &id() const;
    void setId(const QString &newId);

    const QString &name() const;
    void setName(const QString &newName);

    bool isValid();
    void dump();

private:
    QString mId;
    QString mName; // TODO: support multi language
    Province* parent;
    QList<Province> mChildProvince; // City belongs to Province, Province belongs to state
};

#endif // PROVINCE_H
