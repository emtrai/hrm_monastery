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
#include "saintctl.h"
#include "countryctl.h"
#include "ethnicctl.h"
#include "eductl.h"
#include "coursectl.h"
#include "provincectl.h"
#include "specialistctl.h"
#include "workctl.h"
#include "communityctl.h"

#include "config.h"

// TODO: show person code instead of uid?? uid should use for debug only?
#define EXPORT_PERSON_INFO_COMMON_IMPL(item, uid, name) \
    logd("export %s", item.toStdString().c_str());\
    logd("uid %s", uid.toStdString().c_str());\
    logd("name %s", name.toStdString().c_str());\
    QString val; \
    if (!uid.isEmpty()) { \
        val = QString("%1 (%2...)").arg(name, uid.left(8)); \
        logd("PIC %s", val.toStdString().c_str()); \
    } \
    return val;


Person::Person():DbModel()
    , mChristenDate(0)
    , mBirthday(0)
    , mFeastDay(0)
    , mIdCardIssueDate(0)
    , mDadBirthday(0)
    , mMomBirthday(0)
    , mEucharistDate(0)
    , mHollyDate(0)
    , mJoinDate(0)
    , mPreTrainJoinDate(0)
    , mTrainJoinDate(0)
    , mVowsDate(0)
    , mEternalVowsDate(0)
    , mBankDate(0)
    , mGoldenDate(0)
    , mEternalDate(0)
    , mRetireDate(0)
    , mDeadDate(0)
{
    init();
}


Person::~Person()
{
    traced;
    // TODO: free resource
}

