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
#include <QMap>


//QHash<QString, std::function<QString (Person::*())>> Person::gExportFields; = {
//    {KExportFieldImgPath, nullptr},
//    {KExportFieldFullName, &Person::getFullName},
//    {KExportFieldBirthday, nullptr},
//    {KExportFieldBirthplace, &Person::birthPlace},
//    {KExportFieldHollyName, nullptr},
//    {KExportFieldNationality, nullptr},
//    {KExportFieldEthnic, nullptr},
//    {KExportFieldIDcard, nullptr},
//    {KExportFieldIDcardIssueDate, nullptr},
//    {KExportFieldIDcardIssuer, nullptr},
//    {KExportFieldStatus, nullptr},
//    {KExportFieldRetireDate, nullptr},
//    {KExportFieldRetirePlace, nullptr}, //"retire_place";
//    {KExportFieldDeadDate, nullptr}, //"dead_date";
//    {KExportFieldDeadPlace, nullptr}, //"dead_place";
//    {KExportFieldCountry, nullptr}, //"country";
//    {KExportFieldProvince, nullptr}, //"province";
//    {KExportFieldAddress, nullptr}, //"address";
//    {KExportFieldChurchAddress, nullptr}, //"church_addr";
//    {KExportFieldTel, nullptr}, //"telephone";
//    {KExportFieldEmail, nullptr}, //"email";
//    {KExportFieldOtherContant, nullptr}, //"othercontact";
//    {KExportFieldEdu, nullptr}, //"education";
//    {KExportFieldSpeciaist, nullptr}, //"specialist";
//    {KExportFieldWork, nullptr}, //"work";
//    {KExportFieldWorkHistory, nullptr}, //"work_history";
//    {KExportFieldCommunity, nullptr}, //"community";
//    {KExportFieldCommunityHistory, nullptr}, //"community_history";
//    {KExportFieldDad, nullptr}, //"dad";
//    {KExportFieldDadBirthday, nullptr}, //"dad_birthday";
//    {KExportFieldDadAddr, nullptr}, //"dad_addr";
//    {KExportFieldMom, nullptr}, //"mom";
//    {KExportFieldMomBirthday, nullptr}, //"mom_birthday";
//    {KExportFieldMomAddr, nullptr}, //"mom_addr";
//    {KExportFieldFamilyHistory, nullptr}, //"family_history";
//    {KExportFieldFamilyContact, nullptr}, //"family_contact";
//    {KExportFieldChristenDate, nullptr}, //"christen_date";
//    {KExportFieldChristenPlace, nullptr}, //"christen_place";
//    {KExportFieldEucharistDate, nullptr}, //"eucharist_date";
//    {KExportFieldEucharistPlace, nullptr}, //"eucharist_place";
//    {KExportFieldHollyDate, nullptr}, //"holly_date";
//    {KExportFieldHollyPlace, nullptr}, //"holly_place";
//    {KExportFieldCourse, nullptr}, //"course";
//    {KExportFieldJoinDate, nullptr}, //"join_date";
//    {KExportFieldJoinPIC, nullptr}, //"join_pic";
//    {KExportFieldPreTrainDate, nullptr}, //"pre_train_date";
//    {KExportFieldPreTrainPIC, nullptr}, //"pre_train_pic";
//    {KExportFieldTrainDate, nullptr}, //"train_date";
//    {KExportFieldTrainPIC, nullptr}, //"train_pic";
//    {KExportFieldVowsDate, nullptr}, //"vows_date";
//    {KExportFieldVowsCEO, nullptr}, //"vows_ceo";
//    {KExportFieldEternalVowsDate, nullptr}, //"eternal_vows_date";
//    {KExportFieldEternalVowsCEO, nullptr}, //"eternal_vows_ceo";
//    {KExportFieldEternalVowsPIC, nullptr}, //"eternal_vows_pic";
//    {KExportFieldBankDate, nullptr}, //"bank_date";
//    {KExportFieldBankPlace, nullptr}, //"bank_place";
//    {KExportFieldGoldenDate, nullptr}, //"golden_date";
//    {KExportFieldGoldenPlace, nullptr}, //"golden_place";
//    {KExportFieldEternalDate, nullptr}, //"eternal_date";
//    {KExportFieldEternalPlace, nullptr} //"eternal_place";
//};



Person::Person():
    mChristenDate(0),
    mBirthday(0)
{
    init();
}


Person::~Person()
{
    traced;
}

