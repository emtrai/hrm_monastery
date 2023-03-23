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


}

UIPersonListView::~UIPersonListView()
{
    traced;
}

ErrCode UIPersonListView::onLoad()
{
    traced;
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

void UIPersonListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    Person* per = (Person*) item;
    tblItem->addValue(per->personCode());
    tblItem->addValue(per->hollyName());
    tblItem->addValue(per->getFullName());
    tblItem->addValue(per->communityName());
    tblItem->addValue(Utils::date2String(per->birthday()));
    tblItem->addValue(per->birthPlace());
    tblItem->addValue(Utils::date2String(per->feastDay(), DATE_FORMAT_MD)); // seem feastday convert repeate many time, make it common????

    tblItem->addValue(per->tel().join(";"));
    tblItem->addValue(per->email().join(";"));
    tblItem->addValue(per->idCard());
    tblItem->addValue(per->idCardIssuePlace());

}

void UIPersonListView::initHeader()
{
    traced;
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
    traced;
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
    traced;
    QList<UITableMenuAction*> actionList = UITableView::getMenuCommonActions(menu);
    actionList.append(UITableMenuAction::build(tr("Nhập từ tập tin"), this)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onMenuActionImport(m, a);
                                                   }));
    tracede;
    return actionList;
}

QList<UITableMenuAction *> UIPersonListView::getMenuItemActions(const QMenu *menu, UITableWidgetItem *item)
{
    traced;
    QList<UITableMenuAction*> actionList = UITableView::getMenuItemActions(menu, item);
    actionList.append(UITableMenuAction::build(tr("Đổi cộng đoàn"), this, item)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onChangeCommunity(m, a);
                                                   }));
    tracede;
    return actionList;
}

QList<UITableMenuAction *> UIPersonListView::getMenuMultiItemActions(const QMenu *menu, const QList<UITableItem *> &items)
{
    traced;
    QList<UITableMenuAction*> actionList = UITableView::getMenuMultiItemActions(menu, items);
    actionList.append(UITableMenuAction::buildMultiItem(tr("Đổi cộng đoàn"), this, &items)
                                                   ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                                       return this->onChangeCommunity(m, a);
                                                   }));
    tracede;
    return actionList;
}

ErrCode UIPersonListView::onMenuActionAdd(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    MainWindow::showAddEditPerson();
    tracedr(ret);
    return ret;
}

ErrCode UIPersonListView::onMenuActionImport(QMenu *menu, UITableMenuAction *act)
{
    traced;
    ErrCode ret = ErrNone;
    MainWindow::showImportDlg(IMPORT_TARGET_PERSON);
    tracedr(ret);
    return ret;
}

ErrCode UIPersonListView::onChangeCommunity(QMenu *menu, UITableMenuAction *act)
{
    traced;
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
    tracedr(ret);
    return ret;
}

void UIPersonListView::onViewItem(UITableWidgetItem *item)
{
    traced;
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
    tracede;
}

void UIPersonListView::onEditItem(UITableWidgetItem *item)
{
    traced;
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
    tracede;
}

void UIPersonListView::onDeleteItem(UITableWidgetItem *item)
{
    traced;
    DbModel* model = item->itemData();
    //    logd("idx=%d",idx);
    if (model){
        logd("DELETE item:");
        model->dump();
        model->remove(); // TODO: call markRemove
    } else {
        loge("Invalid item data");
        // TODO: popup message???
    }
    tracede;
}

QString UIPersonListView::getTitle()
{
    return tr("Danh sách nữ tu");
}

void UIPersonListView::initFilterFields()
{
    traced;
    appendFilterField(FILTER_FIELD_FULL_NAME, tr("Họ tên"));
    appendFilterField(FILTER_FIELD_COMMUNITY, tr("Cộng đoàn"));
    appendFilterField(FILTER_FIELD_HOLLY_NAME, tr("Tên Thánh"));
    appendFilterField(FILTER_FIELD_EDUCATION, tr("Học vấn"));
    appendFilterField(FILTER_FIELD_SPECIALIST, tr("Chuyên môn"));
    appendFilterField(FILTER_FIELD_WORK, tr("Công việc"));
    appendFilterField(FILTER_FIELD_COURSE, tr("Khoá"));
    tracede;
}

int UIPersonListView::onFilter(int catetoryid,
                                const QString &catetory,
                                qint64 opFlags,
                                const QString &keywords,
                               const QVariant *value)
{
    traced;
    RELEASE_LIST_DBMODEL(mItemList);
    ErrCode ret = PERSONCTL->filter(catetoryid, opFlags, keywords, KModelNamePerson, &mItemList);
    logd("filter ret %d", ret);
    logd("mItemList cnt %d", mItemList.count());
    tracede;
    return mItemList.count();
}

DbModel *UIPersonListView::onNewModel()
{
    return Person::build();
}


void UIPersonListView::cleanUpItem()
{
    traced;
//    logd("Clean up %d items", mPersonList.count());
//    foreach (Person* per, mPersonList){
//        logd("Delet per %s", per->getFullName().toStdString().c_str());
//        delete per;
//    }
//    mPersonList.clear();
}
