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
#include "person.h"
#include "logger.h"
#include "errcode.h"
#include "dbctl.h"
#include "utils.h"
#include "filectl.h"
#include "iexporter.h"
#include "exportfactory.h"
#include "defs.h"





Person::Person():
    mChristenDate(0)
{

}


Person::~Person()
{
    traced;
}

Person* Person::build(){
    return new Person();
}


//ErrCode_t Person::save()
//{
//    return DbCtl::getDb()->addPerson(this);
//}




//void Person::dump()
//{
//    traced;
//    QString log = QStringLiteral(
//                      "first name '%1'\n"
//                      "last name '%2'\n"
//                      )
//                      .arg(firstName(), lastName());
//    logd("%s", log.toStdString().c_str());
//}

PersonBasic *Person::dad() const
{
    return mDad;
}

void Person::setDad(PersonBasic *newDad)
{
    mDad = newDad;
}

qint64 Person::enlistmentDate() const
{
    return mEnlistmentDate;
}

void Person::setEnlistmentDate(qint64 newEnlistmentDate)
{
    mEnlistmentDate = newEnlistmentDate;
}

const QString &Person::family() const
{
    return mFamily;
}

void Person::setFamily(const QString &newFamily)
{
    mFamily = newFamily;
}

ErrCode Person::fromCSVFile(const QString &fname)
{
    traced;
    QHash<QString, QString> item;
    logd("parse csv file %s", fname.toStdString().c_str());
    ErrCode ret = Utils::parseDataFromCSVFile(fname, &item);
    if (ret == ErrNone){
        logd("Parsed %d key", item.count());
        if (item.contains(KCsvItemName)){
            logd("Set name");
            setName(item.value(KCsvItemName));
        }
        if (item.contains(KCsvItemPersonCode)){
            logd("Set code");
            setPersonCode(item.value(KCsvItemPersonCode));
        }
        if (item.contains(KCsvItemChristenDate)){
            logd("Set christenDate");
            setChristenDate(item.value(KCsvItemChristenDate));
        }
    }

    tracedr(ret);
    return ret;

}

const QString &Person::personCode() const
{
    return mPersonCode;
}

void Person::setPersonCode(const QString &newPersonCode)
{
    mPersonCode = newPersonCode;
}

qint64 Person::christenDate() const
{
    return mChristenDate;
}

void Person::setChristenDate(qint64 newChristenDate)
{
    mChristenDate = newChristenDate;
}

void Person::setChristenDate(const QString &newChristenDate)
{
    mChristenDate = Utils::dateFromString(newChristenDate);
    logd("mChristenDate %s -> %d", newChristenDate.toStdString().c_str(), (int)mChristenDate);
}

ErrCode Person::exportTo(const QString &fpath, ExportType type)
{
    traced;
    ErrCode ret = ErrNone;
    // TODO: implement something here
    logi("Export Person to %s", fpath.toStdString().c_str());
    ret = ExportFactory::exportTo(this, fpath, type);
    tracedr(ret);
    return ret;

}

DbModelHandler *Person::getDbModelHandler()
{
    return DbCtl::getDb()->getSaintModelHandler();
}

const QString Person::exportTemplatePath() const
{
    return FileCtl::getPrebuiltDataFilePath(KPrebuiltPersonInfoTemplateFileName);
}

const QStringList Person::getListExportKeyWord() const
{
    traced;
    QStringList fields;
    fields.append(KExportFieldFullName);
    fields.append(KExportFieldFirstName);
    fields.append(KExportFieldLastName);
    fields.append(KExportFieldBirthday);
    fields.append(KExportFieldBirthplace);
    fields.append(KExportFieldHollyUids);
    fields.append(KExportFieldHollyName);
    fields.append(KExportFieldNationality);
    fields.append(KExportFieldNationalityUid);
    return fields;
}

ErrCode Person::getDataString(const QString &keyword, QString *data, bool *isFile) const
{
    ErrCode ret = ErrNone;
    traced;
    logd("keyword %s", keyword.toStdString().c_str());
    if (keyword == KExportFieldFullName){
        *data = getFullName();
    } else if (keyword == KExportFieldLastName) {
        *data = lastName();
    } else if (keyword == KExportFieldFirstName) {
        *data = firstName();
    } else if (keyword == KExportFieldBirthday) {
        *data = Utils::date2String(birthday());
    } else if (keyword == KExportFieldBirthplace) {
        *data = birthPlace();
    } else if (keyword == KExportFieldHollyName) {
        *data = hollyName();
    } else if (keyword == KExportFieldHollyUids) {
        QString hollyId;
        foreach (QString id, mSaintUidList) {
            if (!hollyId.isEmpty()) {
                hollyId += ", ";
            }
            hollyId += id;
        }
        *data = hollyId;
    } else if (keyword == KExportFieldNationality) {
        *data = nationalityName();
    } else if (keyword == KExportFieldNationalityUid) {
        *data = nationalityUid();
    }


    return ret;
}