void Person::clone(const DbModel *model)
{
    traced;
    DbModel::clone(model);
    Person* per = (Person*) model;
    mPersonCode = per->personCode();
    mFirstName = per->firstName();
    mLastName = per->lastName();

    mBirthday = per->birthday();
    mBirthPlace = per->birthPlace();

    mHollyName = per->hollyName();
    mSaintUidList = per->saintUidList();
    mSaintUidNameMap.insert(per->saintUidNameMap());

    mImgId = per->imgId();
    mImgPath = per->imgPath();

    mNationalityUid = per->nationalityUid();
    mNationalityName = per->nationalityName();

    mEthnicUid = per->ethnicUid();
    mEthnicName = per->ethnicName();

    mIdCard = per->idCard();
    mIdCardIssuePlace = per->idCardIssuePlace();
    mIdCardIssueDate = per->idCardIssueDate();

    mEduUid = per->eduUid();
    mEduName = per->eduName();

    mSpecialistUidList = per->specialistUidList();

    mCourseUid = per->courseUid();
    mCourse = per->course();

    mCountryUid = per->countryUid();
    mCountryName = per->countryName();
    mProvinceUid = per->provinceUid();
    mProvinceName = per->provinceName();
    mAddr = per->addr();
    mChurchAddr = per->churchAddr();
    mEmail = per->email();
    mTel = per->tel();
    mOtherContact = per->otherContact();

    mDadName = per->dadName();
    mDadBirthday = per->dadBirthday();
    mDadAddr = per->dadAddr();

    mMomName = per->momName();
    mMomBirthday = per->momBirthday();
    mMomAddr = per->momAddr();

    mFamilyHistory = per->familyHistory();
    mFamilyContact = per->familyContact();

    mChristenDate = per->christenDate();
    mChristenPlace = per->christenPlace();

    mEucharistPlace = per->eucharistPlace();
    mEucharistDate = per->eucharistDate();

    mHollyPlace = per->hollyPlace();
    mHollyDate = per->hollyDate();
    // TODO:   mEducationList,    mWorkList;
    mEducationUidList = per->educationUidList();
    mWorkUidList = per->workUidList();

    mJoinDate = per->joinDate();
    mJoinPICUid = per->joinPICUid();
    mJoinPICName = per->joinPICName();

    mPreTrainJoinDate = per->preTrainJoinDate();
    mPreTrainPICUid = per->preTrainPICUid();
    mPreTrainPICName = per->preTrainPICName();
    mTrainJoinDate = per->trainJoinDate();

    mTrainPICUid = per->trainPICUid();
    mTrainPICName = per->trainPICName();

    mVowsDate = per->vowsDate();
    mVowsCEOUid = per->vowsCEOUid();
    mVowsCEOName = per->vowsCEOName();

    mEternalVowsDate = per->eternalVowsDate();
    mEternalVowsCEOUid = per->eternalVowsCEOUid();
    mEternalVowsCEOName = per->eternalVowsCEOName();
    mEternalVowsPICUid = per->eternalVowsPICUid();
    mEternalVowsPICName = per->eternalVowsPICName();

    mBankDate = per->bankDate();
    mBankPlace = per->bankPlace();

    mGoldenDate = per->goldenDate();
    mGoldenPlace = per->goldenPlace();

    mEternalDate = per->eternalDate();
    mEternalPlace = per->eternalPlace();

    mStatusUid = per->statusUid();
    mStatusName = per->statusName();

    mRetireDate = per->retireDate();
    mRetirePlace = per->retirePlace();

    mDeadDate = per->deadDate();
    mDeadPlace = per->deadPlace();

    mEventUidList = per->eventUidList();

    mCommunityUid = per->communityUid();
    mCommunityName = per->communityName();
    mCurrentWorkUid = per->currentWorkUid();
    mCurrentWorkName = per->currentWorkName();

//    foreach (QString key, per.exportFields().keys()) {
//        mExportFields.insert(key, per.exportFields()[key]); // TODO: use iterator ??
//    }

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
    // FIXME: there are some case that this is not suitable, i.e. info like name
    // is change, no birthday/birthplace info, etc.
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
    mExportFields.insert(KItemImgPath, [this](const QString& item){
        QString imgPath = this->imgPath();
        if (imgPath.isEmpty()){
            imgPath = ":/icon/icon/unknown.png";
        }
        return imgPath;
    });
    mExportFields.insert(KItemFullName, [this](const QString& item){
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
    mExportFields.insert(KItemBirthday, [this](const QString& item){
        return Utils::date2String(this->birthday());});
    mExportFields.insert(KItemBirthplace, [this](const QString& item){
        return this->birthPlace();
    });
    mExportFields.insert(KItemHollyName, [this](const QString& item){
        return this->hollyName();
    });
    mExportFields.insert(KItemFeastDay, [this](const QString& item){
        return Utils::date2String(this->feastDay(), DATE_FORMAT_MD);
    });
    mExportFields.insert(KItemNationality, [this](const QString& item){
        return this->nationalityName();
    });
    mExportFields.insert(KItemEthnic, [this](const QString& item){
        return this->ethnicName();
    });
    mExportFields.insert(KItemIDcard, [this](const QString& item){
        return this->idCard();
    });
    mExportFields.insert(KItemIDcardIssueDate,  [this](const QString& item){
        return Utils::date2String(this->idCardIssueDate(), DATE_FORMAT_YMD);
    });
    mExportFields.insert(KItemIDcardIssuer,  [this](const QString& item){
        return this->idCardIssuePlace();
    });
    mExportFields.insert(KItemStatus, nullptr);
    mExportFields.insert(KItemRetireDate, nullptr);
    mExportFields.insert(KItemRetirePlace, nullptr); //"retire_place";
    mExportFields.insert(KItemDeadDate, nullptr); //"dead_date";
    mExportFields.insert(KItemDeadPlace, nullptr); //"dead_place";
    mExportFields.insert(KItemCountry, nullptr); //"country";
    mExportFields.insert(KItemProvince, nullptr); //"province";
    mExportFields.insert(KItemAddress, [this](const QString& item){
        return this->addr();
    }); //"address";
    mExportFields.insert(KItemChurchAddress, [this](const QString& item){
        return this->churchAddr();
    }); //"church_addr";
    mExportFields.insert(KItemTel, [this](const QString& item){
        return this->tel().join(";");
    }); //"telephone";
    mExportFields.insert(KItemEmail, [this](const QString& item){
        return this->email().join(";");
    }); //"email";
    mExportFields.insert(KItemOtherContact, nullptr); //"othercontact";
    mExportFields.insert(KItemEdu, nullptr); //"education";
    mExportFields.insert(KItemEduUid, [this](const QString& item){
        return this->eduUid();
    }); //"edu uid";
    mExportFields.insert(KItemSpeciaist,  [this](const QString& item){
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
    mExportFields.insert(KItemSpeciaistUid, [this](const QString& item){
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
    mExportFields.insert(KItemJoinDate, [this](const QString& item){
        return Utils::date2String(this->joinDate(), DATE_FORMAT_YMD);
    }); //"join_date";
    mExportFields.insert(KItemJoinPIC, [this](const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, this->joinPICUid(), this->joinPICName());
//        QString val;
//        if (!this->joinPICUid().isEmpty()) {
//            QString("%1 (%2...)").arg(this->joinPICName(), this->joinPICUid().left(8));
//            logd("Join PIC %s", val.toStdString().c_str());
//        }
//        // TODO: show person code instead of uid?? uid should use for debug only?
//        return val;
    }); //"join_pic";
    mExportFields.insert(KItemPreTrainDate, nullptr); //"pre_train_date";
    mExportFields.insert(KItemPreTrainPIC, [this](const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, this->joinPICUid(), this->joinPICName());
    }); //"pre_train_pic";
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
    mImportFields.insert(KItemPersonCode, [this](const QString& value){
        this->setPersonCode(value);
    });
    mImportFields.insert(KItemFullName, [this](const QString& value){
        this->setNameFromFullName(value);
    });
    mImportFields.insert(KItemBirthday, [this](const QString& value){
        this->setBirthday(value);
    });
    mImportFields.insert(KItemBirthplace, [this](const QString& value){
        this->setBirthPlace(value);
    });
    mImportFields.insert(KItemHollyName, [this](const QString& value){
        this->setHollyName(value, true);
    });
    mImportFields.insert(KItemFeastDay, [this](const QString& value){this->setFeastDay(value);});
    mImportFields.insert(KItemNationality, [this](const QString& value){this->setNationalityName(value);});
    mImportFields.insert(KItemEthnicUID, [this](const QString& value){this->setEthnicUid(value);});
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
    mImportFields.insert(KItemOtherContact, [this](const QString& value){
        this->setOtherContact(value);
    }); //"othercontact";
    mImportFields.insert(KItemEdu, [this](const QString& value){
        this->setEduName(value);
    }); //"education";
    mImportFields.insert(KItemEduUid, [this](const QString& value){
        this->setEduUid(value);
    }); //"education";
    mImportFields.insert(KItemSpeciaist, [this](const QString& value){
        this->setSpecialistNames(value, true);
    }); //"specialist";
    mImportFields.insert(KItemSpeciaistUid, [this](const QString& value){
        this->setSpecialistUidList(value);
    }); //"specialist";
    mImportFields.insert(KItemWork, [this](const QString& value){
        this->setCurrentWorkName(value);
    }); //"work"; // TODO
    mImportFields.insert(KItemWorkId, [this](const QString& value){
        this->setCurrentWorkUid(value);
    }); //"work"; // TODO
    mImportFields.insert(KItemWorkHistory, nullptr); //"work_history"; // TODO
    mImportFields.insert(KItemCommunity, [this](const QString& value){
        this->setCommunityName(value);
    }); //"community"; // TODO
    mImportFields.insert(KItemCommunityId, [this](const QString& value){
        this->setCommunityUid(value);
    }); //"community"; // TODO
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

ErrCode Person::commonCheckField(QString& name,
                                 QString& uid,
                                 Controller* controller,
                                 const char* const itemName,
                                 int& invalidField)
{
    traced;
    ErrCode ret = ErrNone;

    loge("Check item %s", itemName);
    if (name.isEmpty() && uid.isEmpty()) {
        logd("Data of %s empty, nothing to do", itemName);
        ret = ErrNone;
        // TODO: check logic again, all empty mean valid????
    } else {
        DbModel* model = nullptr;
        if (!uid.isEmpty()) {
            logd("get model by uid '%s'", uid.toStdString().c_str());
            model = controller->getModelByUid(uid);
        } else if (!name.isEmpty()) {
            logd("get model by name '%s'", name.toStdString().c_str());
            model = controller->getModelByName(name);
        } else {
            logd("name & id is empty");
            ret = ErrInvalidData;
        }
        // TODO: something stupid check here ^^
        if (ret == ErrNone) {
            if (model) {
                uid = model->uid();
                name = model->name();
                // TODO: is is safe to delete here???
                delete model;
            } else {
                loge("Not found %s info", itemName);
                appendValidateResult(itemName, ErrNotFound);
                appendValidateMsg(QString("%1 '%2' not found").arg(itemName, name));
                ret = ErrInvalidData;
                invalidField++;
            }
        }

    }
    tracedr(ret);
    return ret;
}
ErrCode Person::validate()
{
    traced;
    // TODO: cached value???
    ErrCode ret = ErrNone;
    int invaidField = 0;
    DbModelHandler* personHdlr = this->getDbModelHandler();
    if (personHdlr == nullptr) {
        loge("Invalid Person handler");
        return ErrFailed;
    }

//        ret = checkAndUpdateSaintListFromHollyName();
    // Holly name / saint list
    // Holly name to saint list
    // TODO: saint uid list to holly name???
    QHash<QString, QString> saintUidList;
    ret = SAINTCTL->getSaintUidListFromName(mHollyName, &saintUidList);
    if (ret == ErrNone) {
        mSaintUidNameMap.insert(saintUidList);
        mSaintUidList.append(saintUidList.keys());
    } else {
        appendValidateResult(KItemHollyName, ErrNotFound);
        appendValidateMsg(QString("Saint %1 not found").arg(hollyName()));
        invaidField ++;
        loge("Invalid holly name %s", mHollyName.toStdString().c_str());
    }

    // country
    ret = commonCheckField(mCountryName, mCountryUid, COUNTRYCTL, KItemCountry, invaidField);


    // TODO: check logic again, all empty mean valid, should have field must has data???????

    // nationality
    ret = commonCheckField(mNationalityName, mNationalityUid, COUNTRYCTL, KItemNationality, invaidField);
    // TODO: same ethnic name, but different country???
    ret = commonCheckField(mEthnicName, mEthnicUid, ETHNIC, KItemEthnic, invaidField);
    ret = commonCheckField(mEduName, mEduUid, EDUCTL, KItemEdu, invaidField);
    ret = commonCheckField(mCourse, mCourseUid, COURSECTL, KItemCourse, invaidField);

    // TODO: province: check with country??? as province belong to a country
#ifndef SKIP_PERSON_PROVINE
    ret = commonCheckField(mProvinceName, mProvinceUid, PROVINCE, KItemProvince, invaidField);
#endif
    ret = commonCheckField(mCurrentWorkName, mCurrentWorkUid, WORKCTL, KItemWork, invaidField);
    ret = commonCheckField(mCommunityName, mCommunityUid, COMMUNITYCTL, KItemCommunity, invaidField);

    if (invaidField > 0) {
        loge("%d invalid field", invaidField);
        ret = ErrInvalidData;
    }



    tracedr(ret);
    return ret;
}

ErrCode Person::fromCSVFile(const QString &fname)
{
    traced;
    QList<QHash<QString, QString>> items;
    QHash<QString, QString> item;
    logd("parse csv file %s", fname.toStdString().c_str());
    ErrCode ret = Utils::parseDataFromCSVFile(fname, &items);
    if (ret == ErrNone){
        logd("Parsed %d key", items.count());
        // TODO: fixme if it's actually suitable??
        if (items.count() > 0) {
            item = items.at(0); // use 1st one only
        }
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
    CHECK_MODIFIED_THEN_SET(mPersonCode, newPersonCode, KItemPersonCode);
//    mPersonCode = newPersonCode;
}

const QString &Person::firstName() const
{
    return mFirstName;
}

void Person::setFirstName(const QString &newFirstName)
{
    CHECK_MODIFIED_THEN_SET(mFirstName, newFirstName, KItemFullName);
//    mFirstName = newFirstName;
}


const QString &Person::birthPlace() const
{
    return mBirthPlace;
}

void Person::setBirthPlace(const QString &newBirthPlace)
{
//    mBirthPlace = newBirthPlace;

    CHECK_MODIFIED_THEN_SET(mBirthPlace, newBirthPlace, KItemBirthplace);
}

qint64 Person::birthday() const
{
    return mBirthday;
}

void Person::setBirthday(qint64 newBirthday)
{
//    mBirthday = newBirthday;
    CHECK_MODIFIED_THEN_SET(mBirthday, newBirthday, KItemBirthday);
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
    CHECK_MODIFIED_THEN_SET(mLastName, newLastName, KItemFullName);
//    mLastName = newLastName;
}


ErrCode Person::setNameFromFullName(const QString &name)
{
    QStringList items = name.trimmed().split(" ", Qt::SkipEmptyParts);
    QString firstName;
    QString lastName;
    if (items.length() > 0){
        firstName = items.last();
        items.removeLast();
    }
    // TODO: this is vietnamese style only, how about name in Englist style???
    lastName = items.join(" ");
    logd("first name %s", firstName.toStdString().c_str());
    logd("last name %s", lastName.toStdString().c_str());
    if ((firstName != mFirstName) || (lastName != mLastName)) { // FIXME: not good condition checking, but let it be
        mFirstName = firstName;
        mLastName = lastName;
        markItemAsModified(KItemFullName);
    }

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
    CHECK_MODIFIED_THEN_SET(mChristenDate, newChristenDate, KItemChristenDate);
//    mChristenDate = newChristenDate;
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
        std::function<QString(const QString&)> func = mExportFields.value(keyword);
        if (func != nullptr) *data = func(keyword);
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
//        ret = checkAndUpdateSaintListFromHollyName();
        if (mSaintUidList.count() == 0 && mHollyName.length() > 0) {
            logd("Saint id list is zero, but holly name exist");
            QHash<QString, QString> uidList;
            ret = SAINTCTL->getSaintUidListFromName(mHollyName, &uidList);
            if (ret == ErrNone) {
                mSaintUidNameMap.insert(uidList);
                mSaintUidList.append(uidList.keys());
            }
        }
    }
    tracedr(ret);
    return ret;
}

const QHash<QString, std::function<QString (const QString&)> > &Person::exportFields() const
{
    return mExportFields;
}

const QString &Person::currentWorkName() const
{
    return mCurrentWorkName;
}

void Person::setCurrentWorkName(const QString &newCurrentWorkName)
{
    CHECK_MODIFIED_THEN_SET(mCurrentWorkName, newCurrentWorkName, KItemWork);
    // TODO: load work uid???
}

const QString &Person::currentWorkUid() const
{
    return mCurrentWorkUid;
}

void Person::setCurrentWorkUid(const QString &newCurrentWorkUid)
{
    CHECK_MODIFIED_THEN_SET(mCurrentWorkUid, newCurrentWorkUid, KItemWork);
    // TODO: load work name????
}


const QHash<QString, QString> &Person::saintUidNameMap() const
{
    return mSaintUidNameMap;
}

void Person::setSaintUidNameMap(const QHash<QString, QString> &newSaintUidNameMap)
{
    mSaintUidNameMap = newSaintUidNameMap;
}

const QString &Person::specialistInfo() const
{
    return mSpecialistInfo;
}

void Person::setSpecialistInfo(const QString &newSpecialistInfo)
{
    CHECK_MODIFIED_THEN_SET(mSpecialistInfo, newSpecialistInfo, KItemSpeciaistInfo);
//    mSpecialistInfo = newSpecialistInfo;
}

const QString &Person::eduDetail() const
{
    return mEduDetail;
}

void Person::setEduDetail(const QString &newEduDetail)
{
    CHECK_MODIFIED_THEN_SET(mEduDetail, newEduDetail, KItemEduDetail);
//    mEduDetail = newEduDetail;
}

const QString &Person::communityName() const
{
    return mCommunityName;
}

void Person::setCommunityName(const QString &newCommunityName)
{
    CHECK_MODIFIED_THEN_SET(mCommunityName, newCommunityName, KItemCommunity);
//    mCommunityName = newCommunityName;
}

const QString &Person::communityUid() const
{
    return mCommunityUid;
}

void Person::setCommunityUid(const QString &newCommunityUid)
{
    CHECK_MODIFIED_THEN_SET(mCommunityUid, newCommunityUid, KItemCommunity);
//    mCommunityUid = newCommunityUid;
//    markItemAsModified(KItemCommunity);
}

const QString &Person::departName() const
{
    return mDepartName;
}

void Person::setDepartName(const QString &newDepartName)
{
    CHECK_MODIFIED_THEN_SET(mDepartName, newDepartName, KItemDepartment);
//    mDepartName = newDepartName;
}

const QString &Person::departUid() const
{
    return mDepartUid;
}

void Person::setDepartUid(const QString &newDepartUid)
{
    CHECK_MODIFIED_THEN_SET(mDepartUid, newDepartUid, KItemDepartment);
//    mDepartUid = newDepartUid;
}

const QString &Person::areaName() const
{
    return mAreaName;
}

void Person::setAreaName(const QString &newAreaName)
{
    CHECK_MODIFIED_THEN_SET(mAreaName, newAreaName, KItemArea);
//    mAreaName = newAreaName;
}

const QString &Person::areaUid() const
{
    return mAreaUid;
}

void Person::setAreaUid(const QString &newAreaUid)
{
    CHECK_MODIFIED_THEN_SET(mAreaUid, newAreaUid, KItemArea);
//    mAreaUid = newAreaUid;
}

const QString &Person::nationalityName() const
{
    return mNationalityName;
}

void Person::setNationalityName(const QString &newNationalityName)
{
//    mNationalityName = newNationalityName;
    CHECK_MODIFIED_THEN_SET(mNationalityName, newNationalityName, KItemNationality);
}

void Person::dump()
{
    traced;
    DbModel::dump();
    logd("- FullName %s", getFullName().toStdString().c_str());
    logd("- FirstName %s", firstName().toStdString().c_str());
    logd("- LastName %s", lastName().toStdString().c_str());
    logd("- HollyName %s", hollyName().toStdString().c_str());
    logd("- JoinPIC Name %s", joinPICName().toStdString().c_str());
    logd("- JoinPIC Uid %s", joinPICUid().toStdString().c_str());
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
    if (mPersonCode.isEmpty()) {
        logd("No person code, auto gen");
        // TODO: should make temp code??? to distingue with official one
        mPersonCode = CONFIG->getNextPersonalCode();
        // TODO: numer is increased, but not save --> may cause much dummy code?
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
//    mFeastDay = newFeastDay;
    CHECK_MODIFIED_THEN_SET(mFeastDay, newFeastDay, KItemFeastDay);
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
    CHECK_MODIFIED_THEN_SET(mEucharistPlace, newEucharistPlace, KItemEucharistPlace);
//    mEucharistPlace = newEucharistPlace;
}

qint64 Person::eucharistDate() const
{
    return mEucharistDate;
}

void Person::setEucharistDate(qint64 newEucharistDate)
{
    CHECK_MODIFIED_THEN_SET(mEucharistDate, newEucharistDate, KItemEucharistDate);
//    mEucharistDate = newEucharistDate;
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
    CHECK_MODIFIED_THEN_SET(mOtherContact, newOtherContact, KItemOtherContact);
//    mOtherContact = newOtherContact;
}

const QHash<QString, QString> &Person::workUidList() const
{
    return mWorkUidList;
}

void Person::setWorkUidList(const QHash<QString, QString> &newWorkUidList)
{
    mWorkUidList = newWorkUidList; // TODO: check change before set???
    markItemAsModified(KItemWork);
}

const QHash<QString, QString> &Person::educationUidList() const
{
    return mEducationUidList;
}

void Person::setEducationUidList(const QHash<QString, QString> &newEducationUidList)
{
    mEducationUidList = newEducationUidList;
    markItemAsModified(KItemEducation);
}

const QList<Work *> &Person::workList() const
{
    return mWorkList;
}

void Person::setWorkList(const QList<Work *> &newWorkList)
{
    mWorkList = newWorkList;
    markItemAsModified(KItemWork);
}

const QList<Education *> &Person::educationList() const
{
    return mEducationList;
}

void Person::setEducationList(const QList<Education *> &newEducationList)
{
    mEducationList = newEducationList;
    markItemAsModified(KItemEducation);
}

const QStringList &Person::eventUidList() const
{
    return mEventUidList;
}

void Person::setEventUidList(const QStringList &newEventUidList)
{
    mEventUidList = newEventUidList;
    markItemAsModified(KItemEvent);
}

const QString &Person::imgId() const
{
    return mImgId;
}

void Person::setImgId(const QString &newImgId)
{
    CHECK_MODIFIED_THEN_SET(mImgId, newImgId, KItemImg);
//    mImgId = newImgId;
}

const QString &Person::deadPlace() const
{
    return mDeadPlace;
}

void Person::setDeadPlace(const QString &newDeadPlace)
{
    CHECK_MODIFIED_THEN_SET(mDeadPlace, newDeadPlace, KItemDeadPlace);
//    mDeadPlace = newDeadPlace;
}

qint64 Person::deadDate() const
{
    return mDeadDate;
}

void Person::setDeadDate(qint64 newDeadDate)
{
    CHECK_MODIFIED_THEN_SET(mDeadDate, newDeadDate, KItemDeadDate);
//    mDeadDate = newDeadDate;
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
//    mRetirePlace = newRetirePlace;
    CHECK_MODIFIED_THEN_SET(mRetirePlace, newRetirePlace, KItemRetirePlace);
}

qint64 Person::retireDate() const
{
    return mRetireDate;
}

void Person::setRetireDate(qint64 newRetireDate)
{
    CHECK_MODIFIED_THEN_SET(mRetireDate, newRetireDate, KItemRetireDate);
//    mRetireDate = newRetireDate;
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
    CHECK_MODIFIED_THEN_SET(mStatusName, newStatusName, KItemStatus);
//    mStatusName = newStatusName;
}

const QString &Person::statusUid() const
{
    return mStatusUid;
}

void Person::setStatusUid(const QString &newStatusUid)
{
    CHECK_MODIFIED_THEN_SET(mStatusUid, newStatusUid, KItemStatus);
//    mStatusUid = newStatusUid;
}

const QString &Person::eternalPlace() const
{
    return mEternalPlace;
}

void Person::setEternalPlace(const QString &newEternalPlace)
{
    CHECK_MODIFIED_THEN_SET(mEternalPlace, newEternalPlace, KItemEternalPlace);
//    mEternalPlace = newEternalPlace;
}

qint64 Person::eternalDate() const
{
    return mEternalDate;
}

void Person::setEternalDate(qint64 newEternalDate)
{
    CHECK_MODIFIED_THEN_SET(mEternalDate, newEternalDate, KItemEternalDate);
//    mEternalDate = newEternalDate;
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
    CHECK_MODIFIED_THEN_SET(mGoldenPlace, newGoldenPlace, KItemGoldenPlace);
//    mGoldenPlace = newGoldenPlace;
}

qint64 Person::goldenDate() const
{
    return mGoldenDate;
}

void Person::setGoldenDate(qint64 newGoldenDate)
{
    CHECK_MODIFIED_THEN_SET(mGoldenDate, newGoldenDate, KItemGoldenDate);
//    mGoldenDate = newGoldenDate;
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
    CHECK_MODIFIED_THEN_SET(mBankPlace, newBankPlace, KItemGoldenDate);
//    mBankPlace = newBankPlace;
}

qint64 Person::bankDate() const
{
    return mBankDate;
}

void Person::setBankDate(qint64 newBankDate)
{
    CHECK_MODIFIED_THEN_SET(mBankDate, newBankDate, KItemBankDate);
//    mBankDate = newBankDate;
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
    CHECK_MODIFIED_THEN_SET(mEternalVowsPICName, newEternalVowsName, KItemEternalVowsCEO);
//    mEternalVowsPICName = newEternalVowsName;
}

const QString &Person::eternalVowsPICUid() const
{
    return mEternalVowsPICUid;
}

void Person::setEternalVowsPICUid(const QString &newEternalVowsPICUid)
{
    CHECK_MODIFIED_THEN_SET(mEternalVowsPICUid, newEternalVowsPICUid, KItemEternalVowsCEO);
//    mEternalVowsPICUid = newEternalVowsPICUid;
}

const QString &Person::eternalVowsCEOName() const
{
    return mEternalVowsCEOName;
}

void Person::setEternalVowsCEOName(const QString &newEternalVowsCEOName)
{
    CHECK_MODIFIED_THEN_SET(mEternalVowsCEOName, newEternalVowsCEOName, KItemEternalVowsCEO);
//    mEternalVowsCEOName = newEternalVowsCEOName;
}

const QString &Person::eternalVowsCEOUid() const
{
    return mEternalVowsCEOUid;
}

void Person::setEternalVowsCEOUid(const QString &newEternalVowsCEOUid)
{
    CHECK_MODIFIED_THEN_SET(mEternalVowsCEOUid, newEternalVowsCEOUid, KItemEternalVowsCEO);
//    mEternalVowsCEOUid = newEternalVowsCEOUid;
}

qint64 Person::eternalVowsDate() const
{
    return mEternalVowsDate;
}

void Person::setEternalVowsDate(qint64 newEternalVowsDate)
{
    CHECK_MODIFIED_THEN_SET(mEternalVowsDate, newEternalVowsDate, KItemEternalVowsDate);
//    mEternalVowsDate = newEternalVowsDate;
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
    CHECK_MODIFIED_THEN_SET(mVowsDate, newVowsDate, KItemVowsDate);
//    mVowsDate = newVowsDate;
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
    CHECK_MODIFIED_THEN_SET(mVowsCEOName, newVowsCEOName, KItemVowsCEO);
//    mVowsCEOName = newVowsCEOName;
}

const QString &Person::vowsCEOUid() const
{
    return mVowsCEOUid;
}

void Person::setVowsCEOUid(const QString &newVowsCEOUid)
{
    CHECK_MODIFIED_THEN_SET(mVowsCEOUid, newVowsCEOUid, KItemVowsCEO);
//    mVowsCEOUid = newVowsCEOUid;
}

const QString &Person::trainPICName() const
{
    return mTrainPICName;
}

void Person::setTrainPICName(const QString &newTrainPICName)
{
    CHECK_MODIFIED_THEN_SET(mTrainPICName, newTrainPICName, KItemTrainPIC);
//    mTrainPICName = newTrainPICName;
}

const QString &Person::trainPICUid() const
{
    return mTrainPICUid;
}

void Person::setTrainPICUid(const QString &newTrainPICUid)
{
    CHECK_MODIFIED_THEN_SET(mTrainPICUid, newTrainPICUid, KItemTrainPIC);
//    mTrainPICUid = newTrainPICUid;
}

const QString &Person::preTrainPICName() const
{
    return mPreTrainPICName;
}

void Person::setPreTrainPICName(const QString &newPreTrainPICName)
{
    CHECK_MODIFIED_THEN_SET(mPreTrainPICName, newPreTrainPICName, KItemPreTrainPIC);
//    mPreTrainPICName = newPreTrainPICName;
}

const QString &Person::preTrainPICUid() const
{
    return mPreTrainPICUid;
}

void Person::setPreTrainPICUid(const QString &newPreTrainPICUid)
{
    CHECK_MODIFIED_THEN_SET(mPreTrainPICUid, newPreTrainPICUid, KItemPreTrainPIC);
//    mPreTrainPICUid = newPreTrainPICUid;
}

qint64 Person::preTrainJoinDate() const
{
    return mPreTrainJoinDate;
}

void Person::setPreTrainJoinDate(qint64 newPreTrainJoinDate)
{
    CHECK_MODIFIED_THEN_SET(mPreTrainJoinDate, newPreTrainJoinDate, KItemPreTrainDate);
//    mPreTrainJoinDate = newPreTrainJoinDate;
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
    CHECK_MODIFIED_THEN_SET(mJoinPICName, newJoinPICName, KItemJoinPIC);
//    mJoinPICName = newJoinPICName;
}

const QString &Person::joinPICUid() const
{
    return mJoinPICUid;
}

void Person::setJoinPICUid(const QString &newJoinPICUid)
{
    CHECK_MODIFIED_THEN_SET(mJoinPICUid, newJoinPICUid, KItemJoinPIC);
//    mJoinPICUid = newJoinPICUid;
}

qint64 Person::joinDate() const
{
    return mJoinDate;
}

void Person::setJoinDate(qint64 newJoinDate)
{
    CHECK_MODIFIED_THEN_SET(mJoinDate, newJoinDate, KItemJoinDate);
//    mJoinDate = newJoinDate;
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
    CHECK_MODIFIED_THEN_SET(mFamilyContact, newFamilyContact, KItemFamilyContact);
//    mFamilyContact = newFamilyContact;
}

qint64 Person::hollyDate() const
{
    return mHollyDate;
}

void Person::setHollyDate(qint64 newHollyDate)
{
    CHECK_MODIFIED_THEN_SET(mHollyDate, newHollyDate, KItemHollyDate);
//    mHollyDate = newHollyDate;
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
    CHECK_MODIFIED_THEN_SET(mHollyPlace, newHollyPlace, KItemHollyPlace);
//    mHollyPlace = newHollyPlace;
}

const QString &Person::christenPlace() const
{
    return mChristenPlace;
}

void Person::setChristenPlace(const QString &newChristenPlace)
{
    CHECK_MODIFIED_THEN_SET(mChristenPlace, newChristenPlace, KItemChristenPlace);
//    mChristenPlace = newChristenPlace;
}

const QString &Person::familyHistory() const
{
    return mFamilyHistory;
}

void Person::setFamilyHistory(const QString &newFamilyHistory)
{
    CHECK_MODIFIED_THEN_SET(mFamilyHistory, newFamilyHistory, KItemFamilyHistory);
//    mFamilyHistory = newFamilyHistory;
}

const QString &Person::momAddr() const
{
    return mMomAddr;
}

void Person::setMomAddr(const QString &newMomAddr)
{
    CHECK_MODIFIED_THEN_SET(mMomAddr, newMomAddr, KItemMomAddr);
//    mMomAddr = newMomAddr;
}

qint64 Person::momBirthday() const
{
    return mMomBirthday;
}

void Person::setMomBirthday(qint64 newMomBirthday)
{
    CHECK_MODIFIED_THEN_SET(mMomBirthday, newMomBirthday, KItemMomBirthday);
//    mMomBirthday = newMomBirthday;
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
    CHECK_MODIFIED_THEN_SET(mMomName, newMomName, KItemMom);
//    mMomName = newMomName;
}

const QString &Person::dadAddr() const
{
    return mDadAddr;
}

void Person::setDadAddr(const QString &newDadAddr)
{
    CHECK_MODIFIED_THEN_SET(mDadAddr, newDadAddr, KItemDadAddr);
//    mDadAddr = newDadAddr;
}

qint64 Person::dadBirthday() const
{
    return mDadBirthday;
}

void Person::setDadBirthday(qint64 newDadBirthday)
{
    CHECK_MODIFIED_THEN_SET(mDadBirthday, newDadBirthday, KItemDadBirthday);
//    mDadBirthday = newDadBirthday;
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
    CHECK_MODIFIED_THEN_SET(mDadName, newDadName, KItemDad);
//    mDadName = newDadName;
}

const QStringList &Person::tel() const
{
    return mTel;
}

void Person::setTel(const QStringList &newTel)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mTel, newTel, KItemTel);
//    if (!Utils::isSameList<QString>(mTel, newTel)) {
//        mTel.clear();
//        mTel.append(newTel);
////        mTel = newTel;
//        markItemAsModified(KItemTel);
//    }
}

void Person::setTel(const QString &newTel)
{
    setTel(newTel.split(';'));
}

const QStringList &Person::email() const
{
    return mEmail;
}

void Person::setEmail(const QStringList &newEmail)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mEmail, newEmail, KItemEmail);
//    if (!Utils::isSameList<QString>(mEmail, newEmail)) {
//        mEmail.clear();
//        mEmail.append(newEmail);
//        markItemAsModified(KItemTel);
//    }
//    mEmail = newEmail;
}

void Person::setEmail(const QString &newEmail)
{
    setEmail(newEmail.split(';'));
}

const QString &Person::churchAddr() const
{
    return mChurchAddr;
}

void Person::setChurchAddr(const QString &newChurchAddr)
{
    CHECK_MODIFIED_THEN_SET(mChurchAddr, newChurchAddr, KItemChurchAddress);
//    mChurchAddr = newChurchAddr;
}

const QString &Person::addr() const
{
    return mAddr;
}

void Person::setAddr(const QString &newAddr)
{
    CHECK_MODIFIED_THEN_SET(mAddr, newAddr, KItemAddress);
//    mAddr = newAddr;
}

const QString &Person::provinceName() const
{
    return mProvinceName;
}

void Person::setProvinceName(const QString &newProvinceName)
{
    CHECK_MODIFIED_THEN_SET(mProvinceName, newProvinceName, KItemProvince);
//    mProvinceName = newProvinceName;
}

const QString &Person::provinceUid() const
{
    return mProvinceUid;
}

void Person::setProvinceUid(const QString &newProvinceUid)
{
    CHECK_MODIFIED_THEN_SET(mProvinceUid, newProvinceUid, KItemProvince);
//    mProvinceUid = newProvinceUid;
}

const QString &Person::countryName() const
{
    return mCountryName;
}

void Person::setCountryName(const QString &newCountryName)
{
    CHECK_MODIFIED_THEN_SET(mCountryName, newCountryName, KItemCountry);
//    mCountryName = newCountryName;
}

const QString &Person::countryUid() const
{
    return mCountryUid;
}

void Person::setCountryUid(const QString &newCountryUid)
{
    CHECK_MODIFIED_THEN_SET(mCountryUid, newCountryUid, KItemCountry);
//    mCountryUid = newCountryUid;
}

const QString &Person::courseUid() const
{
    return mCourseUid;
}

void Person::setCourseUid(const QString &newCourseUid)
{
    CHECK_MODIFIED_THEN_SET(mCourseUid, newCourseUid, KItemCourse);
//    mCourseUid = newCourseUid;
}

const QString &Person::course() const
{
    return mCourse;
}

void Person::setCourse(const QString &newCourse)
{
    CHECK_MODIFIED_THEN_SET(mCourse, newCourse, KItemCourse);
//    mCourse = newCourse;
}

const QStringList &Person::specialistUidList() const
{
    return mSpecialistUidList;
}

void Person::setSpecialistUidList(const QStringList &newSpecialistUidList)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mSpecialistUidList, newSpecialistUidList, KItemSpeciaist);
//    CHECK_MODIFIED_THEN_SET(mSpecialistUidList, newSpecialistUidList, KItemSpeciaist);
    //    mSpecialistUidList = newSpecialistUidList;
}

