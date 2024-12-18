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
#include "datetimeutils.h"
#include "filectl.h"
#include "dataexporter.h"
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

#include "prebuiltdefs.h"
#include "imagedefs.h"
#include "personevent.h"
#include "personctl.h"

#include "stringdefs.h"
#include <QUuid>
#include "modeldefs.h"

#define SPLIT_EMAIL_TEL ";"
// TODO: show person code instead of uid?? uid should use for debug only?
#define EXPORT_PERSON_INFO_COMMON_IMPL(item, uid, name) \
    logd("export %s", STR2CHA(item));\
    logd("uid %s", STR2CHA(uid));\
    logd("name %s", STR2CHA(name));\
    QString val; \
    if (!uid.isEmpty()) { \
        val = QString(QObject::tr("%1 (Mã định danh: %2)")).arg(name, uid); \
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
    , mIsDeleteImg(false)
    , mPersonEventListUpdated(false)
{
    init();
}


Person::~Person()
{
    traced;
    // TODO: free resource

    RELEASE_HASH(mPersonEventList, QString, DbModel);
}

void Person::_copyData(const DbModel& model)
{
    tracein;
    const Person* per = static_cast<const Person*>(&model);

    setFirstName(per->firstName());
    setLastName(per->lastName());

    setBirthday(per->birthday());
    setBirthPlace(per->birthPlace());

    setHollyName(per->hollyName());
    setSaintUidList(per->saintUidList());
    setSaintUidNameMap(per->saintUidNameMap());

    setFeastDay(per->feastDay());

    setImgId(per->mImgId);
    mImage.copy(per->mImage);
    mIsDeleteImg = per->mIsDeleteImg;

    setNationalityUid(per->nationalityUid());
    setNationalityName(per->nationalityName());

    setEthnicUid(per->ethnicUid());
    setEthnicName(per->ethnicName());

    setIdCard(per->idCard());
    setIdCardIssuePlace(per->idCardIssuePlace());
    setIdCardIssueDate(per->idCardIssueDate());

    setEduUid(per->eduUid());
    setEduName(per->eduName());
    setEduDetail(per->eduDetail());

    setSpecialistUidList(per->specialistUidList());
    setSpecialistNameList(per->specialistNameList());
    setSpecialistInfo(per->specialistInfo());

    setCourseUid(per->courseUid());
    setCourseName(per->courseName());

    setCountryUid(per->countryUid());
    setCountryName(per->countryName());

    setProvinceUid(per->provinceUid());
    setProvinceName(per->provinceName());

    setAddr(per->addr());
    setChurchAddr(per->churchAddr());
    setEmail(per->email());
    setTel(per->tel());
    setOtherContact(per->otherContact());

    setDadName(per->dadName());
    setDadBirthday(per->dadBirthday());
    setDadAddr(per->dadAddr());

    setMomName(per->momName());
    setMomBirthday(per->momBirthday());
    setMomAddr(per->momAddr());

    setFamilyHistory(per->familyHistory());
    setFamilyContact(per->familyContact());

    setChristenDate(per->christenDate());
    setChristenPlace(per->christenPlace());

    setEucharistPlace(per->eucharistPlace());
    setEucharistDate(per->eucharistDate());

    setHollyPlace(per->hollyPlace());
    setHollyDate(per->hollyDate());

    setJoinDate(per->joinDate());
    setJoinPICUid(per->joinPICUid());
    setJoinPICName(per->joinPICName());
    setJoinPICNameId(per->joinPICNameId());

    setPreTrainJoinDate(per->preTrainJoinDate());
    setPreTrainPICUid(per->preTrainPICUid());
    setPreTrainPICName(per->preTrainPICName());
    setPreTrainPICNameId(per->preTrainPICNameId());

    setTrainJoinDate(per->trainJoinDate());
    setTrainPICUid(per->trainPICUid());
    setTrainPICName(per->trainPICName());
    setTrainPICNameId(per->trainPICNameId());

    setVowsDate(per->vowsDate());
    setVowsCEOUid(per->vowsCEOUid());
    setVowsCEOName(per->vowsCEOName());
    setVowsCEONameId(per->vowsCEONameId());

    setEternalVowsDate(per->eternalVowsDate());
    setEternalVowsCEOUid(per->eternalVowsCEOUid());
    setEternalVowsCEOName(per->eternalVowsCEOName());
    setEternalVowsCEONameId(per->eternalVowsCEONameId());
    setEternalVowsPICUid(per->eternalVowsPICUid());
    setEternalVowsPICName(per->eternalVowsPICName());
    setEternalVowsPICNameId(per->eternalVowsPICNameId());

    setBankDate(per->bankDate());
    setBankPlace(per->bankPlace());

    setGoldenDate(per->goldenDate());
    setGoldenPlace(per->goldenPlace());

    setEternalDate(per->eternalDate());
    setEternalPlace(per->eternalPlace());

    setPersonStatusUid(per->personStatusUid());
    setPersonStatusName(per->personStatusName());

    setRetireDate(per->retireDate());
    setRetirePlace(per->retirePlace());

    setDeadDate(per->deadDate());
    setDeadPlace(per->deadPlace());

    setEventUidList(per->eventUidList());

    setCommunityUid(per->communityUid());
    setCommunityName(per->communityName());
    setCommunityNameId(per->communityNameId());

    setCurrentWorkUid(per->currentWorkUid());
    setCurrentWorkName(per->currentWorkName());
    setWorkHistory(per->workHistory());

    setPersonEventList(const_cast<Person*>(per)->personEventList());

    setPersonEventListUpdated(per->mPersonEventListUpdated);

    traceout;
}

void Person::buildUidIfNotSet()
{
    traced;
    // do nothing, as uild will be built later
}

QString Person::buildUid(const QString *seed)
{
    tracein;
    QUuid uuid = QUuid::createUuid();
    QString uid;
    // FIXME: there are some case that this is not suitable, i.e. info like name
    // is change, no birthday/birthplace info, etc.
    QString uidName = QString("%1_%2_%3").arg(fullName()).arg(birthday()).arg(birthPlace());
    if (seed != nullptr) {
        uidName += "_" + *seed;
    }
    if (!uuid.isNull()) {
        uidName += uuid.toString();
    }
    logd("uidName: %s", STR2CHA(uidName));
    uid = Utils::UidFromName(uidName, UidNameConvertType::HASH_NAME);
    logd("uid: %s", STR2CHA(uid));
    traceout;
    return uid;
}

QString Person::modelName() const
{
    return KModelNamePerson;
}

QString Person::toString() const
{
    QString str = DbModel::toString();
    str += QString(":name('%1')").arg(displayName());
    str += QString(":imageid('%1')").arg(imgId());
    str += QString(":imagepath('%1')").arg(imgPath());
    str += QString(":event('%1')").arg(mPersonEventList.size());
    return str;
}

// cannot override name here, as this return a reference
// but name of person is constructed from last name & first name,
// which cannot return from reference, but value
// due to stack issue
//const QString &Person::name() const
//{
//    return displayName();
//}

DbModel* Person::build(){
    Person* item = new Person();
    item->init();
    return item;
}

DbModelBuilder Person::getBuilder() const
{
    return &Person::build;
}

void Person::initExportFields()
{
    tracein;
    DbModel::initExportFields();
    // TODO: took to much memory for this, should put in in PersonCtl????
    mExportCallbacks.insert(KItemImgPath, [](const DbModel* model, const QString& item){
        QString imgPath = ((Person*)model)->imgPath();
        if (imgPath.isEmpty()){
            logd("Image path is empty, use default one");
            imgPath = ":/icon/icon/unknown.png";
        }
        logd("imgPath: '%s'", STR2CHA(imgPath));
        return imgPath;
    });
    mExportCallbacks.insert(KItemFullName, [](const DbModel* model, const QString& item){
        QString name;
        if (!((Person*)model)->lastName().isEmpty()){
            name = ((Person*)model)->lastName();
        }
        if (!name.isEmpty()){
            name += ",";
        }
        name += ((Person*)model)->firstName();
        return name;

    });
    mExportCallbacks.insert(KItemBirthday, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->birthday());});
    mExportCallbacks.insert(KItemBirthplace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->birthPlace();
    });
    mExportCallbacks.insert(KItemHollyName, [](const DbModel* model, const QString& item){
        return ((Person*)model)->hollyName();
    });
    mExportCallbacks.insert(KItemFeastDay, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->feastDay(), DEFAULT_FORMAT_MD);
    });
    mExportCallbacks.insert(KItemNationality, [](const DbModel* model, const QString& item){
        return ((Person*)model)->nationalityName();
    });
    mExportCallbacks.insert(KItemEthnic, [](const DbModel* model, const QString& item){
        return ((Person*)model)->ethnicName();
    });
    mExportCallbacks.insert(KItemIDcard, [](const DbModel* model, const QString& item){
        return ((Person*)model)->idCard();
    });
    mExportCallbacks.insert(KItemIDcardIssueDate,  [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->idCardIssueDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemIDcardIssuer,  [](const DbModel* model, const QString& item){
        return ((Person*)model)->idCardIssuePlace();
    });
    mExportCallbacks.insert(KItemStatus, [](const DbModel* model, const QString& item){
        return ((Person*)model)->personStatusName();
    });
    mExportCallbacks.insert(KItemRetireDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->retireDate(), DEFAULT_FORMAT_YMD);
    });
    mExportCallbacks.insert(KItemRetirePlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->retirePlace();
    }); //"retire_place";
    mExportCallbacks.insert(KItemDeadDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->deadDate(), DEFAULT_FORMAT_YMD);
    }); //"dead_date";
    mExportCallbacks.insert(KItemDeadPlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->deadPlace();
    }); //"dead_place";
    mExportCallbacks.insert(KItemCountry, [](const DbModel* model, const QString& item){
        return ((Person*)model)->countryName();
    }); //"country";
    mExportCallbacks.insert(KItemProvince, [](const DbModel* model, const QString& item){
        return ((Person*)model)->provinceName();
    }); //"province";
    mExportCallbacks.insert(KItemAddress, [](const DbModel* model, const QString& item){
        return ((Person*)model)->addr();
    }); //"address";
    mExportCallbacks.insert(KItemChurchAddress, [](const DbModel* model, const QString& item){
        return ((Person*)model)->churchAddr();
    }); //"church_addr";
    mExportCallbacks.insert(KItemTel, [](const DbModel* model, const QString& item){
        return ((Person*)model)->tel().join(";");
    }); //"telephone";
    mExportCallbacks.insert(KItemEmail, [](const DbModel* model, const QString& item){
        return ((Person*)model)->email().join(";");
    }); //"email";
    mExportCallbacks.insert(KItemOtherContact, [](const DbModel* model, const QString& item){
        return ((Person*)model)->otherContact();
    }); //"othercontact";
    mExportCallbacks.insert(KItemEdu, [](const DbModel* model, const QString& item){
        return ((Person*)model)->eduName();
    }); //"education";
    mExportCallbacks.insert(KItemEduDetail, [](const DbModel* model, const QString& item){
        return ((Person*)model)->eduDetail();
    }); //"education detail";
    mExportCallbacks.insert(KItemEduNameId, [](const DbModel* model, const QString& item){
        QString nameid;
        if (model && IS_MODEL_NAME(model, KModelNamePerson)) {
            const Person* per = static_cast<const Person*>(model);
            if (!per->eduUid().isEmpty()) {
                DbModel* edu = EDUCTL->getModelByUid(per->eduUid());
                if (edu) {
                    nameid = edu->nameId();
                    delete edu;
                } else {
                    logw("Not found edu uid %s", STR2CHA(((Person*)model)->eduUid()));
                }
            } else {
                logw("empty edu uid");
            }
        } else {
            loge("Invalid model '%s'", MODELSTR2CHA(model));
        }
        return nameid;
    }); //"edu uid";
    mExportCallbacks.insert(KItemSpecialist,  [](const DbModel* model, const QString& item){
        logd("get specialist info to export");
        QList<DbModel*> list = ((Person*)model)->getSpecialistList();
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
    mExportCallbacks.insert(KItemSpecialistInfo, [](const DbModel* model, const QString& item){
        return ((Person*)model)->specialistInfo();
    }); //"specialist detail";
    mExportCallbacks.insert(KItemSpecialistUid, [](const DbModel* model, const QString& item){
        QStringList list = ((Person*)model)->specialistUidList();
        if (list.count() > 0)
            return list.join(",");
        else
            return QString();
    }); //"specialist uid";
    mExportCallbacks.insert(KItemWork, [](const DbModel* model, const QString& item){
        return ((Person*)model)->currentWorkName();
    }); //"work";
    mExportCallbacks.insert(KItemEducation, [](const DbModel* model, const QString& item){
        return ((Person*)model)->eduName();
    }); //"edu";
    mExportCallbacks.insert(KItemWorkHistory, [](const DbModel* model, const QString& item){
        return ((Person*)model)->workHistory();
    }); //"work_history";
    mExportCallbacks.insert(KItemCommunity, [](const DbModel* model, const QString& item){
        return ((Person*)model)->communityName();
    }); //"community";
    mExportCallbacks.insert(KItemCommunityNameId, [](const DbModel* model, const QString& item){
        return ((Person*)model)->communityNameId();
    }); //"community";
    // TODO: community history
    mExportCallbacks.insert(KItemCommunityHistory, nullptr); //"community_history";
    mExportCallbacks.insert(KItemDad, [](const DbModel* model, const QString& item){
        return ((Person*)model)->dadName();
    }); //"dad";
    mExportCallbacks.insert(KItemDadBirthday, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->dadBirthday(), DEFAULT_FORMAT_YMD);
    }); //"dad_birthday";
    mExportCallbacks.insert(KItemDadAddr, [](const DbModel* model, const QString& item){
        return ((Person*)model)->dadAddr();
    }); //"dad_addr";
    mExportCallbacks.insert(KItemMom, [](const DbModel* model, const QString& item){
        return ((Person*)model)->momName();
    }); //"mom";
    mExportCallbacks.insert(KItemMomBirthday, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->momBirthday(), DEFAULT_FORMAT_YMD);
    }); //"mom_birthday";
    mExportCallbacks.insert(KItemMomAddr, [](const DbModel* model, const QString& item){
        return ((Person*)model)->momAddr();
    }); //"mom_addr";
    mExportCallbacks.insert(KItemFamilyHistory, [](const DbModel* model, const QString& item){
        return ((Person*)model)->familyHistory();
    }); //"family_history";
    mExportCallbacks.insert(KItemFamilyContact, [](const DbModel* model, const QString& item){
        return ((Person*)model)->familyContact();
    }); //"family_contact";
    mExportCallbacks.insert(KItemChristenDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->christenDate(), DEFAULT_FORMAT_YMD);
    }); //"christen_date";
    mExportCallbacks.insert(KItemChristenPlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->christenPlace();
    }); //"christen_place";
    mExportCallbacks.insert(KItemEucharistDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->eucharistDate(), DEFAULT_FORMAT_YMD);
    }); //"eucharist_date";
    mExportCallbacks.insert(KItemEucharistPlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->eucharistPlace();
    }); //"eucharist_place";
    mExportCallbacks.insert(KItemHollyDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->hollyDate(), DEFAULT_FORMAT_MD);
    }); //"holly_date";
    mExportCallbacks.insert(KItemHollyPlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->hollyPlace();
    }); //"holly_place";
    mExportCallbacks.insert(KItemCourse, [](const DbModel* model, const QString& item){
        return ((Person*)model)->courseName();
    }); //"course";
    mExportCallbacks.insert(KItemJoinDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->joinDate(), DEFAULT_FORMAT_YMD);
    }); //"join_date";
    mExportCallbacks.insert(KItemJoinPIC, [](const DbModel* model, const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, ((Person*)model)->joinPICNameId(), ((Person*)model)->joinPICName());
    }); //"join_pic";
    mExportCallbacks.insert(KItemPreTrainDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->preTrainJoinDate(), DEFAULT_FORMAT_YMD);
    }); //"pre_train_date";
    mExportCallbacks.insert(KItemPreTrainPIC, [](const DbModel* model, const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, ((Person*)model)->preTrainPICNameId(), ((Person*)model)->preTrainPICName());
    }); //"pre_train_pic";
    mExportCallbacks.insert(KItemTrainDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->trainJoinDate(), DEFAULT_FORMAT_YMD);
    }); //"train_date";
    mExportCallbacks.insert(KItemTrainPIC, [](const DbModel* model, const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, ((Person*)model)->trainPICNameId(), ((Person*)model)->trainPICName());
    }); //"train_pic";
    mExportCallbacks.insert(KItemVowsDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->vowsDate(), DEFAULT_FORMAT_YMD);
    }); //"vows_date";
    mExportCallbacks.insert(KItemVowsCEO, [](const DbModel* model, const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, ((Person*)model)->vowsCEONameId(), ((Person*)model)->vowsCEOName());
    }); //"vows_ceo";
    mExportCallbacks.insert(KItemEternalVowsDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->eternalVowsDate(), DEFAULT_FORMAT_YMD);
    }); //"eternal_vows_date";
    mExportCallbacks.insert(KItemEternalVowsCEO, [](const DbModel* model, const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, ((Person*)model)->eternalVowsCEONameId(), ((Person*)model)->eternalVowsCEOName());
    }); //"eternal_vows_ceo";
    mExportCallbacks.insert(KItemEternalVowsPIC, [](const DbModel* model, const QString& item){
        EXPORT_PERSON_INFO_COMMON_IMPL(item, ((Person*)model)->eternalVowsPICNameId(), ((Person*)model)->eternalVowsPICName());
    }); //"eternal_vows_pic";
    mExportCallbacks.insert(KItemBankDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->bankDate(), DEFAULT_FORMAT_YMD);
    }); //"bank_date";
    mExportCallbacks.insert(KItemBankPlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->bankPlace();
    }); //"bank_place";
    mExportCallbacks.insert(KItemGoldenDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->goldenDate(), DEFAULT_FORMAT_YMD);
    }); //"golden_date";
    mExportCallbacks.insert(KItemGoldenPlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->goldenPlace();
    }); //"golden_place";
    mExportCallbacks.insert(KItemEternalDate, [](const DbModel* model, const QString& item){
        return DatetimeUtils::date2String(((Person*)model)->eternalDate(), DEFAULT_FORMAT_YMD);
    }); //"eternal_date";
    mExportCallbacks.insert(KItemEternalPlace, [](const DbModel* model, const QString& item){
        return ((Person*)model)->eternalPlace();
    }); //"eternal_place";


    mExportCallbacks.insert(KItemPersonEvent, [](const DbModel* model, const QString& item){
        QString events;
        if (model) {
            bool ok = false;
            events = PERSONCTL->getListEventsInString(model->uid(), "\n", &ok);
            if (!ok) {
                loge("failed to get event list for ''%s'", MODELSTR2CHA(model));
                events = STR_DATA_ERROR;
            }
        } else {
            events = STR_ERROR;
        }
        return events;
    }); //"eternal_place";
    traceout;
}