const QString &Person::nationalityName() const
{
    return mNationalityName;
}

void Person::setNationalityName(const QString &newNationalityName)
{
    mNationalityName = newNationalityName;
}

void Person::dump()
{
    traced;
    DbModel::dump();
    logd("- FirstName %s", firstName().toStdString().c_str());
    logd("- LastName %s", lastName().toStdString().c_str());
}

ErrCode Person::onImportItem(const QString &keyword, const QString &value)
{
    traced;
    ErrCode ret = ErrNone;
    if (keyword == KCsvItemName) {
        setNameFromFullName(value);
    } else if (keyword == KCsvItemPersonCode) {
        setPersonCode(value);
    } else if (keyword == KCsvItemBirthDay) {
        setBirthday(value);
    } else if (keyword == KCsvItemBirthPlace) {
        setBirthPlace(value);
    } else if (keyword == KCsvItemChristenDate) {
        setChristenDate(value);
    }

    tracedr(ret);
    return ret;
}

const QString &Person::courseUid() const
{
    return mCourseUid;
}

void Person::setCourseUid(const QString &newCourseUid)
{
    mCourseUid = newCourseUid;
}

const QString &Person::course() const
{
    return mCourse;
}

void Person::setCourse(const QString &newCourse)
{
    mCourse = newCourse;
}

const QStringList &Person::specialistUidList() const
{
    return mSpecialistUidList;
}

void Person::setSpecialistUidList(const QStringList &newSpecialistUidList)
{
    mSpecialistUidList = newSpecialistUidList;
}

void Person::clearSpecialistUid()
{
    mSpecialistUidList.clear();
}

void Person::addSpecialistUid(const QString &uid)
{
    mSpecialistUidList.append(uid);
}

const QString &Person::eduName() const
{
    return mEduName;
}

void Person::setEduName(const QString &newEduName)
{
    mEduName = newEduName;
}

const QString &Person::eduUid() const
{
    return mEduUid;
}

void Person::setEduUid(const QString &newEduUid)
{
    mEduUid = newEduUid;
}

const QString &Person::idCardIssuePlace() const
{
    return mIdCardIssuePlace;
}

void Person::setIdCardIssuePlace(const QString &newIdCardIssuePlace)
{
    mIdCardIssuePlace = newIdCardIssuePlace;
}

qint64 Person::idCardIssueDate() const
{
    return mIdCardIssueDate;
}

void Person::setIdCardIssueDate(qint64 newIdCardIssueDate)
{
    mIdCardIssueDate = newIdCardIssueDate;
}

const QString &Person::idCard() const
{
    return mIdCard;
}

void Person::setIdCard(const QString &newIdCard)
{
    mIdCard = newIdCard;
}

const QString &Person::ethnicName() const
{
    return mEthnicName;
}

void Person::setEthnicName(const QString &newEthnicName)
{
    mEthnicName = newEthnicName;
}

const QString &Person::ethnicUid() const
{
    return mEthnicUid;
}

void Person::setEthnicUid(const QString &newEthnicUid)
{
    mEthnicUid = newEthnicUid;
}

const QString &Person::nationalityUid() const
{
    return mNationalityUid;
}

void Person::setNationalityUid(const QString &newNationalityUid)
{
    mNationalityUid = newNationalityUid;
}

const QString &Person::hollyName() const
{
    return mHollyName;
}

void Person::setHollyName(const QString &newHollyName)
{
    mHollyName = newHollyName;
}

const QStringList &Person::saintUidList() const
{
    return mSaintUidList;
}

void Person::setSaintUidList(const QStringList &newSaintUidList)
{
    mSaintUidList = newSaintUidList;
}

void Person::clearSaintUid()
{
    mSaintUidList.clear();
}

void Person::addSaintUid(const QString &saint)
{
    mSaintUidList.append(saint);
}

Person *Person::trainPIC() const
{
    return mTrainPIC;
}

void Person::setTrainPIC(Person *newTrainPIC)
{
    mTrainPIC = newTrainPIC;
}

qint64 Person::trainJoinDate() const
{
    return mTrainJoinDate;
}

void Person::setTrainJoinDate(qint64 newTrainJoinDate)
{
    mTrainJoinDate = newTrainJoinDate;
}

const QString &Person::imgPath() const
{
    return mImgPath;
}

void Person::setImgPath(const QString &newImgPath)
{
    mImgPath = newImgPath;
}