void Person::clone(const Person &per)
{
    traced;
    mPersonCode = per.personCode();
    mFirstName = per.firstName();
    mLastName = per.lastName();

    mBirthday = per.birthday();
    mBirthPlace = per.birthPlace();

    mHollyName = per.hollyName();
    mSaintUidList = per.saintUidList();

    mImgId = per.imgId();
    mImgPath = per.imgPath();

    mNationalityUid = per.nationalityUid();
    mNationalityName = per.nationalityName();

    mEthnicUid = per.ethnicUid();
    mEthnicName = per.ethnicName();

    mIdCard = per.idCard();
    mIdCardIssuePlace = per.idCardIssuePlace();
    mIdCardIssueDate = per.idCardIssueDate();

    mEduUid = per.eduUid();
    mEduName = per.eduName();

    mSpecialistUidList = per.specialistUidList();

    mCourseUid = per.courseUid();
    mCourse = per.course();

    mCountryUid = per.countryUid();
    mCountryName = per.countryName();
    mProvinceUid = per.provinceUid();
    mProvinceName = per.provinceName();
    mAddr = per.addr();
    mChurchAddr = per.churchAddr();
    mEmail = per.email();
    mTel = per.tel();
    mOtherContact = per.otherContact();

    mDadName = per.dadName();
    mDadBirthday = per.dadBirthday();
    mDadAddr = per.dadAddr();

    mMomName = per.momName();
    mMomBirthday = per.momBirthday();
    mMomAddr = per.momAddr();

    mFamilyHistory = per.familyHistory();
    mFamilyContact = per.familyContact();

    mChristenDate = per.christenDate();
    mChristenPlace = per.christenPlace();

    mEucharistPlace = per.eucharistPlace();
    mEucharistDate = per.eucharistDate();

    mHollyPlace = per.hollyPlace();
    mHollyDate = per.hollyDate();
    // TODO:   mEducationList,    mWorkList;
    mEducationUidList = per.educationUidList();
    mWorkUidList = per.workUidList();

    mJoinDate = per.joinDate();
    mJoinPICUid = per.joinPICUid();
    mJoinPICName = per.joinPICName();

    mPreTrainJoinDate = per.preTrainJoinDate();
    mPreTrainPICUid = per.preTrainPICUid();
    mPreTrainPICName = per.preTrainPICName();
    mTrainJoinDate = per.trainJoinDate();

    mTrainPICUid = per.trainPICUid();
    mTrainPICName = per.trainPICName();

    mVowsDate = per.vowsDate();
    mVowsCEOUid = per.vowsCEOUid();
    mVowsCEOName = per.vowsCEOName();

    mEternalVowsDate = per.eternalVowsDate();
    mEternalVowsCEOUid = per.eternalVowsCEOUid();
    mEternalVowsCEOName = per.eternalVowsCEOName();
    mEternalVowsPICUid = per.eternalVowsPICUid();
    mEternalVowsPICName = per.eternalVowsPICName();

    mBankDate = per.bankDate();
    mBankPlace = per.bankPlace();

    mGoldenDate = per.goldenDate();
    mGoldenPlace = per.goldenPlace();

    mEternalDate = per.eternalDate();
    mEternalPlace = per.eternalPlace();

    mStatusUid = per.statusUid();
    mStatusName = per.statusName();

    mRetireDate = per.retireDate();
    mRetirePlace = per.retirePlace();

    mDeadDate = per.deadDate();
    mDeadPlace = per.deadPlace();

    mEventUidList = per.eventUidList();

}

void Person::buildUidIfNotSet()
{
    QString uidName = QString("%1_%2_%3").arg(hollyName()).arg(getFullName()).arg(birthday());
    setUid(Utils::UidFromName(uidName, UidNameConvertType::NO_VN_MARK_UPPER)); // TODO: handle error
}

DbModel* Person::build(){
    return new Person();
}


void Person::init()
{
    traced;
    initExportFields();
    initImportFields();

}

