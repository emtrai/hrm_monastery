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
#include "specialistctl.h"
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
#include "view/dialog/dlghtmlviewer.h"
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

UIPersonListView::UIPersonListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;

    connect(this, SIGNAL(changeCommunityDone(ErrCode,QList<DbModel*>,Community*,bool,bool)),
            this, SLOT(onChangeCommunityDone(ErrCode,QList<DbModel*>,Community*,bool,bool)));
}

UIPersonListView::~UIPersonListView()
{
    tracein;

    PERSONCTL->delListener(this);
    MainWindow::removeMainWindownImportListener(this);

    traceout;
}

void UIPersonListView::setupUI()
{
    tracein;
    UITableView::setupUI();
    PERSONCTL->addListener(this);
    MainWindow::addMainWindownImportListener(this);
    traceout;
}

ErrCode UIPersonListView::onLoad()
{
    tracein;
    QList<DbModel*> items;
    ErrCode err = MainWindow::showProcessingDialog(tr("Đang truy vấn dữ liệu"), nullptr,
       [this, &items](ErrCode* err, void* data, DlgWait* dlg) {
            if (this->mFilterList.count() > 0) {
                // TODO: multi filter items???? should limite???? what the hell is it
                foreach (FilterItem* item, this->mFilterList) {
                    logd("filter item %s", STR2CHA(item->item()));
                    if (item->item() == KItemCommunity) {
                        // TODO: how about keyword? assume value only?????
                        QList<DbModel*> list;
                        *err = PERSONCTL->getListPersonInCommunity(item->value().toString(), list);
                        if (*err == ErrNone) {
                            if (list.count() > 0) {
                                items.append(list);
                            }
                        } else {
                            loge("Get list person in community uid '%s' failed, err=%d",
                                 STR2CHA(item->value().toString()), err);
                        }
                    } else {
                        ASSERT(0, "not this filter item!!!");
                    }
                }
            } else {
                logd("get all person");
                items = PERSONCTL->getAllItems();
            }
            return nullptr;//nothing to return
       },
        [this, &items](ErrCode err, void* data, void* result, DlgWait* dlg) {
            logd("Save result %d", err);
            RELEASE_LIST_DBMODEL(this->mItemList);
            // TODO: loop to much, redundant, do something better?
            foreach (DbModel* item, items) {
                this->mItemList.append(item);
            }

            clearFilter();
            return err;
        });
//    if (mFilterList.count() > 0) {
//        // TODO: multi filter items???? should limite???? what the hell is it
//        foreach (FilterItem* item, mFilterList) {
//            logd("filter item %s", STR2CHA(item->item()));
//            if (item->item() == KItemCommunity) {
//                // TODO: how about keyword? assume value only?????
//                QList<DbModel*> list;
//                ErrCode err = PERSONCTL->getListPersonInCommunity(item->value().toString(), list);
//                if (err == ErrNone) {
//                    if (list.count() > 0) {
//                        items.append(list);
//                    }
//                } else {
//                    loge("Get list person in community uid '%s' failed, err=%d",
//                         STR2CHA(item->value().toString()), err);
//                }
//            } else {
//                ASSERT(0, "not this filter item!!!");
//            }
//        }
//    } else {
//        logd("get all person");
//        items = PERSONCTL->getAllItems();
//    }

//    RELEASE_LIST_DBMODEL(mItemList);
//    // TODO: loop to much, redundant, do something better?
//    foreach (DbModel* item, items) {
//        mItemList.append(item);
//    }

//    clearFilter();
    traceout;
    return err;
}

ErrCode UIPersonListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
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
            tblItem->addValue(per->getFullName());
            tblItem->addValue(per->communityName());
            tblItem->addValue(DatetimeUtils::date2String(per->birthday()));
            tblItem->addValue(per->birthPlace());
            tblItem->addValue(DatetimeUtils::date2String(per->feastDay(), DEFAULT_FORMAT_MD)); // seem feastday convert repeate many time, make it common????

            tblItem->addValue(DatetimeUtils::date2String(per->joinDate()));
            tblItem->addValue(DatetimeUtils::date2String(per->vowsDate()));
            tblItem->addValue(DatetimeUtils::date2String(per->eternalVowsDate()));
            tblItem->addValue(per->courseName());
            tblItem->addValue(DatetimeUtils::date2String(per->deadDate()));
            tblItem->addValue(per->tel().join(";"));
            tblItem->addValue(per->email().join(";"));
            tblItem->addValue(per->specialistNameList().join(","));
            tblItem->addValue(per->currentWorkName());

            // TODO: show thumb image??? if support, need to verify performance
        } else {
            loge("No item found, or not expected model '%s'", item?STR2CHA(item->modelName()):"");
            err = ErrInvalidModel;
        }
    }
    traceret(err);
    return err;
}

