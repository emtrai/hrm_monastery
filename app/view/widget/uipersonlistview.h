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
 * Filename: uipersonlistview.h
 * Author: Anh, Ngo Huy
 * Created date:8/11/2022
 * Brief:
 */
#ifndef UIPERSONLISTVIEW_H
#define UIPERSONLISTVIEW_H

#include "uicommonlistview.h"
#include "importlistener.h"
#include "mainwindow.h"

class PersonEvent;
class QMessageBox;
class CommunityPerson;

class UIPersonListView : public UICommonListView, public ImportListener, public MainWindownImportListener
{
    Q_OBJECT
public:
    explicit UIPersonListView(QWidget *parent = nullptr);
    virtual ~UIPersonListView();
    virtual void setupUI();
    virtual QString getName();
protected:
    virtual int getViewType() { return VIEW_PERSON;}
    virtual ModelController* getController();
    virtual QString getMainModelName();
    virtual DbModel* onCreateDbModelObj(const QString& modelName);

    virtual void initHeader();
    virtual ErrCode fillValueTableRowItem(DbModel* item, UITableItem* tblItem, int idx);


    virtual ErrCode onAddItem(UITableCellWidgetItem *item);
//    virtual ErrCode onViewItem(UITableCellWidgetItem *item);
    virtual ErrCode onEditItem(UITableCellWidgetItem *item);

    virtual ErrCode onLoad();
    /**
     * @brief Get menu actions list when an item is selected
     * @param menu
     * @param item
     * @return
     */
    virtual QList<UITableMenuAction*> getMenuSingleSelectedItemActions(const QMenu* menu, UITableCellWidgetItem* item);
    /**
     * @brief get menu action list when multi item is selected
     * @param menu
     * @param items
     * @return
     */
    virtual QList<UITableMenuAction*> getMenuMultiSelectedItemActions(const QMenu* menu, const QList<UITableItem *>& items);

//    virtual ErrCode onMenuActionImport(QMenu* menu, UITableMenuAction* act);
//    virtual ErrCode onMenuActionExportListPerson(QMenu *menu, UITableMenuAction *act);
    virtual ErrCode onMenuActionViewPersonEvent(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionViewCommunity(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionAddPersonEvent(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode onMenuActionChangeCommunity(QMenu* menu, UITableMenuAction* act);
    virtual ErrCode exportPersonInfo(QMenu* menu, UITableMenuAction* act);
    virtual QString getTitle();

    virtual void initFilterFields();

    virtual int onFilter(int catetoryid,
                          const QString& catetory,
                          qint64 opFlags,
                          const QString& keywords, const QVariant *value);
    virtual void onImportStart(const QString& importName, const QString& fpath, ImportType type);
    virtual void onImportEnd(const QString& importName, ErrCode err, const QString& fpath, ImportType type);

    virtual void onMainWindownImportStart(ImportTarget target);
    virtual void onMainWindownImportEnd(ImportTarget target, ErrCode err, void* importData = nullptr);


    virtual ErrCode updatePersonEvent(const QList<DbModel*>& perList, const PersonEvent* event = nullptr);
private:
    void cleanUpItem();
    ErrCode buildPersonCommunityChangeEventList(const QList<DbModel *> & perList,
                                                const Community * comm,
                                                QList<PersonEvent*>& perEventList);
    ErrCode buildInActiveCommPersonList(const QList<DbModel *> & perList,
                                QList<CommunityPerson*>& commPerList);
    ErrCode buildActiveCommPersonList(const QList<DbModel *> & perList,
                                      const Community * comm,
                                    QList<CommunityPerson*>& commPerList);
//    void onConfirmChangeCommunity(const QList<DbModel *> & perList, const Community * comm);
    ErrCode onConfirmAddPersonEventChangeCommunity(const QList<DbModel *> & perList,
                                                const Community * comm);
    ErrCode onConfirmAddHistoryCommunity(const QList<DbModel *> & perList,
                                                const Community * comm);

signals:
    void changeCommunityDone(ErrCode err, QList<DbModel*>, Community*, bool, bool);
    void confirmChangeCommunity(const QList<DbModel *> & perList, const Community * comm);
private slots:
    void onChangeCommunityDone(ErrCode err, QList<DbModel*>, Community*, bool, bool);
};

#endif // UIPERSONLISTVIEW_H
