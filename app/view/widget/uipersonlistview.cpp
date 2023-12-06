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
 * Filename: uipersonlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uipersonlistview.h"
#include "personctl.h"
#include "utils.h"
#include "datetimeutils.h"
#include "logger.h"
#include <QList>
#include <QTemporaryFile>
#include <QMessageBox>
#include "dbmodel.h"
#include "person.h"
#include "utils.h"
#include "view/dialog/dlgsearchcommunity.h"
#include "view/dialog/dlgaddpersonevent.h"
#include "view/widget/uipersoneventlistview.h"
#include "view/widget/uicommunitiesofpersonlistview.h"
#include <QFile>
#include "filter.h"
#include "dlgperson.h"
#include "mainwindow.h"
#include "community.h"
#include "communityctl.h"
#include "mainwindow.h"
#include "personevent.h"
#include "event.h"
#include "eventdefs.h"
#include "stringdefs.h"
#include "dlgpersoncomm.h"
#include "dlgconfirmupdatepeoplecomm.h"
#include "dialogutils.h"
#include "viewutils.h"
#include "errreporterctl.h"
#include "communityperson.h"
#include "dbctl.h"

UIPersonListView::UIPersonListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    mHasImportMenu = true;
    mHasExportMenu = true;
    traceout;
}

UIPersonListView::~UIPersonListView()
{
    tracein;

    MainWindow::removeMainWindownImportListener(this);

    traceout;
}

void UIPersonListView::setupUI()
{
    tracein;
    UICommonListView::setupUI();
    // register to receive even of import.
    // we need to suspend reloading list when importing, to avoid list to be updated
    // continuously
    MainWindow::addMainWindownImportListener(this);
    traceout;
}

ErrCode UIPersonListView::onLoad()
{
    tracein;
    QList<DbModel*> items;
    ErrCode err = ErrNone;
    if (mSuspendReloadOnDbUpdate) {
        loge("busy, pending to reload");
        return err;
    }
    // async loading, to show progress dialog
    err = MainWindow::showProcessingDialog(STR_QUERYING,
        nullptr, // prepare callback
        // run callback
       [this, &items](ErrCode* err, void* data, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(dlg);
            ErrCode ret = ErrNone;
            if (this->mFilterList.count() > 0) {
                // TODO: multi filter items???? should limite???? what the hell is it
                foreach (FilterItem* item, this->mFilterList) {
                    if (!item) {
                        logw("invalid filter item to load");
                        continue;
                    }
                    logd("filter item %s", STR2CHA(item->item()));
                    if (item->item() == KItemCommunity) {
                        QList<DbModel*> list;
                        // TODO: how about keyword? assume value only?????
                        ret = PERSONCTL->getListPersonInCommunity(item->value().toString(), list);
                        if (ret == ErrNone) {
                            logd("found %lld items", list.size());
                            if (list.size() > 0) {
                                items.append(list);
                            }
                        } else {
                            loge("Get list person in community uid '%s' failed, ret=%d",
                                 STR2CHA(item->value().toString()), ret);
                            break;
                        }
                    } else {
                        loge("Not support to filter item '%s'", STR2CHA(item->item()));
                        ret = ErrNotSupport;
                        break;
                    }
                }
            } else {
                logd("get all person");
                items = PERSONCTL->getAllItems(true);
                logd("found %lld item", items.size());
            }

            if (err) *err = ret;
            return nullptr;//nothing to return
       },
        // finish callback
        [this, &items](ErrCode err, void* data, void* result, DlgWait* dlg) {
            UNUSED(data);
            UNUSED(result);
            UNUSED(dlg);
            logd("Save result %d", err);
            RELEASE_LIST_DBMODEL(this->mModelList);
            if (err == ErrNone) {
                if (items.size() > 0) {
                    this->mModelList.append(items);
                } else {
                    logd("no data to load");
                }
            } else {
                RELEASE_LIST_DBMODEL(items);
                loge("load data failed, err=%d",err);
                REPORTERRCTL->reportErr(STR_QUERY_ERROR, err, true);
            }

            clearFilter();
            return err;
        });

    traceout;
    return err;
}


void UIPersonListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_HOLLYNAME);
    mHeader.append(STR_FULLNAME);
    mHeader.append(STR_STATUS);
    mHeader.append(STR_COURSE);
    mHeader.append(STR_COMMUNITY);
    mHeader.append(STR_BIRTHDAY);
    mHeader.append(STR_BIRTHPLACE);
    mHeader.append(STR_NGAY_BON_MANG);
    mHeader.append(STR_LOP_KHAN);
    mHeader.append(STR_NGAY_NHAP_DONG);
    mHeader.append(STR_NGAY_TIEN_KHAN);
    mHeader.append(STR_NGAY_VINH_KHAN);
    mHeader.append(STR_SPECIALIST);
    mHeader.append(STR_CONG_TAC_XA_HOI);
    mHeader.append(STR_TEL);
    mHeader.append(STR_EMAIL);
    mHeader.append(STR_NGAY_AN_NGHI);
    traceout;
}