void Person::setSpecialistUidList(const QString &newSpecialistUidList)
{
    setSpecialistUidList(newSpecialistUidList.split(NAME_SPLIT));
}

void Person::setSpecialistNames(const QString &newSpecialists, bool parseUid)
{
//    setSpecialistUidList(newSpecialists.split(NAME_SPLIT));
//    mSpecialistNameList = newSpecialists.split(split);
    // TODO: check to remove redudant, check if they're same list
//    markItemAsModified(KItemSpeciaist);
    traced;
    QStringList list;
    foreach (QString name, newSpecialists.split(NAME_SPLIT)) {
        list.append(name.trimmed());
    }
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mSpecialistNameList, list, KItemSpeciaist);

    logd("parseUid %d", parseUid);
    if (parseUid){
        QHash<QString, QString> uidList;
        ErrCode ret = ErrNone;
        ret = SPECIALISTCTL->getUidListFromName(newSpecialists, &uidList);
        if (ret == ErrNone) {
            mSpecialistUidNameMap.insert(uidList);
            mSpecialistUidList.append(uidList.keys());
        } else {
            loge("Failed to get uid from specialist name %d", ret);
        }
    } else {
        logd("Skip parsing specialist to uid");
    }
    tracede;

}

void Person::clearSpecialistUid()
{
    mSpecialistUidList.clear();
    markItemAsModified(KItemSpeciaist);
}

