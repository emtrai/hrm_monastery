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
#include "view/dialog/dlgimportpersonlistresult.h"
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

UIPersonListView::UIPersonListView(QWidget *parent):
    UICommonListView(parent)
{
    tracein;
    // emit event when change community done
    connect(this, SIGNAL(changeCommunityDone(ErrCode,QList<DbModel*>,Community*,bool,bool)),
            this, SLOT(onChangeCommunityDone(ErrCode,QList<DbModel*>,Community*,bool,bool)));

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
            RELEASE_LIST_DBMODEL(this->mItemList);
            if (err == ErrNone) {
                if (items.size() > 0) {
                    this->mItemList.append(items);
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
    DbModel* model = nullptr;
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
        Person* per = (Person*)model;
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
    // validate communit with current selected person
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

    if (ret == ErrNone && addPersonEvent) {
        ret = buildPersonCommunityChangeEventList(peopleList, comm, perEventList);
    }

    if (ret == ErrNone && addCommPer) {
        ret = buildInActiveCommPersonList(peopleList, inActiveCommPerList);
    }

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
                foreach (DbModel* item, peopleList){
                    logi("Add person '%s' to community '%s'",
                         MODELSTR2CHA(item), MODELSTR2CHA(comm));
                    tmpErr = COMMUNITYCTL->addPerson2Community(comm, (Person*)item);
                    if (tmpErr != ErrNone) {
                        loge("Failed to add person '%s' to community '%s', err %d",
                             MODELSTR2CHA(item), MODELSTR2CHA(comm), tmpErr);
                        break;
                    }
                }
                if (tmpErr == ErrNone && addPersonEvent) {
                    logd("add person event list, no item %lld", perEventList.size());
                    foreach (DbModel* item, perEventList){
                        logi("Save event '%s'", MODELSTR2CHA(item));
                        if (item) {
                            tmpErr = item->save();
                            if (tmpErr != ErrNone) {
                                loge("Failed to save pervent item '%s'",
                                     MODELSTR2CHA(item));
                                break;
                            }
                        }
                    }
                }
                if (tmpErr == ErrNone && addCommPer) {
                    logd("update inActiveCommPerList , no item %lld", inActiveCommPerList.size());
                    foreach (DbModel* item, inActiveCommPerList){
                        logi("Save inactive commper '%s'", MODELSTR2CHA(item));
                        if (item) {
                            tmpErr = item->update();
                            if (tmpErr != ErrNone) {
                                loge("Failed to update inactive commper item '%s'",
                                     MODELSTR2CHA(item));
                                break;
                            }
                        }
                    }
                }
                if (tmpErr == ErrNone) {
                    logd("add activeCommPerList , no item %lld", activeCommPerList.size());
                    foreach (DbModel* item, activeCommPerList){
                        logi("Save active commper '%s'", MODELSTR2CHA(item));
                        if (item) {
                            tmpErr = item->save();
                            if (tmpErr != ErrNone) {
                                loge("Failed to save active commper item '%s'",
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
        MAINWIN->showMessageBox(QString(tr("Đổi sang Cộng đoàn %1 cho %2 Nữ tu"))
                                    .arg(comm->name())
                                    .arg(peopleList.size())
                                );
    } else {
        loge("Change community failed, err=%d", ret);
        MAINWIN->showErrorDlgSignal(ret, QString(tr("Đổi cộng đoàn thất bại")));
    }

    this->mSuspendReloadOnDbUpdate = false;
    requestReload();
    RELEASE_LIST_DBMODEL(peopleList);
    RELEASE_LIST(perEventList, PersonEvent);
    RELEASE_LIST(inActiveCommPerList, CommunityPerson);
    RELEASE_LIST(activeCommPerList, CommunityPerson);
    FREE_PTR(comm);
    FREE_PTR(dlg);
//    if (ret == ErrNone) {
//        logd("community to be changed '%s'", MODELSTR2CHA(comm));
//        QString msg = QString(STR_ASK_CHANGE_TO_COMMUNITY).arg(comm->name());
//        int cnt = act->itemListData(peopleList);
//        logd("No. selected person %d", cnt);
//        if (cnt > 0) {

//            DlgConfirmUpdatePeopleComm dlgConfirmChange;
//            dlgConfirmChange.setCommunity(comm);
//            dlgConfirmChange.setPersonList(peopleList);
//            bool addHistory = dlgConfirmChange.addCommunityHistory();
//            bool addEvent = dlgConfirmChange.addPersonEvent();

//            if (dlgConfirmChange.exec() == QDialog::Accepted){
//                mSuspendReloadOnDbUpdate = true;
//                logd("change community '%s'", MODELSTR2CHA(comm));
//                ret = MainWindow::showProcessingDialog(
//                    QString(STR_CHANGE_TO_COMMUNITY).arg(comm->name()),
//                        nullptr, // prepare
//                        [peopleList, comm](ErrCode* err, void* data, DlgWait* dlg) { // run
//                            ErrCode tmpErr = ErrNone;
//                            UNUSED(data);
//                            UNUSED(dlg);
//                            foreach (DbModel* item, peopleList){
//                                logi("Add person '%s' to community '%s'",
//                                     MODELSTR2CHA(item), MODELSTR2CHA(comm));
//                                tmpErr = COMMUNITYCTL->addPerson2Community(comm, (Person*)item);
//                                if (tmpErr != ErrNone) {
//                                    loge("Failed to add person '%s' to community '%s', err %d",
//                                         MODELSTR2CHA(item), MODELSTR2CHA(comm), tmpErr);
//                                    break;
//                                }
//                            }
//                            if (err) *err = tmpErr;
//                            return nullptr;//nothing to return
//                        },
//                        [this, peopleList, comm, addHistory, addEvent](ErrCode err, void* data, void* result, DlgWait* dlg) { // finish
//                            logd("Save result %d", err);
//                            UNUSED(data);
//                            UNUSED(dlg);
//                            UNUSED(result);
//                            emit this->changeCommunityDone(err,
//                                                           CLONE_LIST_DBMODEL(peopleList),
//                                                           CLONE_MODEL(comm, Community),
//                                                           addHistory, addEvent);
//                            return err;
//                        }, nullptr);
//            }
//        } else {
//            DialogUtils::showMsgBox(STR_LIST_PERSON_EMPTY);
//            ret = ErrNoData;
//        }
//    }
    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionViewPersonEvent(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    Person* person = dynamic_cast<Person*>(act->getData());
    if (person != nullptr) {
        UIPersonEventListView* view =
            (UIPersonEventListView*)MAINWIN->getView(ViewType::VIEW_PERSON_EVENT);

        logd("view event of person %s", STR2CHA(person->toString()));
        view->setPerson(person);
        MAINWIN->switchView(view);
    } else {
        loge("no person event info");
        ret = ErrNoData;
        DialogUtils::showErrorBox(tr("Vui lòng chọn cộng đoàn cần xem"));
    }

    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionViewCommunity(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    Person* per = dynamic_cast<Person*>(act->getData());
    if (per != nullptr) {
        UICommunitiesOfPersonListView* view = (UICommunitiesOfPersonListView*)
            MAINWIN->getView(ViewType::VIEW_COMMUNITIES_OF_PERSON_LIST);

        logd("person to view community %s", MODELSTR2CHA(per));
        view->setPerson(per);
        MAINWIN->switchView(view);
    } else {
        loge("no per info");
        ret = ErrNoData;
        DialogUtils::showErrorBox(tr("Vui lòng chọn Nữ tu cần xem"));
    }

    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionAddPersonEvent(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> perList;
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
        if (idx < mItemList.length()){
            per = (Person*)mItemList.value(idx);
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
//        QTemporaryFile file;
        QString fpath;
//        if (file.open()) {
//            fpath = file.fileName();
//            logd("export html file path='%s'", STR2CHA(fpath));
//        }
        INSTANCE(PersonCtl)->exportToFile(per, ExportType::EXPORT_HTML, &fpath);
        if (QFile::exists(fpath)){
            err = Utils::saveHtmlToPdf(fpath,
                                       Utils::UidFromName(per->fullName(),
                                                    UidNameConvertType::NO_VN_MARK_UPPER),
                                        this);
                if (err == ErrNone) {
                    logi("Saved html path '%s' to pdf succeed", STR2CHA(fpath));
                } else {
                    loge("Saved html path '%s' to pdf failed, err=%d", STR2CHA(fpath), err);
                    DialogUtils::showErrorBox(QString(tr("Xuất dữ liệu lỗi, mã lỗi %1")).arg(err));
                }
        } else {
            err = ErrNotFound;
            loge("export to '%s', file not found", STR2CHA(fpath));
        }
    }

    traceout;
    return err;
}

//ErrCode UIPersonListView::onViewItem(UITableCellWidgetItem *item)
//{
//    tracein;
//    int idx = item->idx();
//    logd("idx=%d",idx);
//    if (idx < mItemList.length()){
//        Person* per = (Person*)mItemList.value(idx);
//        if (per) {
////            QTemporaryFile file;
//            QString fpath;
////            if (file.open()) {
////                fpath = file.fileName();
////                logd("export html file path='%s'", STR2CHA(fpath));
////            }
//            INSTANCE(PersonCtl)->exportToFile(per, ExportType::EXPORT_HTML, &fpath);
//            if (QFile::exists(fpath)){
//                dlgHtmlViewer* viewer = new dlgHtmlViewer();
//                viewer->setHtmlPath(fpath);
//                viewer->setSubject(per->getFullName());
//                viewer->exec();
//            } else {
//                loge("html file '%s' not found", STR2CHA(fpath));
//            }
//        } else {
//            loge("not person data to view");
//        }
//    } else {
//        loge("Invalid idx");
//        // TODO: popup message???
//    }
//    traceout;
//    return ErrNone; // TODO: check to return value
//}

QString UIPersonListView::getTitle()
{
    return tr("Danh sách nữ tu");
}

void UIPersonListView::initFilterFields()
{
    tracein;
    appendFilterField(FILTER_FIELD_FULL_NAME, tr("Họ tên"));
    appendFilterField(FILTER_FIELD_COMMUNITY, tr("Cộng đoàn"));
    appendFilterField(FILTER_FIELD_HOLLY_NAME, tr("Tên Thánh"));
    appendFilterField(FILTER_FIELD_EDUCATION, tr("Học vấn"));
//    appendFilterField(FILTER_FIELD_SPECIALIST, tr("Chuyên môn"));
    appendFilterField(FILTER_FIELD_WORK, tr("Công việc"));
    appendFilterField(FILTER_FIELD_COURSE, tr("Khoá"));
    traceout;
}

int UIPersonListView::onFilter(int catetoryid,
                                const QString &catetory,
                                qint64 opFlags,
                                const QString &keywords,
                               const QVariant *value)
{
    tracein;
    RELEASE_LIST_DBMODEL(mItemList);
    ErrCode ret = PERSONCTL->filter(catetoryid, opFlags, keywords, KModelNamePerson, nullptr, &mItemList);
    logd("filter ret %d", ret);
    logd("mItemList cnt %lld", mItemList.count());
    traceout;
    return mItemList.count();
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
        perEvent->setName(QString(tr("Chuyển Nữ tu sang Cộng Đoàn %1")).arg(comm->name()));
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
            dlg->setTitle(tr("Thay đổi thông tin cộng đoàn - Nữ tu chuyển khỏi Cộng đoàn hiện tại"));
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
        dlg->setTitle(QString(tr("Thay đổi thông tin cộng đoàn - Nữ tu chuyển đến Cộng đoàn %1")).arg(comm->name()));
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

ErrCode UIPersonListView::onConfirmAddPersonEventChangeCommunity(const QList<DbModel *> &perList,
                                                              const Community *comm)
{
    tracein;
    ErrCode err = ErrNone;
    PersonEvent* perEvent = nullptr;
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
        perEvent->setName(QString(tr("Chuyển sang Cộng Đoàn %1")).arg(comm->name()));
    } else {
        loge("Evnt name id '%s' not found, err=%d", CHANGE_COMMUNITY_NAMEID, err);
    }
    if (err == ErrNone) {
        err = updatePersonEvent(perList, perEvent);
    }
    FREE_PTR(perEvent);
    traceret(err);
    return err;
}

ErrCode UIPersonListView::onConfirmAddHistoryCommunity(const QList<DbModel *> &perList, const Community *comm)
{
    tracein;
    ErrCode err = ErrNone;
    logd("new community to be set '%s'", MODELSTR2CHA(comm));

    if (!comm) {
        loge("invalid comm");
        err = ErrInvalidArg;
    }
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
            if (per->communityUid() == comm->uid()) {
                logw("same community '%s' of per '%s', continue",
                     STR2CHA(per->communityUid()), MODELSTR2CHA(per));
                // another stupid thing which should not occur
                // but it's harmless, so just skip it.
                continue;
            }
            logd("get list of active comm per of person '%s'", MODELSTR2CHA(per));
            QList<DbModel*> listCommPer;
            err = COMMUNITYCTL->getListActiveCommunityPersonOfPerson(per->uid(), listCommPer);
            logd("list commPer sz %lld", listCommPer.size());
            if (listCommPer.size() == 0) {
                logw("not active list commper");
                // well, should not occur, but if it exist, just ignore
                continue;
            }
            // let's set current community person to inactive
            foreach (DbModel* item, listCommPer) {
                if (!IS_MODEL_NAME(item, KModelNameCommPerson)) {
                    logw("commper '%s' is not CommPer", MODELSTR2CHA(item));
                    // what, again? what did you do? why this can occur????
                    break;
                }
                CommunityPerson* commper = (CommunityPerson*)item;
                logd("Set commper '%s' to INACTIVE", MODELSTR2CHA(commper));
                commper->setModelStatus(MODEL_STATUS_INACTIVE);
                err = commper->update();
                if (err == ErrNone) {
                    logi("Updated inactive status for model '%s'", MODELSTR2CHA(commper));
                } else {
                    // please, don't jump to here, some thing bad will occur
                    loge("update model status for commper '%s' failed, err %d",
                         MODELSTR2CHA(commper), err);
                    break;
                }
            }
            RELEASE_LIST_DBMODEL(listCommPer);
        }
    }

    if (err == ErrNone) {
        logd("add new comm per");

    }

    traceret(err);
    return err;
}
//ErrCode UIPersonListView::onConfirmAddHistoryCommunity(const QList<DbModel *> &perList, const Community *comm)
//{
//    tracein;
//    ErrCode err = ErrNone;
//    DlgPersonCommunity* dlg = nullptr;
//    if (!comm || perList.empty()) {
//        err = ErrInvalidArg;
//        loge("cannot add comm history, invalid arg");
//    }
//    if (err == ErrNone && (dlg = DlgPersonCommunity::build(this)) == nullptr) {
//        loge("Cannot create/build dialog, no memory?");
//        err = ErrNoMemory;
//    }
//    if (err == ErrNone) {
//        err = dlg->setPersonList<DbModel>(perList);
//    }
//    if (err == ErrNone) {
//        logd("set comm '%s'", MODELSTR2CHA(comm));
//        dlg->setCommunity(comm);
//    }

//    if (err == ErrNone) {
//        dlg->exec();
//    }

//    if (err != ErrNone) {
//        logd("Add community history failed, err=%d", err);
//        DialogUtils::showErrorBox(err, tr("Lỗi thêm dữ liệu Cộng đoàn"));
//    }
//    if (dlg) delete dlg;
//    traceret(err);
//    return err;
//}


void UIPersonListView::onChangeCommunityDone(ErrCode err,
                                             QList<DbModel *> listPer,
                                             Community* comm,
                                             bool addHistory, bool addEvent)
{
    tracein;
    logd("err = %d", err);
    if (err == ErrNone && (!comm || (listPer.size() == 0))) {
        err = ErrInvalidArg;
        loge("invalid arg");
    }

    // update event of person
    if (err == ErrNone && addEvent) {
        logd("Add person event");
        err = onConfirmAddPersonEventChangeCommunity(listPer, comm);
    }
    // update community & people list
    if (err == ErrNone && addHistory) {
        err = onConfirmAddHistoryCommunity(listPer, comm);
    }
    if (err != ErrNone) {
        loge("Change community failed, err=%d", err);
        DialogUtils::showErrorBox(err, QString(tr("Đổi cộng đoàn %1 thất bại")).arg(comm->name()));
    }

    this->mSuspendReloadOnDbUpdate = false;
    requestReload();
    FREE_PTR(comm);
    RELEASE_LIST_DBMODEL(listPer);
    traceout;
}

QString UIPersonListView::getName()
{
    return "UIPersonListView";
}

void UIPersonListView::onImportStart(const QString &importName, const QString &fpath, ImportType type)
{
    tracein;
    mSuspendReloadOnDbUpdate = true;
    logd("suspend reload on db update");
    traceout;
}

void UIPersonListView::onImportEnd(const QString &importName, ErrCode err, const QString &fpath, ImportType type)
{
    tracein;
    mSuspendReloadOnDbUpdate = false;
    logd("resume reload on db update");
    reload();
    traceout;
}

void UIPersonListView::onMainWindownImportStart(ImportTarget target)
{
    mSuspendReloadOnDbUpdate = true;
}

void UIPersonListView::onMainWindownImportEnd(ImportTarget target, ErrCode err, void *importData)
{
    mSuspendReloadOnDbUpdate = false;
    reload();
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
    //    verity/validate info
    //            save info
//    delete info
    RELEASE_LIST(perEventList, PersonEvent);
    if (dlg) delete dlg;

    traceret(err);
    return err;
}