ErrCode UIPersonListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    UNUSED(idx);
    ErrCode err = ErrNone;
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }
    if (err == ErrNone) {
        logd("updateItem '%s'", item?STR2CHA(item->modelName()):"");
        if (item && item->modelName() == KModelNamePerson) {
            Person* per = (Person*) item;
            tblItem->addValue(per->nameId());
            tblItem->addValue(per->hollyName());
            tblItem->addValue(per->fullName());
            tblItem->addValue(per->personStatusName());
            tblItem->addValue(per->courseName());
            tblItem->addValue(per->communityName());
            tblItem->addValue(DatetimeUtils::date2String(per->birthday()));
            tblItem->addValue(per->birthPlace());
            tblItem->addValue(DatetimeUtils::date2String(per->feastDay(), DEFAULT_FORMAT_MD)); // seem feastday convert repeate many time, make it common????

            tblItem->addValue(per->courseName());
            tblItem->addValue(DatetimeUtils::date2String(per->joinDate()));
            tblItem->addValue(DatetimeUtils::date2String(per->vowsDate()));
            tblItem->addValue(DatetimeUtils::date2String(per->eternalVowsDate()));
            tblItem->addValue(per->specialistNameList().join(","));
            tblItem->addValue(per->currentWorkName());
            tblItem->addValue(per->tel().join(";"));
            tblItem->addValue(per->email().join(";"));
            tblItem->addValue(DatetimeUtils::date2String(per->deadDate()));

            // TODO: show thumb image??? if support, need to verify performance
        } else {
            loge("No item found, or not expected model '%s'", item?STR2CHA(item->modelName()):"");
            err = ErrInvalidModel;
        }
    }
    traceret(err);
    return err;
}

ErrCode UIPersonListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    ErrCode ret = ErrNone;
    MainWindow::showAddEditPerson();
    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    const DbModel* model = nullptr;
    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        model = item->itemData();
        if (!model || !IS_MODEL_NAME(model, KModelNamePerson)) {
            loge("Invalid model '%s'", MODELSTR2CHA(model));
            err = ErrInvalidModel;
        }
    }
    if (err == ErrNone) {
        const Person* per = (const Person*)model;
        DlgPerson* dlg = DlgPerson::buildDlg(this, per, (per == nullptr));
        if (dlg) {
            dlg->exec();
            delete dlg;
        } else {
            err = ErrNoMemory;
            loge("failed to create dialog, not memory?");
        }
    }
    traceret(err);
    return err;
}

ModelController *UIPersonListView::getController()
{
    return PERSONCTL;
}


QList<UITableMenuAction *> UIPersonListView::getMenuSingleSelectedItemActions(
    const QMenu *menu, UITableCellWidgetItem *item)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuSingleSelectedItemActions(menu, item);
    actionList.append(BUILD_MENU_ACTION_IMPL(STR_CHANGE_COMMUNITY, item, onMenuActionChangeCommunity));

    actionList.append(BUILD_MENU_SEPARATE);

    actionList.append(BUILD_MENU_ACTION_IMPL(STR_ADD_EVENT, item, onMenuActionAddPersonEvent));

    actionList.append(BUILD_MENU_SEPARATE);

    actionList.append(BUILD_MENU_ACTION_IMPL(STR_VIEW_COMMUNITY, item, onMenuActionViewCommunity));

    actionList.append(BUILD_MENU_ACTION_IMPL(STR_VIEW_EVENT, item, onMenuActionViewPersonEvent));

    actionList.append(BUILD_MENU_SEPARATE);

    actionList.append(BUILD_MENU_ACTION_IMPL(STR_EXPORT_PERSON_INFO, item, exportPersonInfo));

    traceout;
    return actionList;
}

QList<UITableMenuAction *> UIPersonListView::getMenuMultiSelectedItemActions(
    const QMenu *menu, const QList<UITableItem *> &items)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiSelectedItemActions(menu, items);
    actionList.append(UITableMenuAction::buildMultiItem(STR_CHANGE_COMMUNITY, this, &items)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionChangeCommunity(m, a);
                                                   }));
    actionList.append(BUILD_MENU_SEPARATE);
    actionList.append(UITableMenuAction::buildMultiItem(STR_ADD_EVENT, this, &items)
                                                 ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                     return this->onMenuActionAddPersonEvent(m, a);
                                                 }));
    traceout;
    return actionList;
}