void Person::addSpecialistUid(const QString &uid)
{
    if (!mSpecialistUidList.contains(uid)) {
        mSpecialistUidList.append(uid);
        markItemAsModified(KItemSpeciaist);
    }
}

const QStringList &Person::specialistNameList() const
{
    return mSpecialistNameList;
}

void Person::setSpecialistNameList(const QStringList &newSpecialistNameList)
{
    mSpecialistNameList = newSpecialistNameList;
}

const QString &Person::eduName() const
{
    return mEduName;
}

void Person::setEduName(const QString &newEduName)
{
    CHECK_MODIFIED_THEN_SET(mEduName, newEduName, KItemEdu);
//    mEduName = newEduName;
}

const QString &Person::eduUid() const
{
    return mEduUid;
}

void Person::setEduUid(const QString &newEduUid)
{
    CHECK_MODIFIED_THEN_SET(mEduUid, newEduUid, KItemEdu);
//    mEduUid = newEduUid;
}

const QString &Person::idCardIssuePlace() const
{
    return mIdCardIssuePlace;
}

void Person::setIdCardIssuePlace(const QString &newIdCardIssuePlace)
{
    CHECK_MODIFIED_THEN_SET(mIdCardIssuePlace, newIdCardIssuePlace, KItemIDcardIssuer);
//    mIdCardIssuePlace = newIdCardIssuePlace;
}