void Person::initImportFields()
{
    tracein;
    DbModel::initImportFields();
    // TODO: check fields like holly name, country, etc. and mark invalid field???

    mImportCallbacks.insert(KItemImgPath, [this](const QString& value){this->setImgPath(value);
        return ErrNone;});
    mImportCallbacks.insert(KItemFullName, [this](const QString& value){
        this->setNameFromFullName(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemBirthday, [this](const QString& value){
        this->setBirthday(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemBirthplace, [this](const QString& value){
        this->setBirthPlace(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemHollyName, [this](const QString& value){
        this->setHollyName(value, true);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemFeastDay, [this](const QString& value){this->setFeastDay(value);
        return ErrNone;});
    mImportCallbacks.insert(KItemNationality, [this](const QString& value){this->setNationalityName(value);
        return ErrNone;});
    mImportCallbacks.insert(KItemEthnicUID, [this](const QString& value){this->setEthnicUid(value);
        return ErrNone;});
    mImportCallbacks.insert(KItemEthnic, [this](const QString& value){this->setEthnicName(value);
        return ErrNone;});
    mImportCallbacks.insert(KItemIDcard, [this](const QString& value){
        this->setIdCard(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemIDcardIssueDate, [this](const QString& value){
        this->setIdCardIssueDate(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemIDcardIssuer, [this](const QString& value){
        this->setIdCardIssuePlace(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemStatus, nullptr); // TODO: implement this
    mImportCallbacks.insert(KItemRetireDate,  [this](const QString& value){
        this->setRetireDate(value);
        return ErrNone;
    });
    mImportCallbacks.insert(KItemRetirePlace, [this](const QString& value){
        this->setRetirePlace(value);
        return ErrNone;
    }); //"retire_place";
    mImportCallbacks.insert(KItemDeadDate, [this](const QString& value){
        this->setDeadDate(value);
        return ErrNone;
    }); //"dead_date";
    mImportCallbacks.insert(KItemDeadPlace, [this](const QString& value){
        this->setDeadPlace(value);
        return ErrNone;
    }); //"dead_place";
    mImportCallbacks.insert(KItemCountry, [this](const QString& value){
        this->setCountryName(value);
        return ErrNone;
    }); //"country";
    mImportCallbacks.insert(KItemProvince, [this](const QString& value){
        this->setProvinceName(value);
        return ErrNone;
    }); //"province";
    mImportCallbacks.insert(KItemAddress, [this](const QString& value){
        this->setAddr(value);
        return ErrNone;
    }); //"address";
    mImportCallbacks.insert(KItemChurchAddress, [this](const QString& value){
        this->setChurchAddr(value);
        return ErrNone;
    }); //"church_addr";
    mImportCallbacks.insert(KItemTel, [this](const QString& value){
        this->setTel(value);
        return ErrNone;
    }); //"telephone";
    mImportCallbacks.insert(KItemEmail, [this](const QString& value){
        this->setEmail(value);
        return ErrNone;
    }); //"email";
    mImportCallbacks.insert(KItemOtherContact, [this](const QString& value){
        this->setOtherContact(value);
        return ErrNone;
    }); //"othercontact";
    mImportCallbacks.insert(KItemEdu, [this](const QString& value){
        this->setEduName(value);
        return ErrNone;
    }); //"education";
    mImportCallbacks.insert(KItemEduNameId, [this](const QString& value){
        logd("Import edu name id %s", STR2CHA(value));
        DbModel* model = EDUCTL->getModelByNameId(value);
        if (model) {
            this->setEduUid(model->uid());
            delete model;
        } else {
            loge("Not found edu nameid %s", STR2CHA(value));
        }
        return ErrNone;
    }); //"education";
    mImportCallbacks.insert(KItemSpecialist, [this](const QString& value){
        this->setSpecialistNames(value, true);
        return ErrNone;
    }); //"specialist";
    mImportCallbacks.insert(KItemSpecialistUid, [this](const QString& value){
        this->setSpecialistUidList(value);
        return ErrNone;
    }); //"specialist";
    mImportCallbacks.insert(KItemWork, [this](const QString& value){
        this->setCurrentWorkName(value);
        return ErrNone;
    }); //"work"; // TODO
    mImportCallbacks.insert(KItemWorkId, [this](const QString& value){
        this->setCurrentWorkUid(value);
        return ErrNone;
    }); //"work"; // TODO
    mImportCallbacks.insert(KItemWorkHistory, [this](const QString& value){
        this->setWorkHistory(value);
        return ErrNone;
    }); //"work_history"; // TODO
    mImportCallbacks.insert(KItemCommunity, [this](const QString& value){
        this->setCommunityName(value);
        return ErrNone;
    }); //"community"; // TODO
    mImportCallbacks.insert(KItemCommunityNameId, [this](const QString& value){
        this->setCommunityUid(value); // TODO: search community base on name id then set uid
        return ErrNone;
    }); //"community"; // TODO
    mImportCallbacks.insert(KItemCommunityHistory, nullptr); //"community_history"; // TODO
    mImportCallbacks.insert(KItemDad, [this](const QString& value){
        this->setDadName(value);
        return ErrNone;
    }); //"dad";
    mImportCallbacks.insert(KItemDadBirthday, [this](const QString& value){
        this->setDadBirthday(value);
        return ErrNone;
    }); //"dad_birthday";
    mImportCallbacks.insert(KItemDadAddr,  [this](const QString& value){
        this->setDadAddr(value);
        return ErrNone;
    }); //"dad_addr";
    mImportCallbacks.insert(KItemMom,  [this](const QString& value){
        this->setMomName(value);
        return ErrNone;
    }); //"mom";
    mImportCallbacks.insert(KItemMomBirthday,  [this](const QString& value){
        this->setMomBirthday(value);
        return ErrNone;
    }); //"mom_birthday";
    mImportCallbacks.insert(KItemMomAddr, [this](const QString& value){
        this->setMomAddr(value);
        return ErrNone;
    }); //"mom_addr";
    mImportCallbacks.insert(KItemFamilyHistory, [this](const QString& value){
        this->setFamilyHistory(value);
        return ErrNone;
    }); //"family_history";
    mImportCallbacks.insert(KItemFamilyContact, [this](const QString& value){
        this->setFamilyContact(value);
        return ErrNone;
    }); //"family_contact";
    mImportCallbacks.insert(KItemChristenDate, [this](const QString& value){
        this->setChristenDate(value);
        return ErrNone;
    }); //"christen_date";
    mImportCallbacks.insert(KItemChristenPlace, [this](const QString& value){
        this->setChristenPlace(value);
        return ErrNone;
    }); //"christen_place";
    mImportCallbacks.insert(KItemEucharistDate, [this](const QString& value){
        this->setEucharistDate(value);
        return ErrNone;
    }); //"eucharist_date";
    mImportCallbacks.insert(KItemEucharistPlace, [this](const QString& value){
        this->setEucharistPlace(value);
        return ErrNone;
    }); //"eucharist_place";
    mImportCallbacks.insert(KItemHollyDate, [this](const QString& value){
        this->setHollyDate(value);
        return ErrNone;
    }); //"holly_date";
    mImportCallbacks.insert(KItemHollyPlace, [this](const QString& value){
        this->setHollyPlace(value);
        return ErrNone;
    }); //"holly_place";
    mImportCallbacks.insert(KItemCourse, [this](const QString& value){
        this->setCourseName(value);
        return ErrNone;
    }); //"course";
    mImportCallbacks.insert(KItemJoinDate, [this](const QString& value){
        this->setJoinDate(value);
        return ErrNone;
    }); //"join_date";
    mImportCallbacks.insert(KItemJoinPIC, nullptr); //"join_pic";
    mImportCallbacks.insert(KItemPreTrainDate,  [this](const QString& value){
        this->setPreTrainJoinDate(value);
        return ErrNone;
    }); //"pre_train_date";
    mImportCallbacks.insert(KItemPreTrainPIC, nullptr); //"pre_train_pic";
    mImportCallbacks.insert(KItemTrainDate, [this](const QString& value){
        this->setTrainJoinDate(value);
        return ErrNone;
    }); //"train_date";
    mImportCallbacks.insert(KItemTrainPIC, nullptr); //"train_pic";
    mImportCallbacks.insert(KItemVowsDate,  [this](const QString& value){
        this->setVowsDate(value);
        return ErrNone;
    }); //"vows_date";
    mImportCallbacks.insert(KItemVowsCEO, nullptr); //"vows_ceo";
    mImportCallbacks.insert(KItemEternalVowsDate,  [this](const QString& value){
        this->setEternalVowsDate(value);
        return ErrNone;
    }); //"eternal_vows_date";
    mImportCallbacks.insert(KItemEternalVowsCEO, nullptr); //"eternal_vows_ceo";
    mImportCallbacks.insert(KItemEternalVowsPIC, nullptr); //"eternal_vows_pic";
    mImportCallbacks.insert(KItemBankDate,  [this](const QString& value){
        this->setBankDate(value);
        return ErrNone;
    }); //"bank_date";
    mImportCallbacks.insert(KItemBankPlace,  [this](const QString& value){
        this->setBankPlace(value);
        return ErrNone;
    }); //"bank_place";
    mImportCallbacks.insert(KItemGoldenDate,  [this](const QString& value){
        this->setGoldenDate(value);
        return ErrNone;
    }); //"golden_date";
    mImportCallbacks.insert(KItemGoldenPlace,  [this](const QString& value){
        this->setGoldenPlace(value);
        return ErrNone;
    }); //"golden_place";
    mImportCallbacks.insert(KItemEternalDate,  [this](const QString& value){
        this->setEternalDate(value);
        return ErrNone;
    }); //"eternal_date";
    mImportCallbacks.insert(KItemEternalPlace,  [this](const QString& value){
        this->setEternalPlace(value);
        return ErrNone;
    }); //"eternal_place";
}

ErrCode Person::commonCheckField(QString& name,
                                 QString& uid,
                                 ModelController* controller,
                                 const char* const itemName,
                                 int& invalidField)
{
    tracein;
    ErrCode ret = ErrNone;

    dbgv("Check item %s", itemName);
    if (name.isEmpty() && uid.isEmpty()) {
        logd("Data of %s empty, nothing to do", itemName);
        ret = ErrNone;
        // TODO: check logic again, all empty mean valid????
    } else {
        DbModel* model = nullptr;
        if (!uid.isEmpty()) {
            dbgd("get model by uid '%s'", STR2CHA(uid));
            model = controller->getModelByUid(uid);
        } else {
            logd("uid is empty");
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
                loge("uid '%s' not found for itemName '%s' info",
                     STR2CHA(uid), itemName);
                appendValidateResult(itemName, ErrNotFound);
                appendValidateMsg(QString("%1 '%2' not found").arg(itemName, name));
                ret = ErrNotFound;
                invalidField++;
            }
        }

    }
    dbgd("item '%s' has invalidField=%d", itemName, invalidField);
    traceret(ret);
    return ret;
}
ErrCode Person::validateAllFields(bool checkExist)
{
    tracein;
    int invaidField = 0;
    // TODO: cached value???
    ErrCode ret = DbModel::validateAllFields(checkExist);
    if (ret == ErrNone) {
        DbModelHandler* personHdlr = this->getDbModelHandler();
        if (personHdlr == nullptr) {
            loge("Invalid Person handler");
            ret = ErrFailed;
        }
    }
    if (ret == ErrNone) {
        // Holly name / saint list
        // Holly name to saint list
        // TODO: saint uid list to holly name???
        if (mSaintUidList.size() == 0 && mHollyName.size() > 0) {
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
        }

        // country
        ErrCode tmpret = commonCheckField(mCountryName, mCountryUid, COUNTRYCTL,
                               KItemCountry, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;
        // TODO: check logic again, all empty mean valid, should have field must has data???????

        // nationality
        tmpret = commonCheckField(mNationalityName, mNationalityUid, COUNTRYCTL,
                               KItemNationality, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;
        // TODO: same ethnic name, but different country???
        tmpret = commonCheckField(mEthnicName, mEthnicUid, ETHNIC,
                               KItemEthnic, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;
        tmpret = commonCheckField(mEduName, mEduUid, EDUCTL,
                               KItemEdu, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;
        tmpret = commonCheckField(mCourse, mCourseUid, COURSECTL,
                               KItemCourse, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;

        // TODO: province: check with country??? as province belong to a country
    #ifndef SKIP_PERSON_PROVINE
        tmpret = commonCheckField(mProvinceName, mProvinceUid, PROVINCE, KItemProvince, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;
    #endif
        tmpret = commonCheckField(mCurrentWorkName, mCurrentWorkUid, WORKCTL,
                               KItemWork, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;

        tmpret = commonCheckField(mCommunityName, mCommunityUid, COMMUNITYCTL,
                               KItemCommunity, invaidField);
        ret = (tmpret != ErrNone)?tmpret:ret;

    }

    if (ret == ErrNone && invaidField > 0) {
        loge("%d invalid field", invaidField);
        ret = ErrInvalidData;
    }
    traceret(ret);
    return ret;
}

ErrCode Person::fromCSVFile(const QString &fname)
{
    tracein;
    QList<QHash<QString, QString>> items;
    QHash<QString, QString> item;
    logi("parse csv file %s", STR2CHA(fname));
    ErrCode ret = Utils::parseDataFromCSVFile(fname, &items);
    if (ret == ErrNone){
        logi("Parsed %lld key", items.size());
        // TODO: fixme if it's actually suitable??
        if (items.size() > 0) {
            item = items.at(0); // use 1st one only
        }
        if (item.contains(KCsvItemName)){
            logd("Set name");
            setName(item.value(KCsvItemName));
        }
        if (item.contains(KCsvItemNameId)){
            logd("Set code");
            setNameId(item.value(KCsvItemNameId));
        }
        if (item.contains(KCsvItemChristenDate)){
            logd("Set christenDate");
            setChristenDate(item.value(KCsvItemChristenDate));
        }
    }

    traceret(ret);
    return ret;

}

const DataExporter *Person::getExporter() const
{
    return this;
}

const QString &Person::firstName() const
{
    return mFirstName;
}

void Person::setFirstName(const QString &newFirstName)
{
    CHECK_MODIFIED_THEN_SET(mFirstName, newFirstName, KItemFullName);
}


const QString &Person::birthPlace() const
{
    return mBirthPlace;
}

void Person::setBirthPlace(const QString &newBirthPlace)
{
    CHECK_MODIFIED_THEN_SET(mBirthPlace, newBirthPlace, KItemBirthplace);
}

qint64 Person::birthday() const
{
    return mBirthday;
}

void Person::setBirthday(qint64 newBirthday)
{
    CHECK_MODIFIED_THEN_SET(mBirthday, newBirthday, KItemBirthday);
}

void Person::setBirthday(const QString &newBirthday)
{
    tracein;
    bool isOk = false;
    qint64 date = DatetimeUtils::dateFromString(newBirthday, DEFAULT_FORMAT_YMD, &isOk);
    logd("conver '%s', isOk=%d, value %d", newBirthday.toStdString().c_str(),
         isOk, (int)date);
    if (isOk && date > 0)
        setBirthday(date);
    else
        setBirthday(0); // TODO: should set 0 or return error????
    traceout;
}

const QString &Person::lastName() const
{
    return mLastName;
}

void Person::setLastName(const QString &newLastName)
{
    CHECK_MODIFIED_THEN_SET(mLastName, newLastName, KItemFullName);
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



QString Person::fullName() const
{
    //TODO:check localization
    return FULLNAME(firstName(), lastName());
}

QString Person::displayName() const
{
    return QString("%1 %2").arg(hollyName(), fullName());
}

qint64 Person::christenDate() const
{
    return mChristenDate;
}

void Person::setChristenDate(qint64 newChristenDate)
{
    CHECK_MODIFIED_THEN_SET(mChristenDate, newChristenDate, KItemChristenDate);
}

void Person::setChristenDate(const QString &newChristenDate,
                             const QString& format)
{
    qint64 christenDate = DatetimeUtils::dateFromString(newChristenDate,format);
    logd("mChristenDate %s -> %lld", STR2CHA(newChristenDate), christenDate);
    setChristenDate(christenDate);
}

ErrCode Person::exportTo(const QString &fpath, ExportType type)
{
    tracein;
    ErrCode ret = ErrNone;
    // TODO: implement something here
    logi("Export Person to %s", fpath.toStdString().c_str());
    ret = ExportFactory::exportTo(this, getName(), fpath, type);
    traceret(ret);
    return ret;

}

DbModelHandler *Person::getDbModelHandler() const
{
    return DB->getModelHandler(KModelHdlPerson);
}

ErrCode Person::exportTemplatePath(FileExporter* exporter,
                                   const QString& name,
                                 QString& fpath,
                                 QString* ftype) const
{
    tracein;
    ErrCode err = ErrNone;

    fpath = FileCtl::getPrebuiltDataFilePath(KPrebuiltPersonInfoTemplateFileName);
    traceret(err);
    return err;
}

ErrCode Person::prepare2Save()
{
    tracein;
    DbModelHandler* saintHdlr = nullptr;
    ErrCode ret = DbModel::prepare2Save();
    if (ret == ErrNone) {
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
    traceret(ret);
    return ret;
}

void Person::setPersonEventListUpdated(bool newPersonEventListUpdated)
{
    mPersonEventListUpdated = newPersonEventListUpdated;
}

bool Person::personEventListUpdated() const
{
    return mPersonEventListUpdated;
}

void Person::check2ReloadPersonEventList(bool reload)
{
    tracein;
    logd("mPersonEventListUpdated %d", mPersonEventListUpdated);
    logd("reload %d", reload);
    if (!mPersonEventListUpdated || reload) {
        QList<DbModel*> list;
        ErrCode err = PERSONCTL->getListEvents(uid(), list);
        if (err == ErrNone) {
            setPersonEventList(list);
            logd("no of person event %lld", mPersonEventList.size());
        } else {
            loge("failed to load person event ret = %d", err);
        }
    }
    traceout;
}

const QList<DbModel *> Person::personEventList(bool reload)
{
    tracein;
    check2ReloadPersonEventList(reload);
    traceout;
    return mPersonEventList.values();
}

const QHash<QString, DbModel *> Person::personEventMap(bool reload)
{
    tracein;
    check2ReloadPersonEventList(reload);
    traceout;
    return mPersonEventList;
}

void Person::setPersonEventList(const QList<DbModel *> &newPersonEventList)
{
    tracein;
    RELEASE_HASH(mPersonEventList, QString, DbModel);
    logd("set %lld events", newPersonEventList.size());
    foreach (DbModel* model, newPersonEventList) {
        addPersonEvent(model);
    }
    traceout;
}

ErrCode Person::addPersonEvent(const DbModel *event)
{
    tracein;
    ErrCode err = ErrNone;
    if (event) {
        logd("clone event '%s'", STR2CHA(event->toString()));
        DbModel* newEvent = CLONE_DBMODEL(event);
        if (newEvent) {
            mPersonEventList.insert(newEvent->nameId(), newEvent);
            markItemAsModified(KItemPersonEvent);
            mPersonEventListUpdated = true;
        } else {
            err = ErrNoMemory;
            loge("failed to allocate memory for new event '%s'", STR2CHA(event->toString()));
        }
    } else {
        loge("No event to add");
        err = ErrInvalidArg;
    }
    traceret(err);
    return err;
}

ErrCode Person::delPersonEvent(const DbModel *event)
{
    tracein;
    ErrCode err = ErrNone;
    if (!event) {
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone && mPersonEventList.size() > 0) {
        int idx = 0;
        DbModel* item = nullptr;
        QString foundKey;
        foreach (QString key, mPersonEventList.keys()) {
            DbModel* item = mPersonEventList.value(key);
            if (item) {
                if (!event->uid().isEmpty() &&
                    (event->uid() == item->uid())) {
                    foundKey = key;
                    break;
                } else if (event->nameId() == item->nameId()) {
                    foundKey = key;
                    break;
                }
            }
        }
        logd("idx=%d", idx);
        if (!foundKey.isEmpty()) {
            if (item) {
                logd("deleted item '%s'", STR2CHA(item->toString()));
                delete item;
            }
            logd("Remove foundKey=%s", STR2CHA(foundKey));
            mPersonEventList.remove(foundKey);
            mPersonEventListUpdated = true;
        }
    }
    traceret(err);
    return err;
}

const QString &Person::communityNameId() const
{
    return mCommunityNameId;
}

void Person::setCommunityNameId(const QString &newCommunityNameId)
{
    mCommunityNameId = newCommunityNameId;
}

ErrCode Person::save(bool notifyDataChange)
{
    tracein;
    ErrCode err = DbModel::save(notifyDataChange);
    if (err == ErrNone && !mImage.fullImgPath().isEmpty()) {
        logd("save image");
        err = mImage.save();
        if (err != ErrNone) {
            loge("Save image, failed, err=%d", err);
            // still ok event save image failed, as data in db is more important
            err = ErrNone; // TODO: handle error case?
        }
    }
    traceret(err);
    return err;
}

ErrCode Person::update(bool allFields, bool notifyDataChange)
{
    tracein;
    ErrCode err = ErrNone;
    bool pendingDelImg = false;
    bool imageSaved = false;
    if ((isFieldUpdated(KItemImg) || allFields)) {
        if (!mIsDeleteImg) {
            logd("update image");
            err = mImage.save();
            imageSaved = true;
            if (err != ErrNone) {
                loge("Save image, failed, err=%d", err);
                err = ErrNone; // TODO: handle error case?
            }
        } else {
            logd("mark to remove image");
            pendingDelImg = true;
        }
    }
    // there is a risk that while storing, notification is raised, but image not store
    // yet, causing data mark that image file not exist
    // therefore, we save first, then update later, if error, just remove it
    if (err == ErrNone) {
        err = DbModel::update(allFields, notifyDataChange);
    }
    // delete image if pending dele image, or error and image saved
    if ((err == ErrNone && pendingDelImg) || (err != ErrNone && imageSaved)) {
        dbgd("remove image, err=%d, pendingDelImg=%d, imageSaved=%d",
             err, pendingDelImg, imageSaved);
        mImage.remove(true);
    }
    traceret(err);
    return err;

}

ErrCode Person::remove(bool force, QString *msg)
{
    tracein;
    ErrCode err = DbModel::remove(force, msg);
    if (err == ErrNone) {
        logd("remove image");
        mImage.remove(true);
    }
    traceret(err);
    return err;

}

const QString &Person::trainPICNameId() const
{
    return mTrainPICNameId;
}

void Person::setTrainPICNameId(const QString &newTrainPICNameId)
{
    mTrainPICNameId = newTrainPICNameId;
}

const QString &Person::joinPICNameId() const
{
    return mJoinPICNameId;
}

void Person::setJoinPICNameId(const QString &newJoinPICNameId)
{
    mJoinPICNameId = newJoinPICNameId;
}

const QString &Person::preTrainPICNameId() const
{
    return mPreTrainPICNameId;
}

void Person::setPreTrainPICNameId(const QString &newPreTrainPICNameId)
{
    mPreTrainPICNameId = newPreTrainPICNameId;
}

const QString &Person::vowsCEONameId() const
{
    return mVowsCEONameId;
}

void Person::setVowsCEONameId(const QString &newVowsCEONameId)
{
    mVowsCEONameId = newVowsCEONameId;
}

const QString &Person::eternalVowsCEONameId() const
{
    return mEternalVowsCEONameId;
}

void Person::setEternalVowsCEONameId(const QString &newEternalVowsCEONameId)
{
    mEternalVowsCEONameId = newEternalVowsCEONameId;
}

const QString &Person::eternalVowsPICNameId() const
{
    return mEternalVowsPICNameId;
}

void Person::setEternalVowsPICNameId(const QString &newEternalVowsPICNameId)
{
    mEternalVowsPICNameId = newEternalVowsPICNameId;
}

const QString &Person::workHistory() const
{
    return mWorkHistory;
}

void Person::setWorkHistory(const QString &newWorkHistory)
{
    CHECK_MODIFIED_THEN_SET(mWorkHistory, newWorkHistory, KItemWorkHistory);
}

const QHash<QString, ExportCallbackFunc > &Person::exportFields() const
{
    return mExportCallbacks;
}

const QString &Person::currentWorkName() const
{
    return mCurrentWorkName;
}

void Person::setCurrentWorkName(const QString &newCurrentWorkName)
{
    mCurrentWorkName = newCurrentWorkName;
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
    CHECK_MODIFIED_THEN_SET(mSpecialistInfo, newSpecialistInfo, KItemSpecialistInfo);
}

const QString &Person::eduDetail() const
{
    return mEduDetail;
}

void Person::setEduDetail(const QString &newEduDetail)
{
    CHECK_MODIFIED_THEN_SET(mEduDetail, newEduDetail, KItemEduDetail);
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
    CHECK_MODIFIED_THEN_SET(mCommunityUid, newCommunityUid, KItemCommunity);
}

const QString &Person::nationalityName() const
{
    return mNationalityName;
}

void Person::setNationalityName(const QString &newNationalityName)
{
    CHECK_MODIFIED_THEN_SET(mNationalityName, newNationalityName, KItemNationality);
}

void Person::dump() const
{
    tracein;
    DbModel::dump();
    logd("- FullName %s", fullName().toStdString().c_str());
    logd("- FirstName %s", firstName().toStdString().c_str());
    logd("- LastName %s", lastName().toStdString().c_str());
    logd("- HollyName %s", hollyName().toStdString().c_str());
    logd("- JoinPIC Name %s", joinPICName().toStdString().c_str());
    logd("- JoinPIC Uid %s", joinPICUid().toStdString().c_str());
    tracein;
}

qint64 Person::feastDay() const
{
    return mFeastDay;
}

void Person::setFeastDay(qint64 newFeastDay)
{
    CHECK_MODIFIED_THEN_SET(mFeastDay, newFeastDay, KItemFeastDay);
}

void Person::setFeastDay(const QString &newFeastDay,
                         const QString& format)
{
    setFeastDay(DatetimeUtils::dateFromString(newFeastDay, format));
}

const QString &Person::eucharistPlace() const
{
    return mEucharistPlace;
}

void Person::setEucharistPlace(const QString &newEucharistPlace)
{
    CHECK_MODIFIED_THEN_SET(mEucharistPlace, newEucharistPlace, KItemEucharistPlace);
}

qint64 Person::eucharistDate() const
{
    return mEucharistDate;
}

void Person::setEucharistDate(qint64 newEucharistDate)
{
    CHECK_MODIFIED_THEN_SET(mEucharistDate, newEucharistDate, KItemEucharistDate);
}

void Person::setEucharistDate(const QString &newEucharistDate, const QString &format)
{
    setEucharistDate(DatetimeUtils::dateFromString(newEucharistDate, format));
}

const QString &Person::otherContact() const
{
    return mOtherContact;
}

void Person::setOtherContact(const QString &newOtherContact)
{
    CHECK_MODIFIED_THEN_SET(mOtherContact, newOtherContact, KItemOtherContact);
}


const QStringList &Person::eventUidList() const
{
    return mEventUidList;
}

void Person::setEventUidList(const QStringList &newEventUidList)
{
    CHECK_MODIFIED_THEN_SET(mEventUidList, newEventUidList, KItemEvent);
}

void Person::addEventUid(const QString &eventUid)
{
    mEventUidList.append(eventUid);
}

QString Person::imgId() const
{
    if (!mIsDeleteImg) {
        return mImgId.isEmpty()?mImage.uid():mImgId;
    } else {
        return "";
    }
}

void Person::setImgId(const QString &newImgId)
{
    tracein;
    CHECK_MODIFIED_THEN_SET(mImgId, newImgId, KItemImg);
    if (!mImgId.isEmpty()) {
        logd("set image for person '%s'", MODELSTR2CHA(this));
        ErrCode err = mImage.loadImageFromUid(newImgId, IMG_TAG_PEOPLE);
        if (err == ErrNone) {
            markItemAsModified(KItemImg);
        } else {
            loge("load image from uid '%s' failed", STR2CHA(newImgId));
        }
    } else {
        logw("img id is empty, nothing to load");
        // TODO: remove existing file???
        markItemAsModified(KItemImg);
    }
    traceout;
    //    mImgId = newImgId;
}

void Person::markImageDelete(bool del)
{
    mIsDeleteImg = del;
}

const QString &Person::deadPlace() const
{
    return mDeadPlace;
}

void Person::setDeadPlace(const QString &newDeadPlace)
{
    CHECK_MODIFIED_THEN_SET(mDeadPlace, newDeadPlace, KItemDeadPlace);
}

qint64 Person::deadDate() const
{
    return mDeadDate;
}

void Person::setDeadDate(qint64 newDeadDate)
{
    CHECK_MODIFIED_THEN_SET(mDeadDate, newDeadDate, KItemDeadDate);
}

void Person::setDeadDate(const QString &newDeadDate, const QString &format)
{
    setDeadDate(DatetimeUtils::dateFromString(newDeadDate, format));
}

const QString &Person::retirePlace() const
{
    return mRetirePlace;
}

void Person::setRetirePlace(const QString &newRetirePlace)
{
    CHECK_MODIFIED_THEN_SET(mRetirePlace, newRetirePlace, KItemRetirePlace);
}

qint64 Person::retireDate() const
{
    return mRetireDate;
}

void Person::setRetireDate(qint64 newRetireDate)
{
    CHECK_MODIFIED_THEN_SET(mRetireDate, newRetireDate, KItemRetireDate);
}

void Person::setRetireDate(const QString &newRetireDate, const QString &format)
{
    tracein;
    setRetireDate(DatetimeUtils::dateFromString(newRetireDate, format));
}

const QString &Person::personStatusName() const
{
    return mPersonStatusName;
}

void Person::setPersonStatusName(const QString &newStatusName)
{
    CHECK_MODIFIED_THEN_SET(mPersonStatusName, newStatusName, KItemStatus);
}

const QString &Person::personStatusUid() const
{
    return mPersonStatusUid;
}

void Person::setPersonStatusUid(const QString &newStatusUid)
{
    CHECK_MODIFIED_THEN_SET(mPersonStatusUid, newStatusUid, KItemStatus);
}

const QString &Person::eternalPlace() const
{
    return mEternalPlace;
}

void Person::setEternalPlace(const QString &newEternalPlace)
{
    CHECK_MODIFIED_THEN_SET(mEternalPlace, newEternalPlace, KItemEternalPlace);
}

qint64 Person::eternalDate() const
{
    return mEternalDate;
}

void Person::setEternalDate(qint64 newEternalDate)
{
    CHECK_MODIFIED_THEN_SET(mEternalDate, newEternalDate, KItemEternalDate);
}

void Person::setEternalDate(const QString &newEternalDate, const QString &format)
{
    setEternalDate(DatetimeUtils::dateFromString(newEternalDate, format));
}

const QString &Person::goldenPlace() const
{
    return mGoldenPlace;
}

void Person::setGoldenPlace(const QString &newGoldenPlace)
{
    CHECK_MODIFIED_THEN_SET(mGoldenPlace, newGoldenPlace, KItemGoldenPlace);
}

qint64 Person::goldenDate() const
{
    return mGoldenDate;
}

void Person::setGoldenDate(qint64 newGoldenDate)
{
    CHECK_MODIFIED_THEN_SET(mGoldenDate, newGoldenDate, KItemGoldenDate);
}

void Person::setGoldenDate(const QString &newGoldenDate, const QString &format)
{
    setGoldenDate(DatetimeUtils::dateFromString(newGoldenDate, format));
}

const QString &Person::bankPlace() const
{
    return mBankPlace;
}

void Person::setBankPlace(const QString &newBankPlace)
{
    CHECK_MODIFIED_THEN_SET(mBankPlace, newBankPlace, KItemBankPlace);
}

qint64 Person::bankDate() const
{
    return mBankDate;
}

void Person::setBankDate(qint64 newBankDate)
{
    CHECK_MODIFIED_THEN_SET(mBankDate, newBankDate, KItemBankDate);
}

void Person::setBankDate(const QString &newBankDate, const QString &format)
{
    setBankDate(DatetimeUtils::dateFromString(newBankDate, format));
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
    CHECK_MODIFIED_THEN_SET(mEternalVowsPICUid, newEternalVowsPICUid, KItemEternalVowsPIC);
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
    CHECK_MODIFIED_THEN_SET(mEternalVowsCEOUid, newEternalVowsCEOUid, KItemEternalVowsCEO);
}

qint64 Person::eternalVowsDate() const
{
    return mEternalVowsDate;
}

void Person::setEternalVowsDate(qint64 newEternalVowsDate)
{
    CHECK_MODIFIED_THEN_SET(mEternalVowsDate, newEternalVowsDate, KItemEternalVowsDate);
}

void Person::setEternalVowsDate(const QString &newEternalVowsDate, const QString &format)
{
    setEternalVowsDate(DatetimeUtils::dateFromString(newEternalVowsDate, format));
}

qint64 Person::vowsDate() const
{
    return mVowsDate;
}

void Person::setVowsDate(qint64 newVowsDate)
{
    CHECK_MODIFIED_THEN_SET(mVowsDate, newVowsDate, KItemVowsDate);
}

void Person::setVowsDate(const QString &newVowsDate, const QString &format)
{
    setVowsDate(DatetimeUtils::dateFromString(newVowsDate, format));
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
    CHECK_MODIFIED_THEN_SET(mVowsCEOUid, newVowsCEOUid, KItemVowsCEO);
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
    CHECK_MODIFIED_THEN_SET(mTrainPICUid, newTrainPICUid, KItemTrainPIC);
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
    CHECK_MODIFIED_THEN_SET(mPreTrainPICUid, newPreTrainPICUid, KItemPreTrainPIC);
}

qint64 Person::preTrainJoinDate() const
{
    return mPreTrainJoinDate;
}

void Person::setPreTrainJoinDate(qint64 newPreTrainJoinDate)
{
    CHECK_MODIFIED_THEN_SET(mPreTrainJoinDate, newPreTrainJoinDate, KItemPreTrainDate);
}

void Person::setPreTrainJoinDate(const QString &newPreTrainJoinDate, const QString &format)
{
    setPreTrainJoinDate(DatetimeUtils::dateFromString(newPreTrainJoinDate, format));
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
    CHECK_MODIFIED_THEN_SET(mJoinPICUid, newJoinPICUid, KItemJoinPIC);
}

qint64 Person::joinDate() const
{
    return mJoinDate;
}

void Person::setJoinDate(qint64 newJoinDate)
{
    CHECK_MODIFIED_THEN_SET(mJoinDate, newJoinDate, KItemJoinDate);
}

void Person::setJoinDate(const QString &newJoinDate, const QString &format)
{
    setJoinDate(DatetimeUtils::dateFromString(newJoinDate, format));
}

const QString &Person::familyContact() const
{
    return mFamilyContact;
}

void Person::setFamilyContact(const QString &newFamilyContact)
{
    CHECK_MODIFIED_THEN_SET(mFamilyContact, newFamilyContact, KItemFamilyContact);
}

qint64 Person::hollyDate() const
{
    return mHollyDate;
}

void Person::setHollyDate(qint64 newHollyDate)
{
    CHECK_MODIFIED_THEN_SET(mHollyDate, newHollyDate, KItemHollyDate);
}

void Person::setHollyDate(const QString &newHollyDate, const QString &format)
{
    setHollyDate(DatetimeUtils::dateFromString(newHollyDate, format));
}

const QString &Person::hollyPlace() const
{
    return mHollyPlace;
}

void Person::setHollyPlace(const QString &newHollyPlace)
{
    CHECK_MODIFIED_THEN_SET(mHollyPlace, newHollyPlace, KItemHollyPlace);
}

const QString &Person::christenPlace() const
{
    return mChristenPlace;
}

void Person::setChristenPlace(const QString &newChristenPlace)
{
    CHECK_MODIFIED_THEN_SET(mChristenPlace, newChristenPlace, KItemChristenPlace);
}

const QString &Person::familyHistory() const
{
    return mFamilyHistory;
}

void Person::setFamilyHistory(const QString &newFamilyHistory)
{
    CHECK_MODIFIED_THEN_SET(mFamilyHistory, newFamilyHistory, KItemFamilyHistory);
}

const QString &Person::momAddr() const
{
    return mMomAddr;
}

void Person::setMomAddr(const QString &newMomAddr)
{
    CHECK_MODIFIED_THEN_SET(mMomAddr, newMomAddr, KItemMomAddr);
}

qint64 Person::momBirthday() const
{
    return mMomBirthday;
}

void Person::setMomBirthday(qint64 newMomBirthday)
{
    CHECK_MODIFIED_THEN_SET(mMomBirthday, newMomBirthday, KItemMomBirthday);
}

void Person::setMomBirthday(const QString &newMomBirthday, const QString &format)
{
    setMomBirthday(DatetimeUtils::dateFromString(newMomBirthday, format));
}

const QString &Person::momName() const
{
    return mMomName;
}

void Person::setMomName(const QString &newMomName)
{
    CHECK_MODIFIED_THEN_SET(mMomName, newMomName, KItemMom);
}

const QString &Person::dadAddr() const
{
    return mDadAddr;
}

void Person::setDadAddr(const QString &newDadAddr)
{
    CHECK_MODIFIED_THEN_SET(mDadAddr, newDadAddr, KItemDadAddr);
}

qint64 Person::dadBirthday() const
{
    return mDadBirthday;
}

void Person::setDadBirthday(qint64 newDadBirthday)
{
    CHECK_MODIFIED_THEN_SET(mDadBirthday, newDadBirthday, KItemDadBirthday);
}

void Person::setDadBirthday(const QString &newDadBirthday, const QString &format)
{
    setDadBirthday(DatetimeUtils::dateFromString(newDadBirthday, format));
}

const QString &Person::dadName() const
{
    return mDadName;
}

void Person::setDadName(const QString &newDadName)
{
    CHECK_MODIFIED_THEN_SET(mDadName, newDadName, KItemDad);
}

const QStringList &Person::tel() const
{
    return mTel;
}

QString Person::telString()
{
    return mTel.join(SPLIT_EMAIL_TEL);
}

void Person::setTel(const QStringList &newTel)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mTel, newTel, KItemTel);
}

void Person::setTel(const QString &newTel)
{
    setTel(newTel.split(SPLIT_EMAIL_TEL));
}

const QStringList &Person::email() const
{
    return mEmail;
}

QString Person::emailString()
{
    return mEmail.join(SPLIT_EMAIL_TEL);
}

void Person::setEmail(const QStringList &newEmail)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mEmail, newEmail, KItemEmail);
}

void Person::setEmail(const QString &newEmail)
{
    setEmail(newEmail.split(SPLIT_EMAIL_TEL));
}

const QString &Person::churchAddr() const
{
    return mChurchAddr;
}

void Person::setChurchAddr(const QString &newChurchAddr)
{
    CHECK_MODIFIED_THEN_SET(mChurchAddr, newChurchAddr, KItemChurchAddress);
}

const QString &Person::addr() const
{
    return mAddr;
}

void Person::setAddr(const QString &newAddr)
{
    CHECK_MODIFIED_THEN_SET(mAddr, newAddr, KItemAddress);
}

const QString &Person::provinceName() const
{
    return mProvinceName;
}

void Person::setProvinceName(const QString &newProvinceName)
{
    CHECK_MODIFIED_THEN_SET(mProvinceName, newProvinceName, KItemProvince);
}

const QString &Person::provinceUid() const
{
    return mProvinceUid;
}

void Person::setProvinceUid(const QString &newProvinceUid)
{
    CHECK_MODIFIED_THEN_SET(mProvinceUid, newProvinceUid, KItemProvince);
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
    CHECK_MODIFIED_THEN_SET(mCountryUid, newCountryUid, KItemCountry);
}

const QString &Person::courseUid() const
{
    return mCourseUid;
}

void Person::setCourseUid(const QString &newCourseUid)
{
    CHECK_MODIFIED_THEN_SET(mCourseUid, newCourseUid, KItemCourse);
}

const QString &Person::courseName() const
{
    return mCourse;
}

void Person::setCourseName(const QString &newCourse)
{
    CHECK_MODIFIED_THEN_SET(mCourse, newCourse, KItemCourse);
}

const QStringList &Person::specialistUidList() const
{
    return mSpecialistUidList;
}

void Person::setSpecialistUidList(const QStringList &newSpecialistUidList)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mSpecialistUidList, newSpecialistUidList, KItemSpecialist);
}

void Person::setSpecialistUidList(const QString &newSpecialistUidList)
{
    setSpecialistUidList(newSpecialistUidList.split(NAME_SPLIT));
}

void Person::setSpecialistNames(const QString &newSpecialists, bool parseUid)
{
    tracein;
    QStringList list;
    foreach (QString name, newSpecialists.split(NAME_SPLIT)) {
        list.append(name.trimmed());
    }
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mSpecialistNameList, list, KItemSpecialist);

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
    traceout;

}

void Person::addSpecialistName(const QString &newSpecialist)
{
    if (!mSpecialistNameList.contains(newSpecialist)) {
        logd("Add specialist name '%s'", STR2CHA(newSpecialist));
        mSpecialistNameList.push_back(newSpecialist);
        markItemAsModified(KItemSpecialist);
    }
}

void Person::clearSpecialistUid()
{
    if (mSpecialistUidList.size() > 0) {
        mSpecialistUidList.clear();
        markItemAsModified(KItemSpecialist);
    }
}

void Person::addSpecialistUid(const QString &uid)
{
    if (!mSpecialistUidList.contains(uid)) {
        logd("Add specialist uid '%s'", STR2CHA(uid));
        mSpecialistUidList.push_back(uid);
        markItemAsModified(KItemSpecialist);
    }
}

QList<DbModel *> Person::getSpecialistList()
{
    tracein;
    QList<DbModel*> list;
    DbPersonModelHandler* hdl = dynamic_cast<DbPersonModelHandler*>(getDbModelHandler());
    if (hdl && !uid().isEmpty()) {
        list = hdl->getSpecialistList(uid());
    } else {
        loge("no DbPersonModelHandler or uid to get");
    }
    traceout;
    return list;
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
    mEduName = newEduName;
}

const QString &Person::eduUid() const
{
    return mEduUid;
}

void Person::setEduUid(const QString &newEduUid)
{
    CHECK_MODIFIED_THEN_SET(mEduUid, newEduUid, KItemEdu);
}

const QString &Person::idCardIssuePlace() const
{
    return mIdCardIssuePlace;
}

void Person::setIdCardIssuePlace(const QString &newIdCardIssuePlace)
{
    CHECK_MODIFIED_THEN_SET(mIdCardIssuePlace, newIdCardIssuePlace, KItemIDcardIssuer);
}

qint64 Person::idCardIssueDate() const
{
    return mIdCardIssueDate;
}

void Person::setIdCardIssueDate(qint64 newIdCardIssueDate)
{
    CHECK_MODIFIED_THEN_SET(mIdCardIssueDate, newIdCardIssueDate, KItemIDcardIssueDate);
}

void Person::setIdCardIssueDate(const QString &newIdCardIssueDate)
{
    setIdCardIssueDate(DatetimeUtils::dateFromString(newIdCardIssueDate));
}

const QString &Person::idCard() const
{
    return mIdCard;
}

void Person::setIdCard(const QString &newIdCard)
{
    CHECK_MODIFIED_THEN_SET(mIdCard, newIdCard, KItemIDcard);
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
    CHECK_MODIFIED_THEN_SET(mEthnicUid, newEthnicUid, KItemEthnic);
}

const QString &Person::nationalityUid() const
{
    return mNationalityUid;
}

void Person::setNationalityUid(const QString &newNationalityUid)
{
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
    CHECK_MODIFIED_THEN_SET(mHollyName, newHollyName, KItemHolly);

    logd("parseUid %d", parseUid);
    if (parseUid){
        QHash<QString, QString> uidList;
        ErrCode ret = ErrNone;
        ret = SAINTCTL->getSaintUidListFromName(mHollyName, &uidList);
        if (ret == ErrNone) {
            mSaintUidNameMap.clear();
            mSaintUidList.clear();
            mSaintUidNameMap.insert(uidList);
            mSaintUidList.append(uidList.keys());
        } else {
            loge("Failed to get uid from holly name %d", ret);
        }
    } else {
        logd("Skip parsing holly name to uid");
    }
}

const QStringList &Person::saintUidList() const
{
    return mSaintUidList;
}

QString Person::saintUidListInString()
{
    tracein;
    QString val;
    if (mSaintUidList.count() > 0){
        val = mSaintUidList.join(HOLLYNAME_SPLIT);
    }
    logd("UidList in string %s", val.toStdString().c_str());
    return val;
}

void Person::setSaintUidList(const QStringList &newSaintUidList)
{
    CHECK_MODIFIED_THEN_SET_QLIST_STRING(mSaintUidList, newSaintUidList, KItemHolly);
}

void Person::setSaintUidList(const QString &newSaintUidList)
{
    tracein;
    setSaintUidList(newSaintUidList.split(HOLLYNAME_SPLIT));
    traceout;
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
    CHECK_MODIFIED_THEN_SET(mTrainJoinDate, newTrainJoinDate, KItemTrainDate);
}

void Person::setTrainJoinDate(const QString &newTrainJoinDate, const QString &format)
{
    setTrainJoinDate(DatetimeUtils::dateFromString(newTrainJoinDate, format));
}

QString Person::imgPath() const
{
    return mImage.fullImgPath();
}

QString Person::thumbImgPath() const
{
    return mImage.thumbImgPath();
}

ErrCode Person::setImgPath(const QString &newImgPath)
{
    tracein;
    ErrCode err = ErrNone;
    if (!newImgPath.isEmpty() && mImage.fullImgPath() != newImgPath) {
        err = mImage.loadImage(newImgPath, IMG_TAG_PEOPLE);
        if (err == ErrNone) {
            mImgId = mImage.uid();
            markItemAsModified(KItemImg);
        }
    } else {
        logw("nothing to set, newImgPath is empty? or not change? '%s'", STR2CHA(newImgPath));
    }
    traceret(err);
    return err;
}
