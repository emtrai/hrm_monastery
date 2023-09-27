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
 * Filename: filter.h
 * Author: Anh, Ngo Huy
 * Created date:10/31/2022
 * Brief:
 */
#ifndef FILTER_H
#define FILTER_H

#include <QString>
#include <QHash>
#include <QVariant>

enum FilterField {
    FILTER_FIELD_NAME = 0,
    FILTER_FIELD_FULL_NAME,
    FILTER_FIELD_HOLLY_NAME,
    FILTER_FIELD_BIRTHDAY,
    FILTER_FIELD_ADDRESS,
    FILTER_FIELD_AREA,
    FILTER_FIELD_COMMUNITY,
    FILTER_FIELD_DEPARTMENT,
    FILTER_FIELD_WORK,
    FILTER_FIELD_EDUCATION,
    FILTER_FIELD_SPECIALIST,
    FILTER_FIELD_MISSON,
    FILTER_FIELD_COURSE,
    FILTER_FIELD_MODEL_STATUS,

    FILTER_FIELD_MAX
};

enum FilterOperation {
    FILTER_OP_EQUAL = 1,
    FILTER_OP_NOT_EQUAL,
    FILTER_OP_LESS,
    FILTER_OP_LESS_EQ,
    FILTER_OP_GREATER,
    FILTER_OP_GREATER_EQ,
    FILTER_OP_CONTAIN,
    FILTER_OP_NOT_CONTAIN,
    FILTER_OP_MAX,
    };

QHash<int, QString> getFilterOpsList(int filterField);

class FilterKeyworkItem {
public:
    FilterKeyworkItem();
    FilterKeyworkItem(int catetoryid, qint64 opFlags, QString keyword);
    int catetoryid;
    QString catetory;
    qint64 opFlags;
    QString keyword;
    QVariant value;
};

class FilterItem {

    // TODO: copy constructor??
    // TODO: operator = override??
public:
    FilterItem(const QString& item, const QString& keyword, const QVariant& value);
    const QString &item() const;
    void setItem(const QString &newItem);
    const QString &keyword() const;
    void setKeyword(const QString &newKeyword);
    const QVariant &value() const;
    void setValue(const QVariant &newValue);
private:
    QString mItem; // filer item, such as community, etc.
    QString mKeyword; // keyword, if specified, will "contain"
    QVariant mValue; // value, if specified, will "exact". If value is set, keyword is ignored
};


#endif // FILTER_H
