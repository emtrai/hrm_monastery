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
 * Filename: uicommonlistview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#include "uicommonlistview.h"
#include "logger.h"
#include "saintctl.h"
#include "communityctl.h"
#include "specialistctl.h"
#include "coursectl.h"
#include "workctl.h"
#include "eductl.h"
#include "filter.h"
#include "mainwindow.h"
#include "stringdefs.h"
#include "dialogutils.h"
#include "importtype.h"
#include "viewutils.h"

UICommonListView::UICommonListView(QWidget *parent):
    UITableView(parent),
    mHasImportMenu(false), // derived class should set this value accordingly
    mHasExportMenu(false),
    mSortItem(false), // Sorting disable by default. Sorting may cause list display incorrectly. TODO: fix it
    mParentModel(nullptr)
{
    traced;
}

UICommonListView::~UICommonListView()
{
    tracein;
    RELEASE_LIST_DBMODEL(mItemList);
    FREE_PTR(mParentModel);
    traceout;
}

void UICommonListView::setupUI()
{
    tracein;
    UITableView::setupUI();
    // register to receive dbmodel event, i.e. updated, added, etc.
    ModelController* ctl = getController();
    if (ctl) {
        ctl->addListener(this);
    } else {
        logw("no controller, may not received any db model event");
    }
    traceout;
}

QList<UITableMenuAction *> UICommonListView::getMenuCommonActions(const QMenu *menu)
{
    tracein;
    ErrCode err = ErrNone;
    QList<UITableMenuAction*> actionList;
    if (!menu) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        actionList = UITableView::getMenuCommonActions(menu);
        bool appendSeparator = false;
        if (hasImportMenuItem()) {
            logd("add import menu item");
            actionList.append(UITableMenuAction::build(STR_IMPORT_FROM_FILE, this)
                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                  return this->onMenuActionImport(m, a);
                              }));
            appendSeparator = true;
        }
        if (hasExportMenuItem()) {
            logd("add export menu item");
            actionList.append(UITableMenuAction::build(STR_EXPORT_TO_FILE, this)
                              ->setCallback([this](QMenu *m, UITableMenuAction *a)-> ErrCode{
                                  return this->onMenuActionExport(m, a);
                              }));
            appendSeparator = true;
        }
        if (appendSeparator) actionList.append(BUILD_MENU_SEPARATE);
    }
    traceout;
    return actionList;
}

bool UICommonListView::hasImportMenuItem()
{
    return mHasImportMenu;
}

bool UICommonListView::hasExportMenuItem()
{
    return mHasExportMenu;
}

ImportTarget UICommonListView::getImportTarget()
{
    return IMPORT_TARGET_MAX;
}

ModelController *UICommonListView::importController()
{
    return hasImportMenuItem()?getController():nullptr;
}

ModelController *UICommonListView::exportController()
{
    return hasExportMenuItem()?getController():nullptr;
}

ErrCode UICommonListView::getListTableRowItems(
    qint32 page, qint32 perPage, qint32 totalPages,
    QList<UITableItem*>& items)
{
    ErrCode err = ErrNone;
    int idx = 0;
    tracein;
    UNUSED(page);
    UNUSED(perPage);
    UNUSED(totalPages);
    foreach (DbModel* item, mItemList) {
        UITableItem* tblItem = UITableItem::build(item);
        if (tblItem) {
            err = fillValueTableRowItem(item, tblItem, ++idx);
            if (err == ErrNone) {
                items.append(tblItem);
            }
        } else {
            err = ErrNoMemory;
            loge("failed to allocate memory for tblItem");
        }
        if (err != ErrNone) {
            break;
        }
    }
    if (err != ErrNone) {
        loge("build table row item failed, err %d", err);
        RELEASE_LIST(items, UITableItem);
    }
    traceret(err);
    return err;
}

