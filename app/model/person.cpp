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
#include "dbpersonmodelhandler.h"

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
    traced;
    // do nothing, as uild will be built later
}

QString Person::buildUid(const QString *seed)
{
    traced;
    QString uid;
    QString uidName = QString("%1_%2_%3").arg(getFullName()).arg(birthday()).arg(birthPlace());
    if (seed != nullptr) {
        uidName += "_" + *seed;
    }
    logd("uidName: %s", uidName.toStdString().c_str());
    uid = Utils::UidFromName(uidName, UidNameConvertType::HASH_NAME);
    logd("uid: %s", uid.toStdString().c_str());
    return uid;
}

QString Person::modelName() const
{
    return KModelNamePerson;
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
    mExportFields.insert(KItemImgPath, [this](){
        QString imgPath = this->imgPath();
        if (imgPath.isEmpty()){
            imgPath = ":/icon/icon/unknown.png";
        }
        return imgPath;
    });
    mExportFields.insert(KItemFullName, [this](){
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
    mExportFields.insert(KItemBirthday, [this](){
        return Utils::date2String(this->birthday());});
    mExportFields.insert(KItemBirthplace, [this](){
        return this->birthPlace();
    });
    mExportFields.insert(KItemHollyName, [this](){
        return this->hollyName();
    });
    mExportFields.insert(KItemFeastDay, [this](){
        return Utils::date2String(this->feastDay(), DATE_FORMAT_MD);
    });
    mExportFields.insert(KItemNationality, [this](){
        return this->nationalityName();
    });
    mExportFields.insert(KItemEthnic, [this](){
        return this->ethnicName();
    });
    mExportFields.insert(KItemIDcard, [this](){
        return this->idCard();
    });
    mExportFields.insert(KItemIDcardIssueDate,  [this](){
        return Utils::date2String(this->idCardIssueDate(), DATE_FORMAT_YMD);
    });
    mExportFields.insert(KItemIDcardIssuer,  [this](){
        return this->idCardIssuePlace();
    });
    mExportFields.insert(KItemStatus, nullptr);
    mExportFields.insert(KItemRetireDate, nullptr);
    mExportFields.insert(KItemRetirePlace, nullptr); //"retire_place";
    mExportFields.insert(KItemDeadDate, nullptr); //"dead_date";
    mExportFields.insert(KItemDeadPlace, nullptr); //"dead_place";
    mExportFields.insert(KItemCountry, nullptr); //"country";
    mExportFields.insert(KItemProvince, nullptr); //"province";
    mExportFields.insert(KItemAddress, [this](){
        return this->addr();
    }); //"address";
    mExportFields.insert(KItemChurchAddress, [this](){
        return this->churchAddr();
    }); //"church_addr";
    mExportFields.insert(KItemTel, [this](){
        return this->tel().join(";");
    }); //"telephone";
    mExportFields.insert(KItemEmail, [this](){
        return this->email().join(";");
    }); //"email";
    mExportFields.insert(KItemOtherContant, nullptr); //"othercontact";
    mExportFields.insert(KItemEdu, nullptr); //"education";
    mExportFields.insert(KItemEduUid, [this](){
        return this->eduUid();
    }); //"edu uid";
    mExportFields.insert(KItemSpeciaist,  [this](){
        logd("get specialist info to export");
        DbPersonModelHandler* hdl = dynamic_cast<DbPersonModelHandler*>(getDbModelHandler());
        QList<DbModel*> list = hdl->getSpecialistList(this->uid());
        QString specialist;
        if (list.count() > 0) {
            foreach (DbModel* item, list) {
                if (!specialist.isEmpty()){
                    specialist += ",";
                }
                item->dump();
                specialist += item->name();
            }
        }
        logd("specialist %s", specialist.toStdString().c_str());
        // TODO: free memory????
        return specialist;
    }); //"specialist";
    mExportFields.insert(KItemSpeciaistUid, [this](){
        QStringList list = this->specialistUidList();
        if (list.count() > 0)
            return list.join(",");
        else
            return QString();
    }); //"specialist uid";
    mExportFields.insert(KItemWork, nullptr); //"work";
    mExportFields.insert(KItemWorkHistory, nullptr); //"work_history";
    mExportFields.insert(KItemCommunity, nullptr); //"community";
    mExportFields.insert(KItemCommunityHistory, nullptr); //"community_history";
    mExportFields.insert(KItemDad, nullptr); //"dad";
    mExportFields.insert(KItemDadBirthday, nullptr); //"dad_birthday";
    mExportFields.insert(KItemDadAddr, nullptr); //"dad_addr";
    mExportFields.insert(KItemMom, nullptr); //"mom";
    mExportFields.insert(KItemMomBirthday, nullptr); //"mom_birthday";
    mExportFields.insert(KItemMomAddr, nullptr); //"mom_addr";
    mExportFields.insert(KItemFamilyHistory, nullptr); //"family_history";
    mExportFields.insert(KItemFamilyContact, nullptr); //"family_contact";
    mExportFields.insert(KItemChristenDate, nullptr); //"christen_date";
    mExportFields.insert(KItemChristenPlace, nullptr); //"christen_place";
    mExportFields.insert(KItemEucharistDate, nullptr); //"eucharist_date";
    mExportFields.insert(KItemEucharistPlace, nullptr); //"eucharist_place";
    mExportFields.insert(KItemHollyDate, nullptr); //"holly_date";
    mExportFields.insert(KItemHollyPlace, nullptr); //"holly_place";
    mExportFields.insert(KItemCourse, nullptr); //"course";
    mExportFields.insert(KItemJoinDate, nullptr); //"join_date";
    mExportFields.insert(KItemJoinPIC, nullptr); //"join_pic";
    mExportFields.insert(KItemPreTrainDate, nullptr); //"pre_train_date";
    mExportFields.insert(KItemPreTrainPIC, nullptr); //"pre_train_pic";
    mExportFields.insert(KItemTrainDate, nullptr); //"train_date";
    mExportFields.insert(KItemTrainPIC, nullptr); //"train_pic";
    mExportFields.insert(KItemVowsDate, nullptr); //"vows_date";
    mExportFields.insert(KItemVowsCEO, nullptr); //"vows_ceo";
    mExportFields.insert(KItemEternalVowsDate, nullptr); //"eternal_vows_date";
    mExportFields.insert(KItemEternalVowsCEO, nullptr); //"eternal_vows_ceo";
    mExportFields.insert(KItemEternalVowsPIC, nullptr); //"eternal_vows_pic";
    mExportFields.insert(KItemBankDate, nullptr); //"bank_date";
    mExportFields.insert(KItemBankPlace, nullptr); //"bank_place";
    mExportFields.insert(KItemGoldenDate, nullptr); //"golden_date";
    mExportFields.insert(KItemGoldenPlace, nullptr); //"golden_place";
    mExportFields.insert(KItemEternalDate, nullptr); //"eternal_date";
    mExportFields.insert(KItemEternalPlace, nullptr); //"eternal_place";

}

void Person::initImportFields()
{
    traced;
    // TODO: check fields like holly name, country, etc. and mark invalid field???

    mImportFields.insert(KItemImgPath, [this](const QString& value){this->setImgPath(value);});
    mImportFields.insert(KItemFullName, [this](const QString& value){
        this->setNameFromFullName(value);
    });
    mImportFields.insert(KItemBirthday, [this](const QString& value){
        this->setBirthday(value);
    });
    mImportFields.insert(KItemBirthplace, [this](const QString& value){
        this->setBirthPlace(value);
    });
    mImportFields.insert(KItemHollyName, [this](const QString& value){this->setHollyName(value);});
    mImportFields.insert(KItemFeastDay, [this](const QString& value){this->setFeastDay(value);});
    mImportFields.insert(KItemNationality, [this](const QString& value){this->setNationalityName(value);});
    mImportFields.insert(KItemEthnic, [this](const QString& value){this->setEthnicName(value);});
    mImportFields.insert(KItemIDcard, [this](const QString& value){
        this->setIdCard(value);
    });
    mImportFields.insert(KItemIDcardIssueDate, [this](const QString& value){
        this->setIdCardIssueDate(value);
    });
    mImportFields.insert(KItemIDcardIssuer, [this](const QString& value){
        this->setIdCardIssuePlace(value);
    });
    mImportFields.insert(KItemStatus, nullptr); // TODO: implement this
    mImportFields.insert(KItemRetireDate,  [this](const QString& value){
        this->setRetireDate(value);
    });
    mImportFields.insert(KItemRetirePlace, [this](const QString& value){
        this->setRetirePlace(value);
    }); //"retire_place";
    mImportFields.insert(KItemDeadDate, [this](const QString& value){
        this->setDeadDate(value);
    }); //"dead_date";
    mImportFields.insert(KItemDeadPlace, [this](const QString& value){
        this->setDeadPlace(value);
    }); //"dead_place";
    mImportFields.insert(KItemCountry, [this](const QString& value){
        this->setCountryName(value);
    }); //"country";
    mImportFields.insert(KItemProvince, [this](const QString& value){
        this->setProvinceName(value);
    }); //"province";
    mImportFields.insert(KItemAddress, [this](const QString& value){
        this->setAddr(value);
    }); //"address";
    mImportFields.insert(KItemChurchAddress, [this](const QString& value){
        this->setChurchAddr(value);
    }); //"church_addr";
    mImportFields.insert(KItemTel, [this](const QString& value){
        this->setTel(value);
    }); //"telephone";
    mImportFields.insert(KItemEmail, [this](const QString& value){
        this->setEmail(value);
    }); //"email";
    mImportFields.insert(KItemOtherContant, [this](const QString& value){
        this->setOtherContact(value);
    }); //"othercontact";
    mImportFields.insert(KItemEdu, [this](const QString& value){
        this->setEduName(value);
    }); //"education";
    mImportFields.insert(KItemEduUid, [this](const QString& value){
        this->setEduUid(value);
    }); //"education";
    mImportFields.insert(KItemSpeciaist, [this](const QString& value){
        this->setSpecialistNames(value);
    }); //"specialist";
    mImportFields.insert(KItemWork, nullptr); //"work"; // TODO
    mImportFields.insert(KItemWorkHistory, nullptr); //"work_history"; // TODO
    mImportFields.insert(KItemCommunity, nullptr); //"community"; // TODO
    mImportFields.insert(KItemCommunityHistory, nullptr); //"community_history"; // TODO
    mImportFields.insert(KItemDad, [this](const QString& value){
        this->setDadName(value);
    }); //"dad";
    mImportFields.insert(KItemDadBirthday, [this](const QString& value){
        this->setDadBirthday(value);
    }); //"dad_birthday";
    mImportFields.insert(KItemDadAddr,  [this](const QString& value){
        this->setDadAddr(value);
    }); //"dad_addr";
    mImportFields.insert(KItemMom,  [this](const QString& value){
        this->setMomName(value);
    }); //"mom";
    mImportFields.insert(KItemMomBirthday,  [this](const QString& value){
        this->setMomBirthday(value);
    }); //"mom_birthday";
    mImportFields.insert(KItemMomAddr, [this](const QString& value){
        this->setMomAddr(value);
    }); //"mom_addr";
    mImportFields.insert(KItemFamilyHistory, [this](const QString& value){
        this->setFamilyHistory(value);
    }); //"family_history";
    mImportFields.insert(KItemFamilyContact, [this](const QString& value){
        this->setFamilyContact(value);
    }); //"family_contact";
    mImportFields.insert(KItemChristenDate, [this](const QString& value){
        this->setChristenDate(value);
    }); //"christen_date";
    mImportFields.insert(KItemChristenPlace, [this](const QString& value){
        this->setChristenPlace(value);
    }); //"christen_place";
    mImportFields.insert(KItemEucharistDate, [this](const QString& value){
        this->setEucharistDate(value);
    }); //"eucharist_date";
    mImportFields.insert(KItemEucharistPlace, [this](const QString& value){
        this->setEucharistPlace(value);
    }); //"eucharist_place";
    mImportFields.insert(KItemHollyDate, [this](const QString& value){
        this->setHollyDate(value);
    }); //"holly_date";
    mImportFields.insert(KItemHollyPlace, [this](const QString& value){
        this->setHollyPlace(value);
    }); //"holly_place";
    mImportFields.insert(KItemCourse, [this](const QString& value){
        this->setCourse(value);
    }); //"course";
    mImportFields.insert(KItemJoinDate, [this](const QString& value){
        this->setJoinDate(value);
    }); //"join_date";
    mImportFields.insert(KItemJoinPIC, nullptr); //"join_pic";
    mImportFields.insert(KItemPreTrainDate,  [this](const QString& value){
        this->setPreTrainJoinDate(value);
    }); //"pre_train_date";
    mImportFields.insert(KItemPreTrainPIC, nullptr); //"pre_train_pic";
    mImportFields.insert(KItemTrainDate, [this](const QString& value){
        this->setTrainJoinDate(value);
    }); //"train_date";
    mImportFields.insert(KItemTrainPIC, nullptr); //"train_pic";
    mImportFields.insert(KItemVowsDate,  [this](const QString& value){
        this->setVowsDate(value);
    }); //"vows_date";
    mImportFields.insert(KItemVowsCEO, nullptr); //"vows_ceo";
    mImportFields.insert(KItemEternalVowsDate,  [this](const QString& value){
        this->setEternalVowsDate(value);
    }); //"eternal_vows_date";
    mImportFields.insert(KItemEternalVowsCEO, nullptr); //"eternal_vows_ceo";
    mImportFields.insert(KItemEternalVowsPIC, nullptr); //"eternal_vows_pic";
    mImportFields.insert(KItemBankDate,  [this](const QString& value){
        this->setBankDate(value);
    }); //"bank_date";
    mImportFields.insert(KItemBankPlace,  [this](const QString& value){
        this->setBankPlace(value);
    }); //"bank_place";
    mImportFields.insert(KItemGoldenDate,  [this](const QString& value){
        this->setGoldenDate(value);
    }); //"golden_date";
    mImportFields.insert(KItemGoldenPlace,  [this](const QString& value){
        this->setGoldenPlace(value);
    }); //"golden_place";
    mImportFields.insert(KItemEternalDate,  [this](const QString& value){
        this->setEternalDate(value);
    }); //"eternal_date";
    mImportFields.insert(KItemEternalPlace,  [this](const QString& value){
        this->setEternalPlace(value);
    }); //"eternal_place";
}

ErrCode Person::validate()
{
    traced;
    // TODO: cached value???
    ErrCode ret = ErrNone;
    DbModelHandler* personHdlr = this->getDbModelHandler();
    if (personHdlr == nullptr) {
        loge("Invalid Person handler");
        ret = ErrFailed;
    }
    if (ret == ErrNone && personHdlr->exist(this)) {
        loge("Person already existed");
        ret = ErrExisted;
    }

    if (ret == ErrNone) {
        ret = checkAndUpdateSaintListFromHollyName();
        if (ret != ErrNone) {
            appendValidateResult(KItemHollyName, ErrNotFound);
            appendValidateMsg(QString("Saint %1 not found").arg(hollyName()));
            ret = ErrInvalidData;
        }
    }
    tracedr(ret);
    return ret;
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
    // TODO: this is vietnamese style only, how about name in Englist style???
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

void Person::setChristenDate(const QString &newChristenDate,
                             const QString& format)
{
    mChristenDate = Utils::dateFromString(newChristenDate,format);
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

ErrCode Person::prepare2Save()
{
    traced;
    DbModelHandler* saintHdlr = nullptr;
    ErrCode ret = DbModel::prepare2Save();
    if (ret == ErrNone) {
        ret = checkAndUpdateSaintListFromHollyName();
    }
    tracedr(ret);
    return ret;
}

const QString &Person::specialistInfo() const
{
    return mSpecialistInfo;
}

void Person::setSpecialistInfo(const QString &newSpecialistInfo)
{
    mSpecialistInfo = newSpecialistInfo;
}

const QString &Person::eduDetail() const
{
    return mEduDetail;
}

void Person::setEduDetail(const QString &newEduDetail)
{
    mEduDetail = newEduDetail;
}

const QString &Person::communityName() const
{
    return mCommunityName;
}

void Person::setCommunityName(const QString &newCommunityName)
{
    mCommunityName = newCommunityName;
}

const QString &Person::communityUid() const
{
    return mCommunityUid;
}

void Person::setCommunityUid(const QString &newCommunityUid)
{
    mCommunityUid = newCommunityUid;
    markItemAsModified(KItemCommunity);
}

const QString &Person::departName() const
{
    return mDepartName;
}

void Person::setDepartName(const QString &newDepartName)
{
    mDepartName = newDepartName;
}

const QString &Person::departUid() const
{
    return mDepartUid;
}

void Person::setDepartUid(const QString &newDepartUid)
{
    mDepartUid = newDepartUid;
}

const QString &Person::areaName() const
{
    return mAreaName;
}

void Person::setAreaName(const QString &newAreaName)
{
    mAreaName = newAreaName;
}

const QString &Person::areaUid() const
{
    return mAreaUid;
}

void Person::setAreaUid(const QString &newAreaUid)
{
    mAreaUid = newAreaUid;
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
    logd("- FullName %s", getFullName().toStdString().c_str());
    logd("- FirstName %s", firstName().toStdString().c_str());
    logd("- LastName %s", lastName().toStdString().c_str());
    logd("- HollyName %s", hollyName().toStdString().c_str());
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

void Person::setFeastDay(const QString &newFeastDay,
                         const QString& format)
{
    setFeastDay(Utils::dateFromString(newFeastDay, format));
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

void Person::setEucharistDate(const QString &newEucharistDate, const QString &format)
{
    setEucharistDate(Utils::dateFromString(newEucharistDate, format));
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

void Person::setDeadDate(const QString &newDeadDate, const QString &format)
{
    setDeadDate(Utils::dateFromString(newDeadDate, format));
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

void Person::setRetireDate(const QString &newRetireDate, const QString &format)
{
    traced;
    setRetireDate(Utils::dateFromString(newRetireDate, format));
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

void Person::setEternalDate(const QString &newEternalDate, const QString &format)
{
    setEternalDate(Utils::dateFromString(newEternalDate, format));
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

void Person::setGoldenDate(const QString &newGoldenDate, const QString &format)
{
    setGoldenDate(Utils::dateFromString(newGoldenDate, format));
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

void Person::setBankDate(const QString &newBankDate, const QString &format)
{
    setBankDate(Utils::dateFromString(newBankDate, format));
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

void Person::setEternalVowsDate(const QString &newEternalVowsDate, const QString &format)
{
    setEternalVowsDate(Utils::dateFromString(newEternalVowsDate, format));
}

qint64 Person::vowsDate() const
{
    return mVowsDate;
}

void Person::setVowsDate(qint64 newVowsDate)
{
    mVowsDate = newVowsDate;
}

void Person::setVowsDate(const QString &newVowsDate, const QString &format)
{
    setVowsDate(Utils::dateFromString(newVowsDate, format));
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

void Person::setPreTrainJoinDate(const QString &newPreTrainJoinDate, const QString &format)
{
    setPreTrainJoinDate(Utils::dateFromString(newPreTrainJoinDate, format));
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

void Person::setJoinDate(const QString &newJoinDate, const QString &format)
{
    setJoinDate(Utils::dateFromString(newJoinDate, format));
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

void Person::setHollyDate(const QString &newHollyDate, const QString &format)
{
    setHollyDate(Utils::dateFromString(newHollyDate, format));
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

void Person::setMomBirthday(const QString &newMomBirthday, const QString &format)
{
    setMomBirthday(Utils::dateFromString(newMomBirthday, format));
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

void Person::setDadBirthday(const QString &newDadBirthday, const QString &format)
{
    setDadBirthday(Utils::dateFromString(newDadBirthday, format));
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

void Person::setSpecialistNames(const QString &newSpecialists, const QString &split)
{
    mSpecialistNameList = newSpecialists.split(split); // TODO: check to remove redudant
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

ErrCode Person::checkAndUpdateSaintListFromHollyName()
{
    traced;
    DbModelHandler* saintHdlr = nullptr;
    ErrCode ret = ErrNone;
    saintHdlr = dynamic_cast<DbModelHandler*>(DB->getModelHandler(KModelHdlSaint));
    if (saintHdlr == nullptr) {
        loge("Invalid Saint handler");
        ret = ErrFailed;
    }
    if (ret == ErrNone) {
        if (mSaintUidList.empty() && !mHollyName.isEmpty()) {
            QStringList names = mHollyName.split(",");
            DbModel* model = nullptr;
            QString hollyNotFound;
            foreach (QString name, names) {
                logd("Check saint name '%s'", name.toStdString().c_str());
                model = saintHdlr->getByName(name.trimmed());
                if (model) {
                    logd("update saint uid %s", model->uid().toStdString().c_str());
                    addSaintUid(model->uid());
                } else {
                    loge("Saint '%s' not found in db", name.toStdString().c_str());
                    ret = ErrNotFound;
                    break;
                }
            }
        }
    }
    tracedr(ret);
    return ret;
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
    if (!mSaintUidList.contains(saint))
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

void Person::setTrainJoinDate(const QString &newTrainJoinDate, const QString &format)
{
    setTrainJoinDate(Utils::dateFromString(newTrainJoinDate, format));
}

const QString &Person::imgPath() const
{
    return mImgPath;
}

void Person::setImgPath(const QString &newImgPath)
{
    mImgPath = newImgPath;
}