qint64 Person::idCardIssueDate() const
{
    return mIdCardIssueDate;
}

void Person::setIdCardIssueDate(qint64 newIdCardIssueDate)
{
    CHECK_MODIFIED_THEN_SET(mIdCardIssueDate, newIdCardIssueDate, KItemIDcardIssueDate);
//    mIdCardIssueDate = newIdCardIssueDate;
}

void Person::setIdCardIssueDate(const QString &newIdCardIssueDate)
{
    setIdCardIssueDate(Utils::dateFromString(newIdCardIssueDate));
}

const QString &Person::idCard() const
{
    return mIdCard;
}

void Person::setIdCard(const QString &newIdCard)
{
    CHECK_MODIFIED_THEN_SET(mIdCard, newIdCard, KItemIDcard);
//    mIdCard = newIdCard;
}

const QString &Person::ethnicName() const
{
    return mEthnicName;
}

void Person::setEthnicName(const QString &newEthnicName)
{
    CHECK_MODIFIED_THEN_SET(mEthnicName, newEthnicName, KItemEthnic);
//    mEthnicName = newEthnicName;
}

const QString &Person::ethnicUid() const
{
    return mEthnicUid;
}

void Person::setEthnicUid(const QString &newEthnicUid)
{
//    mEthnicUid = newEthnicUid;
    CHECK_MODIFIED_THEN_SET(mEthnicUid, newEthnicUid, KItemEthnic);
}