void UICommonListView::initHeader()
{
    tracein;
    mHeader.append(STR_NAMEID);
    mHeader.append(STR_NAME);
    mHeader.append(STR_NOTE);
    // TODO: header/column by order, i.e. 1: name id, 2: name, last: note
    traceout;
}
ErrCode UICommonListView::fillValueTableRowItem(DbModel *item, UITableItem *tblItem, int idx)
{
    tracein;
    ErrCode err = ErrNone;
    UNUSED(idx);
    if (!item || !tblItem) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }
    if (err == ErrNone) {
        tblItem->addValue(item->nameId());
        tblItem->addValue(item->name());
        tblItem->addValue(item->remark());
    }
    traceret(err);
    return err;
}

void UICommonListView::onUpdatePageDone(qint32 page, qint32 totalpages, qint32 totalItems)
{
    tracein;
    UITableView::onUpdatePageDone(page, totalpages, totalItems);
    logd("Sort %d", mSortItem);
    if (mSortItem) {
        // data from db was sorted, by data is stored in hashlist, which is not sorted
        // TODO: improve performance by using sorted list intead of hash map?
        // Sorting may cause list display incorrectly, i.e some field lost info
        // TODO: check why sorting cause some field in tableview list lost data
        sort(1); // name
    }
    traceout;
}

qint32 UICommonListView::getTotalItems()
{
    tracein;
    return mItemList.count();
}

QList<DbModel *> UICommonListView::getListDbModels()
{
    tracein;
    QList<DbModel *> ret;
    ModelController* ctl = getController();
    if (ctl) {
        logd("get list item from controller '%s'", STR2CHA(ctl->getName()));
        ret = ctl->getAllItems(true);
    } else {
        loge("no controller to get list items");
    }
    traceout;
    return ret;
}

ErrCode UICommonListView::onLoad()
{
    tracein;
    ErrCode err = ErrNone;
    if (ready2FetchData()) {
        RELEASE_LIST_DBMODEL(mItemList);
        if (mFilterList.size() > 0) {
            logd("Do filter");
            foreach (FilterItem* item, this->mFilterList) {
                logd("filter item %s", STR2CHA(item->item()));
                if (item->item() == KItemStatus) {
                    err = doFilter(FILTER_FIELD_MODEL_STATUS, FILTER_OP_EQUAL, item->value());
                    // TODO: support only one filter, so found then break;
                    // TODO: support more????
                    break;
                }
            }
            RELEASE_LIST(mFilterList, FilterItem);
        } else {
            logd("load all data");
            mItemList = getListDbModels();
            clearFilter();
        }
        err = ErrNone;
    } else {
        loge("not show, so not load data");
        err = ErrNotReady;
    }
    traceout;
    return err;
}

ErrCode UICommonListView::onReload()
{
    tracein;
    ErrCode err = ErrNone;
    if (ready2FetchData()) {
        err = onLoad();
    } else {
        loge("not show, so not reload data");
        err = ErrNotReady;
    }
    traceret(err);
    return err;
}

void UICommonListView::initFilterFields()
{
    tracein;
    // filter by model status
    appendFilterField(FILTER_FIELD_MODEL_STATUS, STR_MODELSTATUS);
    // filter by  name
    appendFilterField(FILTER_FIELD_NAME, STR_NAME);
    traceout;
}


