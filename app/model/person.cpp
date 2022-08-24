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


// TODO: make those key names configurable????
static const char* KCsvItemName = "TEN";
static const char* KCsvItemPersonCode = "MA";
static const char* KCsvItemChristenDate = "NGAY RUA TOI";


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

const QStringList &Person::hollyName() const
{
    return mHollyName;
}

QString Person::hollyNameString() const
{
    QString hollyName;
    foreach (QString name, mHollyName){
        if (!hollyName.isEmpty())
            hollyName += " ";
        hollyName += name;
    }

    // TODO: safe? this is stack or pointer????
    return hollyName;
}

void Person::addHollyName(const QString &newHollyName)
{
    logd("Add holy name %s", newHollyName.toStdString().c_str());
    mHollyName.append(newHollyName);
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

const QStringList Person::getListKeyWord() const
{
    traced;
    QStringList fields;
    fields.append(KExportFieldFullName);
    fields.append(KExportFieldBirthday);
    return fields;
}

ErrCode Person::getDataString(const QString &keyword, QString *data, bool *isFile) const
{
    ErrCode ret = ErrNone;
    traced;
    logd("keyword %s", keyword.toStdString().c_str());
    if (keyword == KExportFieldFullName){
        *data = getFullName();
    } else if (keyword == KExportFieldBirthday) {
        *data = Utils::date2String(birthday());
    }

    return ret;
}

const QString &Person::imgPath() const
{
    return mImgPath;
}

void Person::setImgPath(const QString &newImgPath)
{
    mImgPath = newImgPath;
}