const QString &Person::nationalityUid() const
{
    return mNationalityUid;
}

void Person::setNationalityUid(const QString &newNationalityUid)
{
//    mNationalityUid = newNationalityUid;
    CHECK_MODIFIED_THEN_SET(mNationalityUid, newNationalityUid, KItemNationality);
}

const QString &Person::hollyName() const
{
    return mHollyName;
}

QStringList Person::hollyNameInList()
{
    QStringList nameList;
    foreach (QString name, mHollyName.split(HOLLYNAME_SPLIT)) {
        nameList.append(name.trimmed());
    }
    return nameList;
}

void Person::setHollyName(const QString &newHollyName, bool parseUid)
{
//    mHollyName = newHollyName;
    CHECK_MODIFIED_THEN_SET(mHollyName, newHollyName, KItemHolly);

    logd("parseUid %d", parseUid);
    if (parseUid){
        QHash<QString, QString> uidList;
        ErrCode ret = ErrNone;
        ret = SAINTCTL->getSaintUidListFromName(mHollyName, &uidList);
        if (ret == ErrNone) {
            mSaintUidNameMap.insert(uidList);
            mSaintUidList.append(uidList.keys());
        } else {
            loge("Failed to get uid from holly name %d", ret);
        }
    } else {
        logd("Skip parsing holly name to uid");
    }
}