QHash<QString, QString> UICommonListView::getFilterKeywords(int fieldId,
                                                            const QString &fieldText)
{
    tracein;
    logd("Query search keywords form db, field %d, %s", fieldId, STR2CHA(fieldText));
    QHash<QString, QString> keywords;
    QList<DbModel*> modelList;
    UNUSED(fieldText);
    // TODO: query may take time, should call in thread?
    switch (fieldId) {
    case FILTER_FIELD_HOLLY_NAME:
        modelList = SAINTCTL->getAllItems();
        break;
    case FILTER_FIELD_COMMUNITY:
        modelList = COMMUNITYCTL->getAllItems();
        break;
    case FILTER_FIELD_SPECIALIST:
        modelList = SPECIALISTCTL->getAllItems();
        break;
    case FILTER_FIELD_COURSE:
        modelList = COURSECTL->getAllItems();
        // TODO: support "unknown" item???
        break;
    case FILTER_FIELD_WORK:
        modelList = WORKCTL->getAllItems();
        break;
    case FILTER_FIELD_EDUCATION:
        modelList = EDUCTL->getAllItems();
        break;
    case FILTER_FIELD_MODEL_STATUS:
        {
            const QHash<int, QString>* map = DbModel::getModelStatusIdNameMap();
            if (map) {
                foreach (int id, map->keys()) {
                    keywords.insert(QString::number(id), map->value(id));
                }
            } else {
                loge("invalid status id name map");
            }
        }
        break;
    default:
        loge("Field %d not supported", fieldId);
    };
    if (modelList.count() > 0) {
        foreach (DbModel* item, modelList) {
            keywords.insert(item->uid(), item->name());
        }
    }
    logd("found %d keywords", (int)keywords.count());
    traceout;
    return keywords;
}

int UICommonListView::onFilter(int catetoryid, const QString &catetory,
                               qint64 opFlags, const QString &keywords,
                               const QVariant *value)
{
    tracein;
    if (keywords.isEmpty()) {
        logd("no filter keyword, do reload all");
        onReload();
    } else {
        QString modelName = getMainModelName();
        ModelController* ctrl = getController();
        ErrCode err = ErrNone;
        logi("filter with keyword '%s'", STR2CHA(keywords));
        if (!ctrl) {
            loge("Invalid controller");
            err = ErrInvalidController;
        }
        if (err == ErrNone && modelName.isEmpty()) {
            loge("Invalid model name");
            err = ErrInvalidModel;
        }
        if (err == ErrNone) {
            RELEASE_LIST_DBMODEL(mItemList);
            QString searchKeyWork;
            if (value && value->isValid()) {
                searchKeyWork = value->toString();
            }else {
                searchKeyWork = keywords;
            }
            logd("category id %d", catetoryid);
            logd("category '%s'", STR2CHA(catetory));
            logd("opFlags 0x%llx", opFlags);
            logd("searchKeyWork %s", STR2CHA(searchKeyWork));
            logd("modelName %s", STR2CHA(modelName));
            logd("ctrl %s", STR2CHA(ctrl->getName()));
            err = ctrl->filter(catetoryid,
                              opFlags, searchKeyWork,
                              modelName.toStdString().c_str(),
                              mParentModel,
                              &mItemList);
        }
        logd("filter err %d", err);
        logd("mItemList cnt %lld", mItemList.count());
    }
    traceout;
    return mItemList.count();
}


ErrCode UICommonListView::onViewItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    DbModel* model = nullptr;

    if (!item) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    if (err == ErrNone && ((model = item->itemData()) == nullptr)) {
        err = ErrInvalidData;
        loge("Invalid data");
    }

    if (err == ErrNone) {
        err = MainWindow::showOnHtmlViewer(model, getTitle());
    }
    if (err != ErrNone) {
        loge("view item err %d", err);
        DialogUtils::showErrorBox(err, tr("Hiển thị thông tin lỗi"));
    }
    traceret(err);
    return err;
}

ErrCode UICommonListView::onAddItem(UITableCellWidgetItem *item)
{
    tracein;
    UNUSED(item);
    // TODO: handle it
    MainWindow::showAddEditCommonModel(true, nullptr, this);
    traceout;
    return ErrNone;
}

ErrCode UICommonListView::onEditItem(UITableCellWidgetItem *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (!item) {
        loge("invalid arg");
        err = ErrInvalidArg;
    }
    if (err == ErrNone) {
        DbModel* comm = item->itemData();
        if (comm) {
            MainWindow::showAddEditCommonModel(true, comm, this);
        } else {
            loge("Model obj is null");
            DialogUtils::showErrorBox("Không có thông tin để chỉnh sửa");
            err = ErrInvalidData;
        }
    }
    traceret(err);
    return err;
}

