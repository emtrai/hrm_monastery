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
 * Filename: saint.h
 * Author: Anh, Ngo Huy
 * Created date:7/24/2022
 * Brief:
 */
#ifndef SAINT_H
#define SAINT_H

#include <QObject>
#include <QString>

#include "defs.h"
#include "errcode.h"

class Saint: public QObject
{
    Q_OBJECT
        public:
            Saint();

            QString nameid() const;
            const QString &name() const;
            void setName(const QString &newName);

            qint64 feastDay() const;
            void setFeastDay(qint64 newFeastDay);

            Gender gender() const;
            void setGender(Gender newGender);


            const QString &history() const;
            void setHistory(const QString &newHistory);

            const QString &country() const;
            void setCountry(const QString &newCountry);

            const QString &fullName() const;
            void setFullName(const QString &newFullName);

            bool isValid();
            void dump();
            QString toString();

            ErrCode save();
        private:
            QString mName;
            QString mFullName;
            Gender mGender;
            qint64 mFeastDay; // ngay bon mang
            QString mHistory;
            QString mCountry;

};

#endif // SAINT_H