void UIPersonListView::initHeader()
{
    tracein;
    mHeader.append(tr("Mã"));
    mHeader.append(tr("Tên Thánh"));
    mHeader.append(tr("Họ tên"));
    mHeader.append(tr("Cộng đoàn"));
    mHeader.append(tr("Năm sinh"));
    mHeader.append(tr("Nơi sinh"));
    mHeader.append(tr("Ngày bổn mạng"));
    mHeader.append(tr("Ngày Nhập Dòng"));
    mHeader.append(tr("Ngày Tiên Khấn"));
    mHeader.append(tr("Ngày Vĩnh Khấn"));
    mHeader.append(tr("Lớp khấn"));
    mHeader.append(tr("Ngày an nghỉ"));
    mHeader.append(tr("Chuyên môn"));
    mHeader.append(tr("Công tác xã hội"));
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
    traceout;
}

ModelController *UIPersonListView::getController()
{
    return PERSONCTL;
}



void UIPersonListView::importRequested(const QString &fpath)
{
    tracein;
    QList<DbModel*> list;
    logd("Import from file %s", fpath.toStdString().c_str());
    ErrCode ret = INSTANCE(PersonCtl)->importFromFile(KModelHdlPerson, ImportType::IMPORT_CSV_LIST, fpath, &list);
    logd("Import result %d", ret);
    logd("No of import item %d", list.count());
    DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
    dlg->setup(list);
    dlg->exec();
    delete dlg;
}

QList<UITableMenuAction *> UIPersonListView::getMenuCommonActions(const QMenu *menu)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuCommonActions(menu);
    actionList.append(UITableMenuAction::build(tr("Nhập từ tập tin"), this)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionImport(m, a);
                                                   }));
    actionList.append(UITableMenuAction::build(tr("Xuất danh sách nữ tu"), this)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionExportListPerson(m, a);
                                                   }));
    traceout;
    return actionList;
}

QList<UITableMenuAction *> UIPersonListView::getMenuSingleSelectedItemActions(const QMenu *menu, UITableCellWidgetItem *item)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuSingleSelectedItemActions(menu, item);
    actionList.append(UITableMenuAction::build(tr("Đổi cộng đoàn"), this, item)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionChangeCommunity(m, a);
                                                   }));
    actionList.append(UITableMenuAction::build(tr("Xem thông tin Cộng đoàn"), this, item)
                                                    ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                        return this->onMenuActionViewCommunity(m, a);
                                                    }));
    actionList.append(UITableMenuAction::build(tr("Xem thông tin sự kiện"), this, item)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionViewPersonEvent(m, a);
                                                   }));
    actionList.append(UITableMenuAction::build(tr("Thêm thông tin sự kiện"), this, item)
                                                    ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                        return this->onMenuActionAddPersonEvent(m, a);
                                                    }));
    actionList.append(UITableMenuAction::build(tr("Xuất thông tin Nữ tu"), this, item)
                                                    ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                        return this->exportPersonInfo(m, a);
                                                    }));
    traceout;
    return actionList;
}

QList<UITableMenuAction *> UIPersonListView::getMenuMultiSelectedItemActions(const QMenu *menu, const QList<UITableItem *> &items)
{
    tracein;
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiSelectedItemActions(menu, items);
    actionList.append(UITableMenuAction::buildMultiItem(tr("Đổi cộng đoàn"), this, &items)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionChangeCommunity(m, a);
                                                   }));
    actionList.append(UITableMenuAction::buildMultiItem(tr("Thêm thông tin sự kiện"), this, &items)
                                                 ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                     return this->onMenuActionAddPersonEvent(m, a);
                                                 }));
    traceout;
    return actionList;
}

