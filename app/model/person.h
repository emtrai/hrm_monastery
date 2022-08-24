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
#ifndef PEOPLE_H
#define PEOPLE_H

#include <QObject>
#include <QString>
#include <QSet>
#include "address.h"
#include "errcode.h"
#include "personbasic.h"
#include <QList>
#include "dbmodel.h"
#include "iexporter.h"


class Church;
class Education;
class Work;

class Person: public PersonBasic, public DbModel, public IExporter
{
    Q_OBJECT
        public:
            static Person* build();

        public:
            Person();
            virtual ~Person();

            const QStringList &hollyName() const;
            QString hollyNameString() const;
            void addHollyName(const QString &newHollyName);

//            ErrCode_t save();
//            void dump();

            PersonBasic *dad() const;
            void setDad(PersonBasic *newDad);

            qint64 enlistmentDate() const;
            void setEnlistmentDate(qint64 newEnlistmentDate);

            const QString &family() const;
            void setFamily(const QString &newFamily);

            ErrCode fromCSVFile(const QString& fname);

            const QString &personCode() const;
            void setPersonCode(const QString &newPersonCode);

            qint64 christenDate() const;
            void setChristenDate(qint64 newChristenDate);
            void setChristenDate(const QString& newChristenDate);

            virtual ErrCode exportTo(const QString &fpath, ExportType type);

            const QString &imgPath() const;
            void setImgPath(const QString &newImgPath);

        protected:
            virtual DbModelHandler *getDbModelHandler();
            virtual const QString exportTemplatePath() const;

            virtual const QStringList getListKeyWord() const;
            virtual ErrCode getDataString(const QString& keyword, QString* data, bool* isFile) const;
        protected:
            QString mPersonCode;
            QStringList mHollyName;
            QString mImgPath;

            Address* mFamilyAddr;
            Church* mChurch;

            // family info, includes: name of dad, mom, address, the number of brothers/sisters, etc.
            QString mFamily;

            // TODO: may dad/mom?????
            PersonBasic* mDad;
            PersonBasic* mMom;

            // Rua toi
            qint64 mChristenDate;
            Church* mChristenPlace;

            // Them suc
            qint64 mConfirmationDate;
            Address* mConfirmationPlace;

            QStringList mPersonalTel;
            QStringList mFamilyTel;

            QStringList mEmail;

            QString mCitizenId;

            QList<Education*> mEducationList;
            QList<Work*> mWorkList;

            qint64 mEnlistmentDate; // ngay nhap tu

            qint64 mPreTrainJoinDate; // ngay gia nhap Tap Vien
            Person* mPreTrainPIC; // nguoi dac trac

            qint64 mVowsDate; // ngay tien khan
            Person* mVowsCEO; // chi tong phu trach


            qint64 mEternalVowsDate; // ngay vinh khan
            Person* mEternalVowsCEO; // chi tong phu trach

            qint64 mBankDate; // ngan khanh
            qint64 mGoldenDate; // kim khanh
            qint64 mEternalDate; // vinh khanh


};

#endif // PEOPLE_H