void Person::initExportFields()
{
    traced;
    // TODO: took to much memory for this, should put in in PersonCtl????
    mExportFields.insert(KExportFieldImgPath, [this](){return this->imgPath();});
    mExportFields.insert(KExportFieldFullName, [this](){
        QString name;
        if (!this->lastName().isEmpty()){
            name = this->lastName();
        }
        if (!name.isEmpty()){
            name += ",";
        }
        name += this->firstName();
        return name;

    });
    mExportFields.insert(KExportFieldBirthday, [this](){
        return Utils::date2String(this->birthday());});
    mExportFields.insert(KExportFieldBirthplace, [this](){return this->birthPlace();});
    mExportFields.insert(KExportFieldHollyName, [this](){return this->hollyName();});
    mExportFields.insert(KExportFieldFeastDay, [this](){return Utils::date2String(this->feastDay(), DATE_FORMAT_MD);});
    mExportFields.insert(KExportFieldNationality, [this](){return this->nationalityName();});
    mExportFields.insert(KExportFieldEthnic, [this](){return this->ethnicName();});
    mExportFields.insert(KExportFieldIDcard, nullptr);
    mExportFields.insert(KExportFieldIDcardIssueDate, nullptr);
    mExportFields.insert(KExportFieldIDcardIssuer, nullptr);
    mExportFields.insert(KExportFieldStatus, nullptr);
    mExportFields.insert(KExportFieldRetireDate, nullptr);
    mExportFields.insert(KExportFieldRetirePlace, nullptr); //"retire_place";
    mExportFields.insert(KExportFieldDeadDate, nullptr); //"dead_date";
    mExportFields.insert(KExportFieldDeadPlace, nullptr); //"dead_place";
    mExportFields.insert(KExportFieldCountry, nullptr); //"country";
    mExportFields.insert(KExportFieldProvince, nullptr); //"province";
    mExportFields.insert(KExportFieldAddress, nullptr); //"address";
    mExportFields.insert(KExportFieldChurchAddress, nullptr); //"church_addr";
    mExportFields.insert(KExportFieldTel, nullptr); //"telephone";
    mExportFields.insert(KExportFieldEmail, nullptr); //"email";
    mExportFields.insert(KExportFieldOtherContant, nullptr); //"othercontact";
    mExportFields.insert(KExportFieldEdu, nullptr); //"education";
    mExportFields.insert(KExportFieldSpeciaist, nullptr); //"specialist";
    mExportFields.insert(KExportFieldWork, nullptr); //"work";
    mExportFields.insert(KExportFieldWorkHistory, nullptr); //"work_history";
    mExportFields.insert(KExportFieldCommunity, nullptr); //"community";
    mExportFields.insert(KExportFieldCommunityHistory, nullptr); //"community_history";
    mExportFields.insert(KExportFieldDad, nullptr); //"dad";
    mExportFields.insert(KExportFieldDadBirthday, nullptr); //"dad_birthday";
    mExportFields.insert(KExportFieldDadAddr, nullptr); //"dad_addr";
    mExportFields.insert(KExportFieldMom, nullptr); //"mom";
    mExportFields.insert(KExportFieldMomBirthday, nullptr); //"mom_birthday";
    mExportFields.insert(KExportFieldMomAddr, nullptr); //"mom_addr";
    mExportFields.insert(KExportFieldFamilyHistory, nullptr); //"family_history";
    mExportFields.insert(KExportFieldFamilyContact, nullptr); //"family_contact";
    mExportFields.insert(KExportFieldChristenDate, nullptr); //"christen_date";
    mExportFields.insert(KExportFieldChristenPlace, nullptr); //"christen_place";
    mExportFields.insert(KExportFieldEucharistDate, nullptr); //"eucharist_date";
    mExportFields.insert(KExportFieldEucharistPlace, nullptr); //"eucharist_place";
    mExportFields.insert(KExportFieldHollyDate, nullptr); //"holly_date";
    mExportFields.insert(KExportFieldHollyPlace, nullptr); //"holly_place";
    mExportFields.insert(KExportFieldCourse, nullptr); //"course";
    mExportFields.insert(KExportFieldJoinDate, nullptr); //"join_date";
    mExportFields.insert(KExportFieldJoinPIC, nullptr); //"join_pic";
    mExportFields.insert(KExportFieldPreTrainDate, nullptr); //"pre_train_date";
    mExportFields.insert(KExportFieldPreTrainPIC, nullptr); //"pre_train_pic";
    mExportFields.insert(KExportFieldTrainDate, nullptr); //"train_date";
    mExportFields.insert(KExportFieldTrainPIC, nullptr); //"train_pic";
    mExportFields.insert(KExportFieldVowsDate, nullptr); //"vows_date";
    mExportFields.insert(KExportFieldVowsCEO, nullptr); //"vows_ceo";
    mExportFields.insert(KExportFieldEternalVowsDate, nullptr); //"eternal_vows_date";
    mExportFields.insert(KExportFieldEternalVowsCEO, nullptr); //"eternal_vows_ceo";
    mExportFields.insert(KExportFieldEternalVowsPIC, nullptr); //"eternal_vows_pic";
    mExportFields.insert(KExportFieldBankDate, nullptr); //"bank_date";
    mExportFields.insert(KExportFieldBankPlace, nullptr); //"bank_place";
    mExportFields.insert(KExportFieldGoldenDate, nullptr); //"golden_date";
    mExportFields.insert(KExportFieldGoldenPlace, nullptr); //"golden_place";
    mExportFields.insert(KExportFieldEternalDate, nullptr); //"eternal_date";
    mExportFields.insert(KExportFieldEternalPlace, nullptr); //"eternal_place";

}