ErrCode UIPersonListView::onMenuActionChangeCommunity(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    UNUSED(menu);
    ErrCode ret = ErrNone;
    DlgSearchCommunity* dlg = nullptr;
    Community* comm = nullptr;
    QList<DbModel*> peopleList;
    bool addCommPer = false;
    bool addPersonEvent = false;
    int dlgret = 0;

    QList<PersonEvent*> perEventList;
    QList<CommunityPerson *> inActiveCommPerList;
    QList<CommunityPerson *> activeCommPerList;
    if (!act) {
        ret = ErrInvalidArg;
        loge("invalid arg");
    }
    if (ret == ErrNone) {
        dlg = DlgSearchCommunity::build(this, false);
        if (dlg) {
            dlg->setIsMultiSelection(false);
            dlg->enableGetAllSupport();
        } else {
            ret = ErrNoMemory;
            loge("failed to build search comm dlg, no memory?");
        }
    }
    if (ret == ErrNone && ((dlgret = dlg->exec()) != QDialog::Accepted)){
        ret = ErrCancelled;
        loge("user cancel? dlgret=%d", dlgret);
    }
    if (ret == ErrNone) {
        const DbModel* item = dlg->selectedItem();
        if (item != nullptr && IS_MODEL_NAME(item, KModelNameCommunity)) {
            comm = (Community*)item->clone();
            if (!comm) {
                loge("cannot clone community '%s', no memory?", MODELSTR2CHA(item));
                ret = ErrNoMemory;
            }
        } else {
            ret = ErrInvalidData;
            loge("invalid selected community '%s'", MODELSTR2CHA(item));
        }
    }
    if (ret == ErrNone) {
        int cnt = act->itemListData(peopleList);
        if (!(cnt > 0 && peopleList.size() > 0)) {
            loge("not item selected");
            ret = ErrNoData;
        }
    }
    // validate community with current selected person
    if (ret == ErrNone) {
        foreach (DbModel* item, peopleList) {
            if (!item || !IS_MODEL_NAME(item, KModelNamePerson)) {
                loge("invalid model data '%s'", MODELSTR2CHA(item));
                ret = ErrInvalidModel;
                break;
            }
            Person* per = (Person*) item;
            // check if new community already current community of person
            if (per->communityUid() == comm->uid()) {
                ret = ErrExisted;
                loge("community '%s' already current community of person '%s'",
                     MODELSTR2CHA(comm), MODELSTR2CHA(per));
                break;
            }
        }
    }
    if (ret == ErrNone) {
        DlgConfirmUpdatePeopleComm dlgConfirmChange;
        dlgConfirmChange.setCommunity(comm);
        dlgConfirmChange.setPersonList(peopleList);
        addCommPer = dlgConfirmChange.addCommunityHistory();
        addPersonEvent = dlgConfirmChange.addPersonEvent();

        if (dlgConfirmChange.exec() != QDialog::Accepted){
            ret = ErrAbort;
            loge("abort to change community");
        }
    }

    // change persone event
    if (ret == ErrNone && addPersonEvent) {
        ret = buildPersonCommunityChangeEventList(peopleList, comm, perEventList);
    }

    //inactive comm person
    if (ret == ErrNone && addCommPer) {
        ret = buildInActiveCommPersonList(peopleList, inActiveCommPerList);
        if (ret == ErrNoData) {
            logw("no inactive comm person list, just ignore");
            ret = ErrNone;
        }
    }

    //active comm person
    if (ret == ErrNone && addCommPer) {
        ret = buildActiveCommPersonList(peopleList, comm, activeCommPerList);
    }

    if (ret == ErrNone) {
        logd("perEventList sz %lld", perEventList.size());
        logd("inActiveCommPerList sz %lld", inActiveCommPerList.size());
        logd("activeCommPerList sz %lld", activeCommPerList.size());

#ifdef DEBUG_LOG
        foreach(PersonEvent* item, perEventList) {
            logd("perevent '%s'", MODELSTR2CHA(item));
        }
        foreach(CommunityPerson* item, inActiveCommPerList) {
            logd("commper '%s'", MODELSTR2CHA(item));
        }
        foreach(CommunityPerson* item, activeCommPerList) {
            logd("active commper '%s'", MODELSTR2CHA(item));
        }
#endif
    }
    if (ret == ErrNone) {
        mSuspendReloadOnDbUpdate = true;
        logd("start change to community '%s'", MODELSTR2CHA(comm));
        ret = MainWindow::showProcessingDialog(
            QString(STR_CHANGE_TO_COMMUNITY).arg(comm->name()),
            nullptr, // prepare
            [peopleList, comm,
             addCommPer, addPersonEvent,
             perEventList, inActiveCommPerList, activeCommPerList]
            (ErrCode* err, void* data, DlgWait* dlg) { // run
                ErrCode tmpErr = ErrNone;
                UNUSED(data);
                UNUSED(dlg);
                logd("change to community '%s'", MODELSTR2CHA(comm));

                // add to community person active)
                if (tmpErr == ErrNone && addCommPer) {
                    logd("add activeCommPerList , no item %lld", activeCommPerList.size());
                    foreach (DbModel* item, activeCommPerList){
                        if (item) {
                            logi("Save active commper '%s'", MODELSTR2CHA(item));
                            tmpErr = item->save();
                            if (tmpErr != ErrNone) {
                                loge("Failed to save active commper item '%s'",
                                     MODELSTR2CHA(item));
                                break;
                            }
                        }
                    }
                }

                // add to community person (inactive)
                if (tmpErr == ErrNone && addCommPer) {
                    logd("update inActiveCommPerList , no item %lld", inActiveCommPerList.size());
                    foreach (DbModel* item, inActiveCommPerList){
                        if (item) {
                            logi("Save inactive commper '%s'", MODELSTR2CHA(item));
                            tmpErr = item->update();
                            if (tmpErr != ErrNone) {
                                loge("Failed to update inactive commper item '%s'",
                                     MODELSTR2CHA(item));
                                break;
                            }
                        }
                    }
                }

                foreach (DbModel* item, peopleList){
                    logi("Add person '%s' to community '%s'",
                         MODELSTR2CHA(item), MODELSTR2CHA(comm));
                    // not auto update person comm mapping
                    // when call addPerson2Community(), because we need to allow user
                    // to set end time and start time when change community.
                    // so here, we just update people table, with current community info
                    tmpErr = COMMUNITYCTL->addPerson2Community(comm, (Person*)item, false);
                    if (tmpErr != ErrNone) {
                        loge("Failed to add person '%s' to community '%s', err %d",
                             MODELSTR2CHA(item), MODELSTR2CHA(comm), tmpErr);
                        break;
                    }
                }

                // update persone event
                if (tmpErr == ErrNone && addPersonEvent) {
                    logd("add person event list, no item %lld", perEventList.size());
                    foreach (DbModel* item, perEventList){
                        if (item) {
                            logi("Save event '%s'", MODELSTR2CHA(item));
                            tmpErr = item->save();
                            if (tmpErr != ErrNone) {
                                loge("Failed to save person event item '%s'",
                                     MODELSTR2CHA(item));
                                break;
                            }
                        }
                    }
                }
                if (err) *err = tmpErr;
                return nullptr;//nothing to return
            },
            nullptr, nullptr);
    }
    if (ret == ErrNone) {
        DialogUtils::showMsgBox(QString(STR_CHANGE_COMMUNITY_FOR_PERSON)
                                    .arg(comm->name())
                                    .arg(peopleList.size())
                                );
    } else {
        loge("Change community failed, err=%d", ret);
        DialogUtils::showErrorBox(ret, STR_FAILED_CHANGE_COMMUNITY);
    }

    logd("reload list");
    // in some case, error is returned, but some data has been updated
    // so need to reload here.
    this->mSuspendReloadOnDbUpdate = false;
    requestReload();

    logd("free up");
    RELEASE_LIST_DBMODEL(peopleList);
    RELEASE_LIST(perEventList, PersonEvent);
    RELEASE_LIST(inActiveCommPerList, CommunityPerson);
    RELEASE_LIST(activeCommPerList, CommunityPerson);
    FREE_PTR(comm);
    FREE_PTR(dlg);
    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionViewPersonEvent(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    UNUSED(menu);
    ErrCode ret = ErrNone;
    const Person* person = nullptr;
    QString errMsg = STR_ERROR_VIEW_PERSON_EVENT;
    UIPersonEventListView* view = nullptr;
    if (!act) {
        ret = ErrInvalidArg;
        loge("invalid act arg");
        errMsg = STR_SELECT_PERSON;
    }
    if (ret == ErrNone) {
        person = dynamic_cast<const Person*>(act->getData());
        if (!person || !IS_MODEL_NAME(person, KModelNamePerson)) {
            ret = ErrNoData;
            loge("invalid model data '%s'", MODELSTR2CHA(person));
            errMsg = STR_SELECT_PERSON;
        }
    }
    if (ret == ErrNone) {
        view =
            (UIPersonEventListView*)MAINWIN->getView(ViewType::VIEW_PERSON_EVENT);
        if (!view) {
            ret = ErrInvalidView;
            loge("invalid view VIEW_PERSON_EVENT, no mem or invalid type?");
        }
    }
    if (ret == ErrNone) {
        ret = view->setPerson(person);
    }
    if (ret == ErrNone) {
        logd("view event of person %s", MODELSTR2CHA(person));
        MAINWIN->switchView(view);
    }

    if (ret != ErrNone) {
        loge("Error when view person event, err=%d", ret);
        DialogUtils::showErrorBox(ret, errMsg);
    }

    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionViewCommunity(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    const Person* person = nullptr;
    QString errMsg = STR_ERROR_VIEW_COMMUNITY;
    UICommunitiesOfPersonListView* view = nullptr;
    UNUSED(menu);
    if (!act) {
        ret = ErrInvalidArg;
        loge("invalid act arg");
        errMsg = STR_SELECT_PERSON;
    }
    if (ret == ErrNone) {
        person = dynamic_cast<const Person*>(act->getData());
        if (!person || !IS_MODEL_NAME(person, KModelNamePerson)) {
            ret = ErrNoData;
            loge("invalid model data '%s'", MODELSTR2CHA(person));
            errMsg = STR_SELECT_PERSON;
        }
    }
    if (ret == ErrNone) {
        view = (UICommunitiesOfPersonListView*)
            MAINWIN->getView(ViewType::VIEW_COMMUNITIES_OF_PERSON_LIST);
        if (!view) {
            ret = ErrInvalidView;
            loge("invalid view VIEW_COMMUNITIES_OF_PERSON_LIST, no mem or invalid type?");
        }
    }
    if (ret == ErrNone) {
        ret = view->setPerson(person);
    }
    if (ret == ErrNone) {
        logd("person to view community %s", MODELSTR2CHA(person));
        MAINWIN->switchView(view);
    }

    if (ret != ErrNone) {
        loge("Error when view person community, err=%d", ret);
        DialogUtils::showErrorBox(ret, errMsg);
    }

    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionAddPersonEvent(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> perList;
    UNUSED(menu);
    if (!act) {
        err = ErrInvalidArg;
        loge("invalid menu action");
    }
    if (err == ErrNone && !(act->itemList().size() > 0)) {
        err = ErrNoData;
        loge("no item data to handle");
    }
    if (err == ErrNone) {
        logd("is multi select %d", act->getIsMultiSelectedItem());
        logd("no selected item %lld", act->itemList().size());
        int cnt = act->itemListData(perList);
        logd("no person %d", cnt);
        if (cnt <= 0) {
            err = ErrNoData;
            loge("no person data to handle");
        }
    }
    if (err == ErrNone) {
        logd("update person event from blank");
        err = updatePersonEvent(perList);
    }
    RELEASE_LIST_DBMODEL(perList);

    traceret(err);
    return err;
}

ErrCode UIPersonListView::exportPersonInfo(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    UITableCellWidgetItem* item = nullptr;
    Person* per = nullptr;
    QString fpath;
    QString errMsg = STR_ERROR;
    UNUSED(menu);
    if (!act) {
        loge("invalid argument");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        item = (UITableCellWidgetItem*)act->tblItem();
        if (!item) {
            loge("No item data");
            err = ErrNoData;
        }
    }
    if (err == ErrNone) {
        int idx = item->idx();
        logd("idx=%d",idx);
        if (idx < mModelList.length()){
            per = (Person*)mModelList.value(idx);
            if (!per) {
                loge("not person data");
                err = ErrNoData;
            }
        } else {
            loge("Invalid idx=%d", idx);
            err = ErrInvalidData;
        }
    }

    if (err == ErrNone) {
        logi("export person infor to html file first");
        err = INSTANCE(PersonCtl)->exportToFile(per, ExportType::EXPORT_HTML, &fpath);
        errMsg = STR_XUAT_DU_LIEU_LOI;
    }
    if (err == ErrNone) {
        if (QFile::exists(fpath)){
            logi("convert html file '%s' to pdf", STR2CHA(fpath));
            err = Utils::saveHtmlToPdf(fpath,
                                       Utils::UidFromName(per->fullName(),
                                                    UidNameConvertType::NO_VN_MARK_UPPER),
                                        this);
                if (err == ErrNone) {
                    logi("Saved html path '%s' to pdf succeed", STR2CHA(fpath));
                } else {
                    loge("Saved html path '%s' to pdf failed, err=%d", STR2CHA(fpath), err);
                    errMsg = STR_XUAT_DU_LIEU_LOI;
                }
        } else {
            err = ErrNotFound;
            loge("export to '%s', file not found", STR2CHA(fpath));
        }
    }

    if (!fpath.isEmpty() && QFile::exists(fpath)) {
        logi("Remove temp file '%s'", STR2CHA(fpath));
        QFile::remove(fpath);
    }
    if (err != ErrNone) {
        DialogUtils::showErrorBox(err, errMsg);
    }

    traceout;
    return err;
}

QString UIPersonListView::getTitle()
{
    return STR_LIST_PERSON;
}

void UIPersonListView::initFilterFields()
{
    tracein;
    appendFilterField(FILTER_FIELD_FULL_NAME, STR_FULLNAME);
    appendFilterField(FILTER_FIELD_COMMUNITY, STR_COMMUNITY_NAME);
    appendFilterField(FILTER_FIELD_HOLLY_NAME, STR_HOLLYNAME);
    appendFilterField(FILTER_FIELD_EDUCATION, STR_EDUCATION);
    appendFilterField(FILTER_FIELD_SPECIALIST, STR_SPECIALIST);
    appendFilterField(FILTER_FIELD_WORK, STR_WORK);
    appendFilterField(FILTER_FIELD_COURSE, STR_COURSE);
    traceout;
}

DbModel *UIPersonListView::onCreateDbModelObj(const QString& modelName)
{
    UNUSED(modelName);
    return Person::build();
}


void UIPersonListView::cleanUpItem()
{
    traced;
}

ErrCode UIPersonListView::buildPersonCommunityChangeEventList(const QList<DbModel *> &perList,
                                                               const Community *comm,
                                                               QList<PersonEvent *> &perEventList)
{
    tracein;
    ErrCode err = ErrNone;
    PersonEvent* perEvent = nullptr;
    DlgAddPersonEvent* dlg = nullptr;
    if (!comm || perList.empty()) {
        err = ErrInvalidArg;
        loge("cannot add per event, invalid arg");
    }

    if (err == ErrNone && (perEvent = (PersonEvent*)PersonEvent::build()) == nullptr) {
        loge("no memory for perEvent");
        err = ErrNoMemory;
    }

    if (err == ErrNone) {
        err = perEvent->setEventNameId(CHANGE_COMMUNITY_NAMEID);
    }

    if (err == ErrNone) {
        perEvent->setDate(QDateTime::currentDateTime().toString("dd.MM.yyyy"));
        perEvent->setName(QString(STR_CHANGE_PERSON_TO_COMMUNITY).arg(comm->name()));
    } else {
        loge("Evnt name id '%s' not found, err=%d", CHANGE_COMMUNITY_NAMEID, err);
    }

    if (err == ErrNone) {
        dlg = DlgAddPersonEvent::build(this, false, KModelNamePersonEvent, perEvent);
        if (!dlg) {
            err = ErrNoMemory;
            loge("no memory");
        }
    }

    if (err == ErrNone) {
        dlg->setEvenInfoOnly(&perList);
        if (dlg->exec() == QDialog::Accepted) {
            // TODO something
            PersonEvent* perEvent = (PersonEvent*)dlg->getModel();
            Event* event = (Event*)dlg->getSelectedEvent();
            if (perEvent && event) {
                foreach (DbModel* per, perList) {
                    if (IS_MODEL_NAME(per, KModelNamePerson)) {
                        PersonEvent* tmp = (PersonEvent* )(((DbModel*)perEvent)->clone());
                        if (tmp) {
                            tmp->setPersonUid(per->uid());
                            tmp->buildNameIdFromOthersNameId(per->nameId(), event->nameId());
                            perEventList.append(tmp);
                        } else {
                            err = ErrNoMemory;
                            loge("failed to clone person event, no memory");
                            break;
                        }
                    } else {
                        logw("something strange, per is invalid '%s'", MODELSTR2CHA(per));
                        //TODO: break for error???
                        break;
                    }
                }

            } else {
                loge("event %d or perEvent %d is null",
                     (perEvent==nullptr), (event==nullptr));
            }
        } else {
            err = ErrAbort;
            loge("Abort to add person event");
        }
    }

    if (err == ErrNone) {
        err = dlg->acceptResult();
    }
    if (err == ErrNone && perEventList.size() == 0) {
        err = ErrNoData;
        loge("not event data to add");
    }

    if (err == ErrNone) {
        foreach(PersonEvent* event, perEventList) {
            if (IS_MODEL_NAME(event, KModelNamePersonEvent)) {
                logd("validate perEvent '%s'", MODELSTR2CHA(event));
                err = event->validateAllFields(true);
                if (err == ErrNone) {
                    continue;
                } else if (err == ErrExisted) {
                    loge("Event '%s' already existed", MODELSTR2CHA(event));
                    break;
                } else {
                    loge("Event '%s' validateAllFields failed, err = %d",
                         MODELSTR2CHA(event), err);
                    break;
                }
            } else {
                logw("something went wrong, event was invalid '%s'", MODELSTR2CHA(event));
                err = ErrInvalidData;
                break;
            }
        }
    }

    if (err != ErrNone) {
        RELEASE_LIST(perEventList, PersonEvent);
    }
    FREE_PTR(dlg);
    FREE_PTR(perEvent);
    traceret(err);
    return err;
}

ErrCode UIPersonListView::buildInActiveCommPersonList(const QList<DbModel *> &perList,
                                                      QList<CommunityPerson *> &commPerListOut)
{
    tracein;
    ErrCode err = ErrNone;
    QList<CommunityPerson*> listCommPer;

    if (err == ErrNone && perList.size() == 0) {
        err = ErrNoData;
        loge("nothing to do, no data");
    }

    if (err == ErrNone) {
        foreach (DbModel* model, perList) {
            if (!IS_MODEL_NAME(model, KModelNamePerson)) {
                err = ErrInvalidArg;
                loge("invalid argument '%s'", MODELSTR2CHA(model));
                // stupid error, should not happend, as stupid thing will occur
                // just log here if this stupid happend
                break;
            }
            Person* per = (Person*)model;
            logd("per '%s', current commuid '%s'",
                 MODELSTR2CHA(per), STR2CHA(per->communityUid()));

            QList<DbModel*> listCommPerTmp;
            logd("get list of active comm per of person '%s'", MODELSTR2CHA(per));
            err = COMMUNITYCTL->getListActiveCommunityPersonOfPerson(per->uid(), listCommPerTmp);
            logd("list commPer sz %lld", listCommPerTmp.size());
            if (listCommPerTmp.size() == 0) {
                logw("not active list commper");
                // well, should not occur, but if it exist, just ignore
                continue;
            }
            // let's set current community person to inactive
            foreach (DbModel* item, listCommPerTmp) {
                if (!IS_MODEL_NAME(item, KModelNameCommPerson)) {
                    logw("commper '%s' is not CommPer", MODELSTR2CHA(item));
                    // what, again? what did you do? why this can occur????
                    break;
                }
                CommunityPerson* commper = (CommunityPerson*)item;
                commper->setMarkModified(true);
                logd("Set commper '%s' to INACTIVE", MODELSTR2CHA(commper));
                commper->setModelStatus(MODEL_STATUS_INACTIVE);
                listCommPer.append(commper);
            }
        }
    }

    if (err == ErrNone && listCommPer.size() == 0) {
        err = ErrNoData;
        loge("nothing to do, no list comm per data");
    }

    if (err == ErrNone) {
        DlgPersonCommunity* dlg = nullptr;

        if (err == ErrNone && (dlg = DlgPersonCommunity::build(this, false)) == nullptr) {
            loge("Cannot create/build dialog, no memory?");
            err = ErrNoMemory;
        }
        if (err == ErrNone) {
            dlg->setTitle(STR_PERSON_MOVE_OUT_COMMUNITY);
            dlg->setIsNew(false);
            dlg->setSkipStartDate(true);// as we focus on end date only
            dlg->setEndDate();
            err = dlg->setCommunityPersonList(listCommPer, true);
        }
        if (err == ErrNone) {
            err = dlg->setModelStatus(MODEL_STATUS_INACTIVE, false);
        }
        if (err == ErrNone && (dlg->exec() != QDialog::Accepted)) {
            err = ErrAbort;
            loge("Abort change");
        }

        if (err == ErrNone) {
            err = dlg->acceptResult();
        }

        if (err == ErrNone) {
            QList<CommunityPerson*> listCommPerTmp = dlg->communityPersonList();
            logd("listCommPerTmp sz %lld", listCommPerTmp.size());
            if (listCommPerTmp.size() > 0) {
                commPerListOut = CLONE_LIST(listCommPerTmp, CommunityPerson);
            } else {
                loge("no data");
                err = ErrNoData;
            }
        }

        FREE_PTR(dlg);
    }

    RELEASE_LIST(listCommPer, CommunityPerson);
    traceret(err);
    return err;
}

ErrCode UIPersonListView::buildActiveCommPersonList(const QList<DbModel *> &perList,
                                                    const Community *comm,
                                                    QList<CommunityPerson *> &commPerList)
{
    tracein;
    ErrCode err = ErrNone;
    DlgPersonCommunity* dlg = nullptr;
    if (!comm || perList.empty()) {
        err = ErrInvalidArg;
        loge("cannot add comm history, invalid arg");
    }
    if (err == ErrNone && (dlg = DlgPersonCommunity::build(this, false)) == nullptr) {
        loge("Cannot create/build dialog, no memory?");
        err = ErrNoMemory;
    }
    if (err == ErrNone) {
        dlg->setTitle(QString(STR_PERSON_MOVE_IN_COMMUNITY).arg(comm->name()));
        dlg->loadModelStatus(false);
    }
    if (err == ErrNone) {
        logd("set comm '%s'", MODELSTR2CHA(comm));
        dlg->setCommunity(comm);
        err = dlg->setPersonList<DbModel>(perList);
    }
    if (err == ErrNone) {
        dlg->setStartDate();
        err = dlg->setModelStatus(MODEL_STATUS_ACTIVE, false);
    }

    if (err == ErrNone && (dlg->exec() != QDialog::Accepted)) {
        err = ErrAbort;
        loge("Abort change");
    }
    if (err == ErrNone) {
        err = dlg->acceptResult();
    }
    if (err == ErrNone) {
        commPerList = CLONE_LIST(dlg->communityPersonList(), CommunityPerson);
    }

    FREE_PTR(dlg);
    traceret(err);
    return err;
}

QString UIPersonListView::getName()
{
    return "UIPersonListView";
}

void UIPersonListView::onImportStart(const QString &importName, const QString &fpath,
                                     ImportType type)
{
    tracein;
    UNUSED(importName);
    UNUSED(fpath);
    UNUSED(type);
    mSuspendReloadOnDbUpdate = true;
    logd("suspend reload on db update");
    traceout;
}

void UIPersonListView::onImportEnd(const QString &importName, ErrCode err,
                                   const QString &fpath, ImportType type)
{
    tracein;
    UNUSED(importName);
    UNUSED(err);
    UNUSED(fpath);
    UNUSED(type);
    mSuspendReloadOnDbUpdate = false;
    logd("resume reload on db update");
    requestReload();
    traceout;
}

void UIPersonListView::onMainWindownImportStart(ImportTarget target)
{
    UNUSED(target);
    mSuspendReloadOnDbUpdate = true;
}

void UIPersonListView::onMainWindownImportEnd(ImportTarget target, ErrCode err, void *importData)
{
    UNUSED(target);
    UNUSED(err);
    UNUSED(importData);
    mSuspendReloadOnDbUpdate = false;
    requestReload();
}

QString UIPersonListView::getMainModelName()
{
    return KModelNamePerson;
}

ErrCode UIPersonListView::updatePersonEvent(const QList<DbModel *>& perList,
                                            const PersonEvent *event)
{
    tracein;
    ErrCode err = ErrNone;
    DlgAddPersonEvent* dlg = nullptr;
    QList<PersonEvent*> perEventList;
    if (perList.empty()) {
        err = ErrInvalidArg;
        loge("invalid perList input");
    }
    if (err == ErrNone) {
        dlg = DlgAddPersonEvent::build(this, false, KModelNamePersonEvent, event);
        if (!dlg) {
            err = ErrNoMemory;
            loge("no memory");
        }
    }
    if (err == ErrNone) {
        dlg->setEvenInfoOnly(&perList);
        if (dlg->exec() == QDialog::Accepted) {
            // TODO something
            PersonEvent* perEvent = (PersonEvent*)dlg->getModel();
            Event* event = (Event*)dlg->getSelectedEvent();
            if (perEvent && event) {
                foreach (DbModel* per, perList) {
                    if (per) {
                        PersonEvent* tmp = (PersonEvent* )(((DbModel*)perEvent)->clone());
                        if (tmp) {
                            tmp->setPersonUid(per->uid());
                            tmp->buildNameIdFromOthersNameId(per->nameId(), event->nameId());
                            perEventList.append(tmp);
                        } else {
                            err = ErrNoMemory;
                            loge("failed to clone person event, no memory");
                            break;
                        }
                    } else {
                        logw("something strange, null value in list");
                        //TODO: break for error???
                    }
                }

            } else {
                loge("event %d or perEvent %d is null",
                     (perEvent==nullptr), (event==nullptr));
            }
        }
    }
    if (err == ErrNone && perEventList.size() == 0) {
        err = ErrNoData;
        loge("not event data to add");
    }

    if (err == ErrNone) {
        foreach(PersonEvent* perEvent, perEventList) {
            if (perEvent) {
                logd("validate perEvent '%s'", MODELSTR2CHA(perEvent));
                err = perEvent->validateAllFields(true);
                if (err == ErrNone) {
                    continue;
                } else if (err == ErrExisted) {
                    loge("Event '%s' already existed", MODELSTR2CHA(perEvent));
                    break;
                } else {
                    loge("Event '%s' validateAllFields failed, err = %d",
                         MODELSTR2CHA(perEvent), err);
                    break;
                }
            } else {
                logw("something went wrong, event was null");
                err = ErrInvalidData;
                break;
            }
        }
    }

    if (err == ErrNone) {
        foreach(PersonEvent* perEvent, perEventList) {
            if (perEvent) {
                logd("Save for perEvent '%s'", MODELSTR2CHA(perEvent));
                QString nameid = perEvent->nameId();
                QString availNameId;
                if (nameid.isEmpty()) {
                    err = ErrInvalidArg;
                    loge("Lack of nameId");
                    break;
                }
                err = DB->getAvailableNameId(KModelNamePersonEvent, nameid, availNameId);
                dbgd("availNameId '%s'", STR2CHA(availNameId));
                if (err == ErrNone && availNameId.isEmpty()) {
                    loge("not found suitable name id, init name id '%s'", STR2CHA(nameid));
                    err = ErrNotFound;
                    break;
                }
                if (err == ErrNone) {
                    perEvent->setNameId(availNameId);
                }

                err = perEvent->save();
                if (err != ErrNone) {
                    loge("Event '%s' save failed, err = %d",
                         MODELSTR2CHA(perEvent), err);
                    break;
                }
            } else {
                logw("something went wrong, event was null");
                err = ErrInvalidData;
                break;
            }
        }
    }

    RELEASE_LIST(perEventList, PersonEvent);
    FREE_PTR(dlg);

    traceret(err);
    return err;
}

ImportTarget UIPersonListView::getImportTarget()
{
    return IMPORT_TARGET_PERSON;
}

int UIPersonListView::getCategoryId(int currCategoryId, const QString &keywords,
                                    const QVariant *value)
{
    int nextCategory = currCategoryId;
    UNUSED(keywords);
    traceout;
    logd("currCategoryId %d", currCategoryId);
    if (value != nullptr && value->isValid()) {
        switch (currCategoryId) {
        case FILTER_FIELD_COMMUNITY:
            nextCategory = FILTER_FIELD_COMMUNITY_UID;
            break;
        case FILTER_FIELD_EDUCATION:
            nextCategory = FILTER_FIELD_EDUCATION_UID;
            break;
        case FILTER_FIELD_SPECIALIST:
            nextCategory = FILTER_FIELD_SPECIALIST_UID;
            break;
        case FILTER_FIELD_WORK:
            nextCategory = FILTER_FIELD_WORK_UID;
            break;
        case FILTER_FIELD_COURSE:
            nextCategory = FILTER_FIELD_COURSE_UID;
            break;
        }
    }
    logd("nextCategory %d", nextCategory);
    traceret(nextCategory);
    return nextCategory;
}

