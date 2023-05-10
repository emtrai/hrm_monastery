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
#include "logger.h"
#include <QList>
#include <QTemporaryFile>
#include "dbmodel.h"
#include "person.h"
#include "utils.h"
#include "view/dialog/dlgimportpersonlistresult.h"
#include "view/dialog/dlghtmlviewer.h"
#include "view/dialog/dlgsearchcommunity.h"
#include "view/widget/uipersoneventlistview.h"
#include <QFile>
#include "filter.h"
#include "dlgperson.h"
#include "mainwindow.h"
#include "community.h"
#include "communityctl.h"
#include "mainwindow.h"

UIPersonListView::UIPersonListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIPersonListView::~UIPersonListView()
{
    traced;

    PERSONCTL->delListener(this);
    MainWindow::removeMainWindownImportListener(this);
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

void UIPersonListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    logd("updateItem '%s'", item?STR2CHA(item->modelName()):"");
    if (item && item->modelName() == KModelNamePerson) {
        Person* per = (Person*) item;
        tblItem->addValue(per->nameId());
        tblItem->addValue(per->hollyName());
        tblItem->addValue(per->getFullName());
        tblItem->addValue(per->communityName());
        tblItem->addValue(Utils::date2String(per->birthday()));
        tblItem->addValue(per->birthPlace());
        tblItem->addValue(Utils::date2String(per->feastDay(), DEFAULT_FORMAT_MD)); // seem feastday convert repeate many time, make it common????

        tblItem->addValue(Utils::date2String(per->joinDate()));
        tblItem->addValue(Utils::date2String(per->vowsDate()));
        tblItem->addValue(Utils::date2String(per->eternalVowsDate()));
        tblItem->addValue(per->courseName());
        tblItem->addValue(Utils::date2String(per->deadDate()));
        tblItem->addValue(per->tel().join(";"));
        tblItem->addValue(per->email().join(";"));
        tblItem->addValue(per->specialistNameList().join(","));
        tblItem->addValue(per->currentWorkName());

        // TODO: show thumb image??? if support, need to verify performance
    } else {
        loge("No item found, or not expected model '%s'", item?STR2CHA(item->modelName()):"");
    }
    traceout;
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
                                                       return this->onChangeCommunity(m, a);
                                                   }));
    actionList.append(UITableMenuAction::build(tr("Xem thông tin sự kiện"), this, item)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionViewPersonEvent(m, a);
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
                                                       return this->onChangeCommunity(m, a);
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
    MainWindow::showImportDlg(IMPORT_TARGET_PERSON);
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
        err = MainWindow::exportListItems(&list, PERSONCTL, tr("Xuất danh sách nữ tu"), EXPORT_XLSX);
    } else {
        logw("nothing to export");
    }
    RELEASE_LIST_DBMODEL(list);
    traceret(err);
    return err;
}

ErrCode UIPersonListView::onChangeCommunity(QMenu *menu, UITableMenuAction *act)
{
    tracein;
    ErrCode ret = ErrNone;
    DlgSearchCommunity* dlg = DlgSearchCommunity::build(this, false);
    dlg->setIsMultiSelection(false);
    dlg->enableGetAllSupport();

    if (dlg->exec() == QDialog::Accepted){
        Community* comm = (Community*)dlg->selectedItem();
        if (comm != nullptr) {
            comm->dump();
            QList<DbModel*> items;
            int cnt = act->itemListData(items);
            logd("No. selected person %d", cnt);

            mSuspendReloadOnDbUpdate = true;
            ret = MainWindow::showProcessingDialog(tr("Đang đổi cộng đoàn"), nullptr,
               [this, comm, items](ErrCode* err, void* data, DlgWait* dlg) {
                    foreach (DbModel* item, items){
                        logd("Add person to community");
                        item->dump();
                        COMMUNITYCTL->addPerson2Community(comm, (Person*) item);
                    }
                    return nullptr;//nothing to return
               },
                [this](ErrCode err, void* data, void* result, DlgWait* dlg) {
                    logd("Save result %d", err);
                    this->mSuspendReloadOnDbUpdate = false;
                    reload();
                    return err;
                });
//            foreach (DbModel* item, items){
//                logd("Add person to community");
//                item->dump();
//                COMMUNITYCTL->addPerson2Community(comm, (Person*) item);
//            }
//            mSuspendReloadOnDbUpdate = false;
//            reload();
            // TODO: implement this
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
            (UIPersonEventListView*)UITableViewFactory::getView(ViewType::VIEW_PERSON_EVENT);

        logd("view event of person %s", STR2CHA(person->toString()));
        view->setPerson(person);
        MainWindow::getInstance()->switchView(view);
    } else {
        loge("no person event info");
        ret = ErrNoData;
        Utils::showErrorBox(tr("Vui lòng chọn cộng đoàn cần xem"));
    }

    traceret(ret);
    return ret;
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
                    Utils::showErrorBox(QString(tr("Xuất dữ liệu lỗi, mã lỗi %1")).arg(err));
                }
        } else {
            err = ErrNotFound;
            loge("export to '%s', file not found", STR2CHA(fpath));
        }
    }

    traceout;
    return err;
}

void UIPersonListView::onViewItem(UITableCellWidgetItem *item)
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
}

void UIPersonListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    DbModel* model = item->itemData();
//    logd("idx=%d",idx);
    if (model){
        Person* per = (Person*)model;
        DlgPerson* dlg = DlgPerson::buildDlg(this, per, (per == nullptr));
        dlg->exec();
        delete dlg;

    } else {
        loge("Invalid item data");
        // TODO: popup message???
    }
    traceout;
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
    ErrCode ret = PERSONCTL->filter(catetoryid, opFlags, keywords, KModelNamePerson, &mItemList);
    logd("filter ret %d", ret);
    logd("mItemList cnt %d", mItemList.count());
    traceout;
    return mItemList.count();
}

DbModel *UIPersonListView::onNewModel(const QString& modelName)
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