void Person::initImportFields()
{
    traced;
    // TODO: check fields like holly name, country, etc. and mark invalid field???

    mImportFields.insert(KExportFieldImgPath, [this](const QString& value){this->setImgPath(value);});
    mImportFields.insert(KExportFieldFullName, [this](const QString& value){
        this->setNameFromFullName(value);
    });
    mImportFields.insert(KExportFieldBirthday, [this](const QString& value){
        this->setBirthday(value);
    });
    mImportFields.insert(KExportFieldBirthplace, [this](const QString& value){
        this->setBirthPlace(value);
    });
    mImportFields.insert(KExportFieldHollyName, [this](const QString& value){this->setHollyName(value);});
    mImportFields.insert(KExportFieldFeastDay, [this](const QString& value){this->setFeastDay(value);});
    mImportFields.insert(KExportFieldNationality, [this](const QString& value){this->setNationalityName(value);});
    mImportFields.insert(KExportFieldEthnic, [this](const QString& value){this->setEthnicName(value);});
    mImportFields.insert(KExportFieldIDcard, [this](const QString& value){
        this->setIdCard(value);
    });
    mImportFields.insert(KExportFieldIDcardIssueDate, [this](const QString& value){
        this->setIdCardIssueDate(value);
    });
    mImportFields.insert(KExportFieldIDcardIssuer, [this](const QString& value){
        this->setIdCardIssuePlace(value);
    });
    mImportFields.insert(KExportFieldStatus, nullptr);
    mImportFields.insert(KExportFieldRetireDate, nullptr);
    mImportFields.insert(KExportFieldRetirePlace, nullptr); //"retire_place";
    mImportFields.insert(KExportFieldDeadDate, nullptr); //"dead_date";
    mImportFields.insert(KExportFieldDeadPlace, nullptr); //"dead_place";
    mImportFields.insert(KExportFieldCountry, nullptr); //"country";
    mImportFields.insert(KExportFieldProvince, nullptr); //"province";
    mImportFields.insert(KExportFieldAddress, nullptr); //"address";
    mImportFields.insert(KExportFieldChurchAddress, nullptr); //"church_addr";
    mImportFields.insert(KExportFieldTel, [this](const QString& value){
        this->setTel(value);
    }); //"telephone";
    mImportFields.insert(KExportFieldEmail, [this](const QString& value){
        this->setEmail(value);
    }); //"email";
    mImportFields.insert(KExportFieldOtherContant, nullptr); //"othercontact";
    mImportFields.insert(KExportFieldEdu, nullptr); //"education";
    mImportFields.insert(KExportFieldSpeciaist, nullptr); //"specialist";
    mImportFields.insert(KExportFieldWork, nullptr); //"work";
    mImportFields.insert(KExportFieldWorkHistory, nullptr); //"work_history";
    mImportFields.insert(KExportFieldCommunity, nullptr); //"community";
    mImportFields.insert(KExportFieldCommunityHistory, nullptr); //"community_history";
    mImportFields.insert(KExportFieldDad, nullptr); //"dad";
    mImportFields.insert(KExportFieldDadBirthday, nullptr); //"dad_birthday";
    mImportFields.insert(KExportFieldDadAddr, nullptr); //"dad_addr";
    mImportFields.insert(KExportFieldMom, nullptr); //"mom";
    mImportFields.insert(KExportFieldMomBirthday, nullptr); //"mom_birthday";
    mImportFields.insert(KExportFieldMomAddr, nullptr); //"mom_addr";
    mImportFields.insert(KExportFieldFamilyHistory, nullptr); //"family_history";
    mImportFields.insert(KExportFieldFamilyContact, nullptr); //"family_contact";
    mImportFields.insert(KExportFieldChristenDate, nullptr); //"christen_date";
    mImportFields.insert(KExportFieldChristenPlace, nullptr); //"christen_place";
    mImportFields.insert(KExportFieldEucharistDate, nullptr); //"eucharist_date";
    mImportFields.insert(KExportFieldEucharistPlace, nullptr); //"eucharist_place";
    mImportFields.insert(KExportFieldHollyDate, nullptr); //"holly_date";
    mImportFields.insert(KExportFieldHollyPlace, nullptr); //"holly_place";
    mImportFields.insert(KExportFieldCourse, nullptr); //"course";
    mImportFields.insert(KExportFieldJoinDate, nullptr); //"join_date";
    mImportFields.insert(KExportFieldJoinPIC, nullptr); //"join_pic";
    mImportFields.insert(KExportFieldPreTrainDate, nullptr); //"pre_train_date";
    mImportFields.insert(KExportFieldPreTrainPIC, nullptr); //"pre_train_pic";
    mImportFields.insert(KExportFieldTrainDate, nullptr); //"train_date";
    mImportFields.insert(KExportFieldTrainPIC, nullptr); //"train_pic";
    mImportFields.insert(KExportFieldVowsDate, nullptr); //"vows_date";
    mImportFields.insert(KExportFieldVowsCEO, nullptr); //"vows_ceo";
    mImportFields.insert(KExportFieldEternalVowsDate, nullptr); //"eternal_vows_date";
    mImportFields.insert(KExportFieldEternalVowsCEO, nullptr); //"eternal_vows_ceo";
    mImportFields.insert(KExportFieldEternalVowsPIC, nullptr); //"eternal_vows_pic";
    mImportFields.insert(KExportFieldBankDate, nullptr); //"bank_date";
    mImportFields.insert(KExportFieldBankPlace, nullptr); //"bank_place";
    mImportFields.insert(KExportFieldGoldenDate, nullptr); //"golden_date";
    mImportFields.insert(KExportFieldGoldenPlace, nullptr); //"golden_place";
    mImportFields.insert(KExportFieldEternalDate, nullptr); //"eternal_date";
    mImportFields.insert(KExportFieldEternalPlace, nullptr); //"eternal_place";
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

IExporter *Person::getExporter()
{
    return this;
}

const QString &Person::personCode() const
{
    return mPersonCode;
}

void Person::setPersonCode(const QString &newPersonCode)
{
    mPersonCode = newPersonCode;
}

const QString &Person::firstName() const
{
    return mFirstName;
}

void Person::setFirstName(const QString &newFirstName)
{
    mFirstName = newFirstName;
}


const QString &Person::birthPlace() const
{
    return mBirthPlace;
}

void Person::setBirthPlace(const QString &newBirthPlace)
{
    mBirthPlace = newBirthPlace;
}

qint64 Person::birthday() const
{
    return mBirthday;
}

void Person::setBirthday(qint64 newBirthday)
{
    mBirthday = newBirthday;
}

void Person::setBirthday(const QString &newBirthday)
{
    traced;
    bool isOk = false;
    qint64 date = Utils::dateFromString(newBirthday, DATE_FORMAT_YMD, &isOk);
    logd("conver '%s', isOk=%d, value %d", newBirthday.toStdString().c_str(),
         isOk, (int)date);
    if (isOk && date > 0)
        setBirthday(date);
    else
        setBirthday(0); // TODO: should set 0 or return error????
}

const QString &Person::lastName() const
{
    return mLastName;
}

void Person::setLastName(const QString &newLastName)
{
    mLastName = newLastName;
}


ErrCode Person::setNameFromFullName(const QString &name)
{
    QStringList items = name.trimmed().split(" ", Qt::SkipEmptyParts);
    if (items.length() > 0){
        mFirstName = items.last();
        items.removeLast();
    }
    mLastName = items.join(" ");
    logd("first name %s", mFirstName.toStdString().c_str());
    logd("last name %s", mLastName.toStdString().c_str());

    return ErrNone;
}



QString Person::getFullName() const{
    //TODO:check localization
    return mLastName + " " + mFirstName;
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
    return DB->getModelHandler(KModelHdlPerson);
}

const QString Person::exportTemplatePath() const
{
    return FileCtl::getPrebuiltDataFilePath(KPrebuiltPersonInfoTemplateFileName);
}

const QStringList Person::getListExportKeyWord() const
{
    traced;

//    fields.append(KExportFieldImgPath);
//    fields.append(KExportFieldFullName);
//    fields.append(KExportFieldBirthday);
//    fields.append(KExportFieldBirthplace);
//    fields.append(KExportFieldHollyName);
//    fields.append(KExportFieldNationality);
//    fields.append(KExportFieldEthnic);
//    fields.append(KExportFieldIDcard);
//    fields.append(KExportFieldIDcardIssueDate);
//    fields.append(KExportFieldIDcardIssuer);
//    fields.append(KExportFieldStatus);
//    fields.append(KExportFieldRetireDate);

//    fields.append(KExportFieldRetirePlace); //"retire_place";
//    fields.append(KExportFieldDeadDate); //"dead_date";
//    fields.append(KExportFieldDeadPlace); //"dead_place";
//    fields.append(KExportFieldCountry); //"country";
//    fields.append(KExportFieldProvince); //"province";
//    fields.append(KExportFieldAddress); //"address";
//    fields.append(KExportFieldChurchAddress); //"church_addr";
//    fields.append(KExportFieldTel); //"telephone";
//    fields.append(KExportFieldEmail); //"email";
//    fields.append(KExportFieldOtherContant); //"othercontact";
//    fields.append(KExportFieldEdu); //"education";
//    fields.append(KExportFieldSpeciaist); //"specialist";
//    fields.append(KExportFieldWork); //"work";
//    fields.append(KExportFieldWorkHistory); //"work_history";
//    fields.append(KExportFieldCommunity); //"community";
//    fields.append(KExportFieldCommunityHistory); //"community_history";
//    fields.append(KExportFieldDad); //"dad";
//    fields.append(KExportFieldDadBirthday); //"dad_birthday";
//    fields.append(KExportFieldDadAddr); //"dad_addr";
//    fields.append(KExportFieldMom); //"mom";
//    fields.append(KExportFieldMomBirthday); //"mom_birthday";
//    fields.append(KExportFieldMomAddr); //"mom_addr";
//    fields.append(KExportFieldFamilyHistory); //"family_history";
//    fields.append(KExportFieldFamilyContact); //"family_contact";
//    fields.append(KExportFieldChristenDate); //"christen_date";
//    fields.append(KExportFieldChristenPlace); //"christen_place";
//    fields.append(KExportFieldEucharistDate); //"eucharist_date";
//    fields.append(KExportFieldEucharistPlace); //"eucharist_place";
//    fields.append(KExportFieldHollyDate); //"holly_date";
//    fields.append(KExportFieldHollyPlace); //"holly_place";
//    fields.append(KExportFieldCourse); //"course";
//    fields.append(KExportFieldJoinDate); //"join_date";
//    fields.append(KExportFieldJoinPIC); //"join_pic";
//    fields.append(KExportFieldPreTrainDate); //"pre_train_date";
//    fields.append(KExportFieldPreTrainPIC); //"pre_train_pic";
//    fields.append(KExportFieldTrainDate); //"train_date";
//    fields.append(KExportFieldTrainPIC); //"train_pic";
//    fields.append(KExportFieldVowsDate); //"vows_date";
//    fields.append(KExportFieldVowsCEO); //"vows_ceo";
//    fields.append(KExportFieldEternalVowsDate); //"eternal_vows_date";
//    fields.append(KExportFieldEternalVowsCEO); //"eternal_vows_ceo";
//    fields.append(KExportFieldEternalVowsPIC); //"eternal_vows_pic";
//    fields.append(KExportFieldBankDate); //"bank_date";
//    fields.append(KExportFieldBankPlace); //"bank_place";
//    fields.append(KExportFieldGoldenDate); //"golden_date";
//    fields.append(KExportFieldGoldenPlace); //"golden_place";
//    fields.append(KExportFieldEternalDate); //"eternal_date";
//    fields.append(KExportFieldEternalPlace); //"eternal_place";

    return mExportFields.keys();
}

ErrCode Person::getExportDataString(const QString &keyword, QString *data) const
{
    ErrCode ret = ErrNone;
    traced;
    logd("keyword %s", keyword.toStdString().c_str());
    if (mExportFields.contains(keyword)){
        std::function<QString()> func = mExportFields.value(keyword);
        if (func != nullptr) *data = func();
    }
    // TODO: raise exception when error occur???

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

ErrCode Person::onImportItem(int importFileType, const QString &keyword, const QString &value, quint32 idx, void* tag)
{
    traced;
    ErrCode ret = ErrNone;
    logd("importFileType %d", importFileType);

    // TODO: raise exception when error occur???
    logd("keyword %s", keyword.toStdString().c_str());
    if (mImportFields.contains(keyword)){
        std::function<void(const QString& value)> func = mImportFields.value(keyword);
        if (func != nullptr) func(value);
    }

    tracedr(ret);
    return ret;
}

qint64 Person::feastDay() const
{
    return mFeastDay;
}

void Person::setFeastDay(qint64 newFeastDay)
{
    mFeastDay = newFeastDay;
}

void Person::setFeastDay(const QString &newFeastDay)
{
    setFeastDay(Utils::dateFromString(newFeastDay, DATE_FORMAT_MD));
}

const QString &Person::eucharistPlace() const
{
    return mEucharistPlace;
}

void Person::setEucharistPlace(const QString &newEucharistPlace)
{
    mEucharistPlace = newEucharistPlace;
}

qint64 Person::eucharistDate() const
{
    return mEucharistDate;
}

void Person::setEucharistDate(qint64 newEucharistDate)
{
    mEucharistDate = newEucharistDate;
}

const QString &Person::otherContact() const
{
    return mOtherContact;
}

void Person::setOtherContact(const QString &newOtherContact)
{
    mOtherContact = newOtherContact;
}

const QHash<QString, QString> &Person::workUidList() const
{
    return mWorkUidList;
}

void Person::setWorkUidList(const QHash<QString, QString> &newWorkUidList)
{
    mWorkUidList = newWorkUidList;
}

const QHash<QString, QString> &Person::educationUidList() const
{
    return mEducationUidList;
}

void Person::setEducationUidList(const QHash<QString, QString> &newEducationUidList)
{
    mEducationUidList = newEducationUidList;
}

const QList<Work *> &Person::workList() const
{
    return mWorkList;
}

void Person::setWorkList(const QList<Work *> &newWorkList)
{
    mWorkList = newWorkList;
}

const QList<Education *> &Person::educationList() const
{
    return mEducationList;
}

void Person::setEducationList(const QList<Education *> &newEducationList)
{
    mEducationList = newEducationList;
}

const QStringList &Person::eventUidList() const
{
    return mEventUidList;
}

void Person::setEventUidList(const QStringList &newEventUidList)
{
    mEventUidList = newEventUidList;
}

const QString &Person::imgId() const
{
    return mImgId;
}

void Person::setImgId(const QString &newImgId)
{
    mImgId = newImgId;
}

const QString &Person::deadPlace() const
{
    return mDeadPlace;
}

void Person::setDeadPlace(const QString &newDeadPlace)
{
    mDeadPlace = newDeadPlace;
}

qint64 Person::deadDate() const
{
    return mDeadDate;
}

void Person::setDeadDate(qint64 newDeadDate)
{
    mDeadDate = newDeadDate;
}

const QString &Person::retirePlace() const
{
    return mRetirePlace;
}

void Person::setRetirePlace(const QString &newRetirePlace)
{
    mRetirePlace = newRetirePlace;
}

qint64 Person::retireDate() const
{
    return mRetireDate;
}

void Person::setRetireDate(qint64 newRetireDate)
{
    mRetireDate = newRetireDate;
}

const QString &Person::statusName() const
{
    return mStatusName;
}

void Person::setStatusName(const QString &newStatusName)
{
    mStatusName = newStatusName;
}

const QString &Person::statusUid() const
{
    return mStatusUid;
}

void Person::setStatusUid(const QString &newStatusUid)
{
    mStatusUid = newStatusUid;
}

const QString &Person::eternalPlace() const
{
    return mEternalPlace;
}

void Person::setEternalPlace(const QString &newEternalPlace)
{
    mEternalPlace = newEternalPlace;
}

qint64 Person::eternalDate() const
{
    return mEternalDate;
}

void Person::setEternalDate(qint64 newEternalDate)
{
    mEternalDate = newEternalDate;
}

const QString &Person::goldenPlace() const
{
    return mGoldenPlace;
}

void Person::setGoldenPlace(const QString &newGoldenPlace)
{
    mGoldenPlace = newGoldenPlace;
}

qint64 Person::goldenDate() const
{
    return mGoldenDate;
}

void Person::setGoldenDate(qint64 newGoldenDate)
{
    mGoldenDate = newGoldenDate;
}

const QString &Person::bankPlace() const
{
    return mBankPlace;
}

void Person::setBankPlace(const QString &newBankPlace)
{
    mBankPlace = newBankPlace;
}

qint64 Person::bankDate() const
{
    return mBankDate;
}

void Person::setBankDate(qint64 newBankDate)
{
    mBankDate = newBankDate;
}

const QString &Person::eternalVowsPICName() const
{
    return mEternalVowsPICName;
}

void Person::setEternalVowsPICName(const QString &newEternalVowsName)
{
    mEternalVowsPICName = newEternalVowsName;
}

const QString &Person::eternalVowsPICUid() const
{
    return mEternalVowsPICUid;
}

void Person::setEternalVowsPICUid(const QString &newEternalVowsPICUid)
{
    mEternalVowsPICUid = newEternalVowsPICUid;
}

const QString &Person::eternalVowsCEOName() const
{
    return mEternalVowsCEOName;
}

void Person::setEternalVowsCEOName(const QString &newEternalVowsCEOName)
{
    mEternalVowsCEOName = newEternalVowsCEOName;
}

const QString &Person::eternalVowsCEOUid() const
{
    return mEternalVowsCEOUid;
}

void Person::setEternalVowsCEOUid(const QString &newEternalVowsCEOUid)
{
    mEternalVowsCEOUid = newEternalVowsCEOUid;
}

qint64 Person::eternalVowsDate() const
{
    return mEternalVowsDate;
}

void Person::setEternalVowsDate(qint64 newEternalVowsDate)
{
    mEternalVowsDate = newEternalVowsDate;
}

qint64 Person::vowsDate() const
{
    return mVowsDate;
}

void Person::setVowsDate(qint64 newVowsDate)
{
    mVowsDate = newVowsDate;
}

const QString &Person::vowsCEOName() const
{
    return mVowsCEOName;
}

void Person::setVowsCEOName(const QString &newVowsCEOName)
{
    mVowsCEOName = newVowsCEOName;
}

const QString &Person::vowsCEOUid() const
{
    return mVowsCEOUid;
}

void Person::setVowsCEOUid(const QString &newVowsCEOUid)
{
    mVowsCEOUid = newVowsCEOUid;
}

const QString &Person::trainPICName() const
{
    return mTrainPICName;
}

void Person::setTrainPICName(const QString &newTrainPICName)
{
    mTrainPICName = newTrainPICName;
}

const QString &Person::trainPICUid() const
{
    return mTrainPICUid;
}

void Person::setTrainPICUid(const QString &newTrainPICUid)
{
    mTrainPICUid = newTrainPICUid;
}

const QString &Person::preTrainPICName() const
{
    return mPreTrainPICName;
}

void Person::setPreTrainPICName(const QString &newPreTrainPICName)
{
    mPreTrainPICName = newPreTrainPICName;
}

const QString &Person::preTrainPICUid() const
{
    return mPreTrainPICUid;
}

void Person::setPreTrainPICUid(const QString &newPreTrainPICUid)
{
    mPreTrainPICUid = newPreTrainPICUid;
}

qint64 Person::preTrainJoinDate() const
{
    return mPreTrainJoinDate;
}

void Person::setPreTrainJoinDate(qint64 newPreTrainJoinDate)
{
    mPreTrainJoinDate = newPreTrainJoinDate;
}

const QString &Person::joinPICName() const
{
    return mJoinPICName;
}

void Person::setJoinPICName(const QString &newJoinPICName)
{
    mJoinPICName = newJoinPICName;
}

const QString &Person::joinPICUid() const
{
    return mJoinPICUid;
}

void Person::setJoinPICUid(const QString &newJoinPICUid)
{
    mJoinPICUid = newJoinPICUid;
}

qint64 Person::joinDate() const
{
    return mJoinDate;
}

void Person::setJoinDate(qint64 newJoinDate)
{
    mJoinDate = newJoinDate;
}

const QString &Person::familyContact() const
{
    return mFamilyContact;
}

void Person::setFamilyContact(const QString &newFamilyContact)
{
    mFamilyContact = newFamilyContact;
}

qint64 Person::hollyDate() const
{
    return mHollyDate;
}

void Person::setHollyDate(qint64 newHollyDate)
{
    mHollyDate = newHollyDate;
}

const QString &Person::hollyPlace() const
{
    return mHollyPlace;
}

void Person::setHollyPlace(const QString &newHollyPlace)
{
    mHollyPlace = newHollyPlace;
}

const QString &Person::christenPlace() const
{
    return mChristenPlace;
}

void Person::setChristenPlace(const QString &newChristenPlace)
{
    mChristenPlace = newChristenPlace;
}

const QString &Person::familyHistory() const
{
    return mFamilyHistory;
}

void Person::setFamilyHistory(const QString &newFamilyHistory)
{
    mFamilyHistory = newFamilyHistory;
}

const QString &Person::momAddr() const
{
    return mMomAddr;
}

void Person::setMomAddr(const QString &newMomAddr)
{
    mMomAddr = newMomAddr;
}

qint64 Person::momBirthday() const
{
    return mMomBirthday;
}

void Person::setMomBirthday(qint64 newMomBirthday)
{
    mMomBirthday = newMomBirthday;
}

const QString &Person::momName() const
{
    return mMomName;
}

void Person::setMomName(const QString &newMomName)
{
    mMomName = newMomName;
}

const QString &Person::dadAddr() const
{
    return mDadAddr;
}

void Person::setDadAddr(const QString &newDadAddr)
{
    mDadAddr = newDadAddr;
}

qint64 Person::dadBirthday() const
{
    return mDadBirthday;
}

void Person::setDadBirthday(qint64 newDadBirthday)
{
    mDadBirthday = newDadBirthday;
}

const QString &Person::dadName() const
{
    return mDadName;
}

void Person::setDadName(const QString &newDadName)
{
    mDadName = newDadName;
}

const QStringList &Person::tel() const
{
    return mTel;
}

void Person::setTel(const QStringList &newTel)
{
    mTel = newTel;
}

void Person::setTel(const QString &newTel)
{
    mTel = newTel.split(';');
}

const QStringList &Person::email() const
{
    return mEmail;
}

void Person::setEmail(const QStringList &newEmail)
{
    mEmail = newEmail;
}

void Person::setEmail(const QString &newEmail)
{
    mEmail = newEmail.split(';');
}

const QString &Person::churchAddr() const
{
    return mChurchAddr;
}

void Person::setChurchAddr(const QString &newChurchAddr)
{
    mChurchAddr = newChurchAddr;
}

const QString &Person::addr() const
{
    return mAddr;
}

void Person::setAddr(const QString &newAddr)
{
    mAddr = newAddr;
}

const QString &Person::provinceName() const
{
    return mProvinceName;
}

void Person::setProvinceName(const QString &newProvinceName)
{
    mProvinceName = newProvinceName;
}

const QString &Person::provinceUid() const
{
    return mProvinceUid;
}

void Person::setProvinceUid(const QString &newProvinceUid)
{
    mProvinceUid = newProvinceUid;
}

const QString &Person::countryName() const
{
    return mCountryName;
}

void Person::setCountryName(const QString &newCountryName)
{
    mCountryName = newCountryName;
}

const QString &Person::countryUid() const
{
    return mCountryUid;
}

void Person::setCountryUid(const QString &newCountryUid)
{
    mCountryUid = newCountryUid;
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

void Person::setIdCardIssueDate(const QString &newIdCardIssueDate)
{
    mIdCardIssueDate = Utils::dateFromString(newIdCardIssueDate);
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