//ErrCode Person::checkAndUpdateSaintListFromHollyName()
//{
//    traced;
//    DbModelHandler* saintHdlr = nullptr;
//    ErrCode ret = ErrNone;
//    saintHdlr = dynamic_cast<DbModelHandler*>(DB->getModelHandler(KModelHdlSaint));
//    if (saintHdlr == nullptr) {
//        loge("Invalid Saint handler");
//        ret = ErrFailed;
//    }
//    if (ret == ErrNone) {
//        if (mSaintUidList.empty() && !mHollyName.isEmpty()) {
//            QStringList names = mHollyName.split(",");
//            DbModel* model = nullptr;
//            QString hollyNotFound;
//            foreach (QString name, names) {
//                logd("Check saint name '%s'", name.toStdString().c_str());
//                model = saintHdlr->getByName(name.trimmed());
//                if (model) {
//                    logd("update saint uid %s", model->uid().toStdString().c_str());
//                    addSaintUid(model->uid());
//                } else {
//                    loge("Saint '%s' not found in db", name.toStdString().c_str());
//                    ret = ErrNotFound;
//                    break;
//                }
//            }
//        }
//    }
//    tracedr(ret);
//    return ret;
//}

const QStringList &Person::saintUidList() const
{
    return mSaintUidList;
}

QString Person::saintUidListInString()
{
    traced;
    QString val;
    if (mSaintUidList.count() > 0){
        val = mSaintUidList.join(HOLLYNAME_SPLIT);
    }
    logd("UidList in string %s", val.toStdString().c_str());
    return val;
}

void Person::setSaintUidList(const QStringList &newSaintUidList)
{
//    mSaintUidList = newSaintUidList;
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mSaintUidList, newSaintUidList, KItemHolly);
    // TODO: get name????
}

void Person::setSaintUidList(const QString &newSaintUidList)
{
    traced;
    setSaintUidList(newSaintUidList.split(HOLLYNAME_SPLIT));
}

void Person::clearSaintUid()
{
    mSaintUidList.clear();
}

void Person::addSaintUid(const QString &saint)
{
    if (!mSaintUidList.contains(saint)) {
        mSaintUidList.append(saint);
        markItemAsModified(KItemHolly);
    }
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