ErrCode UIPersonListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    MainWindow::showAddEditPerson();
    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionImport(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    ret = MainWindow::showImportDlg(IMPORT_TARGET_PERSON);
    traceret(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionExportListPerson(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode err = ErrNone;
    QList<DbModel*> list = PERSONCTL->getAllItemsFromDb();
    if (!list.empty()) {
        logd("Export %d items", list.size());
        err = MainWindow::exportListItems(&list, KModelNamePerson, PERSONCTL, tr("Xuất danh sách nữ tu"), EXPORT_XLSX);
    } else {
        logw("nothing to export");
    }
    RELEASE_LIST_DBMODEL(list);
    traceret(err);
    return err;
}

ErrCode UIPersonListView::onMenuActionChangeCommunity(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    DlgSearchCommunity* dlg = DlgSearchCommunity::build(this, false);
    dlg->setIsMultiSelection(false);
    dlg->enableGetAllSupport();

    if (dlg->exec() == QDialog::Accepted){
        const DbModel* item = dlg->selectedItem();
        Community* comm = nullptr;
        if (item != nullptr) {
            comm = (Community*)item->clone();
            if (!comm) {
                loge("cannot clone community '%s', no memory?", MODELSTR2CHA(comm));
                ret = ErrNoMemory;
            }
        } else {
            ret = ErrInvalidData;
            loge("no selected community");
        }
        if (ret == ErrNone) {
            logd("community to be changed '%s'", MODELSTR2CHA(comm));
            QString msg = QString(tr("Đổi sang cộng đoàn '%1'?")).arg(comm->name());
            QList<DbModel*> items;
            int cnt = act->itemListData(items);
            logd("No. selected person %d", cnt);
            if (cnt > 0) {

                DlgConfirmUpdatePeopleComm dlgConfirmChange;
                dlgConfirmChange.setCommunity(comm);
                dlgConfirmChange.setPersonList(items);
                bool addHistory = dlgConfirmChange.addCommunityHistory();
                bool addEvent = dlgConfirmChange.addPersonEvent();

                if (dlgConfirmChange.exec() == QDialog::Accepted){
                    mSuspendReloadOnDbUpdate = true;
                    logd("change community '%s'", MODELSTR2CHA(comm));
                    ret = MainWindow::showProcessingDialog(
                        QString(tr("Đổi sang cộng đoàn '%1'")).arg(comm->name()),
                            nullptr, // prepare
                            [items, comm](ErrCode* err, void* data, DlgWait* dlg) { // run
                                ErrCode tmpErr = ErrNone;
                                UNUSED(data);
                                UNUSED(dlg);
                                foreach (DbModel* item, items){
                                    logd("Add person '%s' to community", MODELSTR2CHA(item));
                                    tmpErr = COMMUNITYCTL->addPerson2Community(comm, (Person*)item);
                                    if (tmpErr != ErrNone) {
                                        loge("Failed to add person '%s' to community '%s', err %d",
                                             MODELSTR2CHA(item), MODELSTR2CHA(comm), tmpErr);
                                        break;
                                    }
                                }
                                if (err) *err = tmpErr;
                                return nullptr;//nothing to return
                            },
                            [this, items, comm, addHistory, addEvent](ErrCode err, void* data, void* result, DlgWait* dlg) { // finish
                                logd("Save result %d", err);
                                UNUSED(data);
                                UNUSED(dlg);
                                UNUSED(result);
                                // TODO: risk of access freed resource (use-after-free),
                                // as perList/community is managed by confirm dialog
                                emit this->changeCommunityDone(err,
                                                               CLONE_LIST_DBMODEL(items),
                                                               CLONE_MODEL(comm, Community),
                                                               addHistory, addEvent);
                                return err;
                            }, nullptr);
                }
            } else {
                DialogUtils::showMsgBox(tr("Danh sách nữ tu trống)"));
                ret = ErrNoData;
            }
            RELEASE_LIST_DBMODEL(items);
            FREE_PTR(comm);
        } else {
            logi("No community selected");
        }
    } else {
        logd("not accept???");
    }
    // TODO: update history of person/community??
    // TODO: update event of person???
    delete dlg;
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
        logd("no selected item %ld", act->itemList().size());
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
                                       Utils::UidFromName(per->getFullName(),
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

ErrCode UIPersonListView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    int idx = item->idx();
    logd("idx=%d",idx);
    if (idx < mItemList.length()){
        Person* per = (Person*)mItemList.value(idx);
        if (per) {
//            QTemporaryFile file;
            QString fpath;
//            if (file.open()) {
//                fpath = file.fileName();
//                logd("export html file path='%s'", STR2CHA(fpath));
//            }
            INSTANCE(PersonCtl)->exportToFile(per, ExportType::EXPORT_HTML, &fpath);
            if (QFile::exists(fpath)){
                dlgHtmlViewer* viewer = new dlgHtmlViewer();
                viewer->setHtmlPath(fpath);
                viewer->setSubject(per->getFullName());
                viewer->exec();
            } else {
                loge("html file '%s' not found", STR2CHA(fpath));
            }
        } else {
            loge("not person data to view");
        }
    } else {
        loge("Invalid idx");
        // TODO: popup message???
    }
    traceout;
    return ErrNone; // TODO: check to return value
}

ErrCode UIPersonListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DbModel* model = item->itemData();
//    logd("idx=%d",idx);
    if (model){
        Person* per = (Person*)model;
        DlgPerson* dlg = DlgPerson::buildDlg(this, per, (per == nullptr));
        dlg->exec();
        delete dlg;

    } else {
        loge("Invalid item data");
        err = ErrInvalidArg;
        // TODO: popup message???
    }
    traceret(err);
    return err;
}

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
    logd("mItemList cnt %d", mItemList.count());
    traceout;
    return mItemList.count();
}

DbModel *UIPersonListView::onCreateDbModelObj(const QString& modelName)
{
    return Person::build();
}


void UIPersonListView::cleanUpItem()
{
    tracein;
//    logd("Clean up %d items", mPersonList.count());
//    foreach (Person* per, mPersonList){
//        logd("Delet per %s", per->getFullName().toStdString().c_str());
//        delete per;
//    }
    //    mPersonList.clear();
}

//void UIPersonListView::onConfirmChangeCommunity(const QList<DbModel *> &perList,
//                                                const Community *comm)
//{
//    tracein;
//    ErrCode err = ErrNone;
//    if (!comm || perList.size() == 0) {
//        err = ErrInvalidArg;
//        loge("invalid comm or perList is zero");
//    }
//    if (err == ErrNone) {
//        mSuspendReloadOnDbUpdate = true;
//        logd("change community '%s'", MODELSTR2CHA(comm));
//        err = MainWindow::showProcessingDialog(
//            QString(tr("Đổi sang cộng đoàn '%1'")).arg(comm->name()),
//            nullptr, // prepare
//            [perList, comm](ErrCode* err, void* data, DlgWait* dlg) { // run
//                ErrCode tmpErr = ErrNone;
//                UNUSED(data);
//                UNUSED(dlg);
//                foreach (DbModel* item, perList){
//                    logd("Add person '%s' to community", MODELSTR2CHA(item));
//                    tmpErr = COMMUNITYCTL->addPerson2Community(comm, (Person*)item);
//                    if (tmpErr != ErrNone) {
//                        loge("Failed to add person '%s' to community '%s', err %d",
//                             MODELSTR2CHA(item), MODELSTR2CHA(comm), tmpErr);
//                        break;
//                    }
//                }
//                if (err) *err = tmpErr;
//                return nullptr;//nothing to return
//            },
//            [this, perList, comm](ErrCode err, void* data, void* result, DlgWait* dlg) { // finish
//                logd("Save result %d", err);
//                UNUSED(data);
//                UNUSED(dlg);
//                UNUSED(result);
//                // TODO: risk of access freed resource (use-after-free),
//                // as perList/community is managed by confirm dialog
//                emit this->changeCommunityDone(err,
//                                               CLONE_LIST(perList, DbModel),
//                                               CLONE_MODEL(comm, Community));
//                return err;
//            }, nullptr);
//    }
//    traceout;
//}

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
    DlgPersonCommunity* dlg = nullptr;
    if (!comm || perList.empty()) {
        err = ErrInvalidArg;
        loge("cannot add comm history, invalid arg");
    }
    if (err == ErrNone && (dlg = DlgPersonCommunity::build(this)) == nullptr) {
        loge("Cannot create/build dialog, no memory?");
        err = ErrNoMemory;
    }
    if (err == ErrNone) {
        err = dlg->setPersonList<DbModel>(perList);
    }
    if (err == ErrNone) {
        logd("set comm '%s'", MODELSTR2CHA(comm));
        dlg->setCommunity(comm);
    }

    if (err == ErrNone) {
        dlg->exec();
    }

    if (err != ErrNone) {
        logd("Add community history failed, err=%d", err);
        DialogUtils::showErrorBox(err, tr("Lỗi thêm dữ liệu Cộng đoàn"));
    }
    if (dlg) delete dlg;
    traceret(err);
    return err;
}


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

    if (err == ErrNone) {
        this->mSuspendReloadOnDbUpdate = false;
        reload();
    } else {
        loge("Change community failed, err=%d", err);
        DialogUtils::showErrorBox(err, QString(tr("Đổi cộng đoàn %1 thất bại")).arg(comm->name()));
    }
    if (err == ErrNone && addEvent) {
        logd("Add person event");
        err = onConfirmAddPersonEventChangeCommunity(listPer, comm);
    }
    if (err == ErrNone && addHistory) {
        err = onConfirmAddHistoryCommunity(listPer, comm);
    }
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

ErrCode UIPersonListView::updatePersonEvent(const QList<DbModel *>& perList, const PersonEvent *event)
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
                            tmp->buildNameId(per->nameId(), event->nameId());
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