void UICommonListView::onDbModelReady(ErrCode ret, DbModel *model, DlgCommonEditModel *dlg)
{
    tracein;
    UNUSED(dlg);
    if (ret == ErrNone) {
#ifdef DEBUG_LOG
        if (model){
            model->dump();
        }
#endif
        onReload();
    } else {
        loge("db model err = %d", ret);
    }

    traceout;
}

DbModel *UICommonListView::onCreateDbModelObj(const QString& modelName)
{
    tracein;
    UNUSED(modelName);
    return nullptr;
}

QString UICommonListView::getName()
{
    return "UICommonListView";
}

QString UICommonListView::getMainModelName()
{
    return "";
}

void UICommonListView::onModelControllerDataUpdated(const DbModel *model)
{
    tracein;
    logd("update model '%s' for list '%s'",
         model?MODELSTR2CHA(model):"(null)", STR2CHA(getName()));
    if (!mSuspendReloadOnDbUpdate) {
        QString mainModelName = getMainModelName();
        logd("main model name '%s'", STR2CHA(mainModelName));
        if (mainModelName.isEmpty() || (model && mainModelName == model->modelName())) {
            logd("reload");
            reload();
        }
    } else {
        logw("Suspend reload on data update");
    }
    traceout;
}


ErrCode UICommonListView::onMenuActionImport(QMenu *menu, UITableMenuAction *act)
{
    UNUSED(menu);
    UNUSED(act);
    tracein;
    ErrCode ret = ErrNone;
    ModelController* ctrl = importController();
    QString modelName = getMainModelName();
    ImportTarget importTarget = getImportTarget();
    if (!ctrl) {
        ret = ErrInvalidData;
        loge("Invalid import controller");
    }
    if (ret == ErrNone && modelName.isEmpty()) {
        ret = ErrInvalidData;
        loge("Invalid model name");
    }
    if (ret == ErrNone) {
        ret = MainWindow::showImportDlg(importTarget,
                                        ctrl,
                                        modelName,
                                        parentModel());
    }
    if (ret == ErrNone) {
        logi("Import for target 0x%x, model '%s' success",
             importTarget, STR2CHA(modelName));
    } else {
        loge("import failed, ret=%d", ret);
        DialogUtils::showErrorBox(ret, tr("Nhập dữ liệu không thành công"));
    }
    traceret(ret);
    return ret;
}

ErrCode UICommonListView::onMenuActionExport(QMenu *menu, UITableMenuAction *act)
{
    UNUSED(menu);
    UNUSED(act);
    tracein;
    ErrCode err = ErrNone;
    if (!act){
        err = ErrInvalidArg;
        loge("export list failed, Empty menu action");
    }
    if (err == ErrNone) {
        if (!mItemList.empty()) {
            err = MainWindow::exportListItems(&mItemList,
                                              getMainModelName(),
                                              exportController(),
                                              QString("%1: %2").arg(STR_EXPORT_TO_FILE, getTitle()),
                                              EXPORT_XLSX);
        } else {
            loge("nothing to export");
            err = ErrNoData;
        }
    }
    if (err != ErrNone){
        loge("export list failed, error code %d",err);
        DialogUtils::showErrorBox(QString("Xuất dữ liệu lỗi, mã lỗi: %1").arg(err));
    }
    traceret(err);
    return err;
}

DbModel *UICommonListView::parentModel() const
{
    return mParentModel;
}

ErrCode UICommonListView::setParentModel(const DbModel *newParentModel)
{
    tracein;
    ErrCode err = ErrNone;
    FREE_PTR(mParentModel);
    if (newParentModel) {
        mParentModel = CLONE_MODEL(newParentModel, DbModel);
    } else {
        logw("null value is set");
    }
    if (err == ErrNone) {
        // update title when setting parent model
        setTitle(getTitle());
    }
    traceret(err);
    return err;
}
