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
 * Filename: filter.cpp
 * Author: Anh, Ngo Huy
 * Created date:11/1/2022
 * Brief:
 */
#include "filter.h"
#include "logger.h"
#include "errcode.h"
#include <QObject>

static QHash<int, quint32> s_FilterOper;
static QHash<int, QString> s_FilterOperName;

#define OPERATOR_FOR_STRING ((1 << FILTER_OP_EQUAL) | \
                            (1 << FILTER_OP_NOT_EQUAL) | \
                            (1 << FILTER_OP_CONTAIN) | \
                            (1 << FILTER_OP_NOT_CONTAIN))

#define OPERATOR_FOR_STRING_CONTAIN ((1 << FILTER_OP_CONTAIN) | \
                            (1 << FILTER_OP_NOT_CONTAIN))

#define OPERATOR_FOR_STRING_EXACT ((1 << FILTER_OP_EQUAL) | \
                            (1 << FILTER_OP_NOT_EQUAL))

#define OPERATOR_FOR_INT    ((1 << FILTER_OP_EQUAL) | \
                            (1 << FILTER_OP_NOT_EQUAL) | \
                            (1 << FILTER_OP_LESS) | \
                            (1 << FILTER_OP_LESS_EQ) | \
                            (1 << FILTER_OP_GREATER) | \
                            (1 << FILTER_OP_GREATER_EQ))

static void initFilterOps()
{
    static bool initialized = false;
    tracein;
    logd("initialized = %d", initialized);
    if (!initialized) {
        logd("not init FilterOps, init it");
        s_FilterOper.insert(FILTER_FIELD_NAME, OPERATOR_FOR_STRING_CONTAIN);
        s_FilterOper.insert(FILTER_FIELD_FULL_NAME, OPERATOR_FOR_STRING_CONTAIN);
        s_FilterOper.insert(FILTER_FIELD_HOLLY_NAME, OPERATOR_FOR_STRING_CONTAIN);
        s_FilterOper.insert(FILTER_FIELD_BIRTHDAY, OPERATOR_FOR_INT);
        s_FilterOper.insert(FILTER_FIELD_ADDRESS, OPERATOR_FOR_STRING);
        s_FilterOper.insert(FILTER_FIELD_AREA, OPERATOR_FOR_STRING_EXACT);
        s_FilterOper.insert(FILTER_FIELD_COMMUNITY, OPERATOR_FOR_STRING_EXACT);
        s_FilterOper.insert(FILTER_FIELD_EDUCATION, OPERATOR_FOR_STRING_EXACT);
        s_FilterOper.insert(FILTER_FIELD_COURSE, OPERATOR_FOR_STRING_EXACT);
        s_FilterOper.insert(FILTER_FIELD_SPECIALIST, OPERATOR_FOR_STRING_EXACT);
        s_FilterOper.insert(FILTER_FIELD_WORK, OPERATOR_FOR_STRING_EXACT);
        s_FilterOper.insert(FILTER_FIELD_MODEL_STATUS, OPERATOR_FOR_STRING_EXACT);

        s_FilterOperName.insert(FILTER_OP_EQUAL, QObject::tr("= / Bằng"));
        s_FilterOperName.insert(FILTER_OP_NOT_EQUAL, QObject::tr("!= / Khác"));
        s_FilterOperName.insert(FILTER_OP_CONTAIN, QObject::tr("Có chứa"));
        s_FilterOperName.insert(FILTER_OP_NOT_CONTAIN, QObject::tr("Không chứa"));
        s_FilterOperName.insert(FILTER_OP_LESS, QObject::tr("< / Nhỏ hơn"));
        s_FilterOperName.insert(FILTER_OP_LESS_EQ, QObject::tr("<= / Nhỏ hơn hoặc bằng"));
        s_FilterOperName.insert(FILTER_OP_GREATER, QObject::tr("> / Lớn hơn"));
        s_FilterOperName.insert(FILTER_OP_GREATER_EQ, QObject::tr(">= / Lớn hơn hoặc bằng"));

        initialized = true;
    }
    traceout;
}

QHash<int, QString> getFilterOpsList(int filterField)
{
    quint32 ops = 0;
    QHash<int, QString> opsList;
    tracein;
    initFilterOps();
    logd("FilterField %d", filterField);
    if (s_FilterOper.contains(filterField)) {
        ops = s_FilterOper.value(filterField);
    }
    logd("ops 0x%x", ops);
    if (ops) {
        for (int i = 0; i < FILTER_OP_MAX; i++) {
            if (((1 << i) & ops)) {
                logd("found ops %d", i);
                if (s_FilterOperName.contains(i)) {
                    opsList.insert(i, s_FilterOperName.value(i));
                } else {
                    loge("not found name for ops %d", i);
                }

            }
        }
    }
    logd("opsList cnt %d", opsList.count());
    traceout;
    return opsList;
}

const QString &FilterItem::keyword() const
{
    return mKeyword;
}

void FilterItem::setKeyword(const QString &newKeyword)
{
    mKeyword = newKeyword;
}

const QVariant &FilterItem::value() const
{
    return mValue;
}

void FilterItem::setValue(const QVariant &newValue)
{
    mValue = newValue;
}

FilterItem::FilterItem(const QString &item, const QString &keyword, const QVariant& value)
{
    tracein;
    mItem = item;
    if (!keyword.isEmpty())
        mKeyword = keyword;
    else
        logd("no keyword");
    if (value.isValid())
        mValue = value;
    else
        logd("no value");
    traceout;
}

const QString &FilterItem::item() const
{
    return mItem;
}

void FilterItem::setItem(const QString &newItem)
{
    mItem = newItem;
}


FilterKeyworkItem::FilterKeyworkItem():
    catetoryid(0),
    opFlags(0)
{

}

FilterKeyworkItem::FilterKeyworkItem(int catetoryid, qint64 opFlags, QString keyword)
{
    this->catetoryid = catetoryid;
    this->opFlags = opFlags;
    this->keyword = keyword;
}
