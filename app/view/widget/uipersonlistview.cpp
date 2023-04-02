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
#include "dbmodel.h"
#include "person.h"
#include "utils.h"
#include "view/dialog/dlgimportpersonlistresult.h"
#include "view/dialog/dlghtmlviewer.h"
#include "view/dialog/dlgsearchcommunity.h"
#include <QFile>
#include "filter.h"
#include "dlgperson.h"
#include "mainwindow.h"
#include "community.h"
#include "communityctl.h"

UIPersonListView::UIPersonListView(QWidget *parent):
    UICommonListView(parent)
{
    traced;
}

UIPersonListView::~UIPersonListView()
{
    traced;
}

void UIPersonListView::setupUI()
{
    tracein;
    UITableView::setupUI();
    PERSONCTL->addListener(this);
    traceout;
}

ErrCode UIPersonListView::onLoad()
{
    tracein;
    QList<DbModel*> items;
    if (mFilterList.count() > 0) {
        // TODO: multi filter items???? should limite???? what the hell is it
        foreach (FilterItem* item, mFilterList) {
            logd("filter item %s", STR2CHA(item->item()));
            if (item->item() == KItemCommunity) {
                // TODO: how about keyword? assume value only?????
                QList<DbModel*> list;
                ErrCode err = PERSONCTL->getListPersonInCommunity(item->value().toString(), list);
                if (err == ErrNone) {
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

    RELEASE_LIST_DBMODEL(mItemList);
    // TODO: loop to much, redundant, do something better?
    foreach (DbModel* item, items) {
        mItemList.append(item);
    }

    clearFilter();
    return ErrNone;
}

void UIPersonListView::updateItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    loge("updateItem '%s'", item?STR2CHA(item->modelName()):"");
    if (item && item->modelName() == KModelNamePerson) {
        Person* per = (Person*) item;
        tblItem->addValue(per->nameId());
        tblItem->addValue(per->hollyName());
        tblItem->addValue(per->getFullName());
        tblItem->addValue(per->communityName());
        tblItem->addValue(Utils::date2String(per->birthday()));
        tblItem->addValue(per->birthPlace());
        tblItem->addValue(Utils::date2String(per->feastDay(), DEFAULT_FORMAT_MD)); // seem feastday convert repeate many time, make it common????

        tblItem->addValue(per->tel().join(";"));
        tblItem->addValue(per->email().join(";"));
        tblItem->addValue(per->idCard());
        tblItem->addValue(per->idCardIssuePlace());
        tblItem->addValue(per->courseName());
        tblItem->addValue(per->specialistNameList().join(","));
        tblItem->addValue(per->currentWorkName());
        tblItem->addValue(Utils::date2String(per->joinDate()));
        tblItem->addValue(Utils::date2String(per->vowsDate()));
        tblItem->addValue(Utils::date2String(per->eternalVowsDate()));
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
    mHeader.append(tr("Điện thoại"));
    mHeader.append(tr("Email"));
    mHeader.append(tr("Căn cước công dân"));
    mHeader.append(tr("Nơi cấp CCCD"));
    mHeader.append(tr("Khoá"));
    mHeader.append(tr("Chuyên môn"));
    mHeader.append(tr("Công tác xã hội"));
    mHeader.append(tr("Ngày Nhập Tu"));
    mHeader.append(tr("Ngày Tiên Khấn"));
    mHeader.append(tr("Ngày Vĩnh Khấn"));
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
            foreach (DbModel* item, items){
                logd("Add person to community");
                item->dump();
                COMMUNITYCTL->addPerson2Community(comm, (Person*) item);
            }
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

void UIPersonListView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    int idx = item->idx();
    logd("idx=%d",idx);
    if (idx < mItemList.length()){
        Person* per = (Person*)mItemList.value(idx);
        QString fpath;
        INSTANCE(PersonCtl)->exportToFile(per, ExportType::EXPORT_HTML, &fpath);
        if (QFile::exists(fpath)){
            dlgHtmlViewer* viewer = new dlgHtmlViewer();
            viewer->setHtmlPath(fpath);
            viewer->setSubject("Person");
            viewer->exec();
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

DbModel *UIPersonListView::onNewModel()
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
    return "UICommunityListView";
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
