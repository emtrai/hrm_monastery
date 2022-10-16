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
#include "logger.h"
#include <QList>
#include "dbmodel.h"
#include "person.h"
#include "utils.h"
#include "view/dialog/dlgimportpersonlistresult.h"
#include "view/dialog/dlghtmlviewer.h"
#include <QFile>
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
    QList<DbModel*> items = PersonCtl::getInstance()->getAllPerson();
    traced;
    mItemList.clear(); // TODO: clean up item data
    // TODO: loop to much, redundant, do something better?
    foreach (DbModel* item, items) {
        mItemList.append(item);
    }
    return ErrNone;
}

void UIPersonListView::updateItem(DbModel *item, UITableItem *tblItem)
{
    traced;
    Person* per = (Person*) item;
    tblItem->addValue(per->uid());
    tblItem->addValue(per->hollyName());
    tblItem->addValue(per->getFullName());
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
    mHeader.append(tr("ID"));
    mHeader.append(tr("Tên Thánh"));
    mHeader.append(tr("Họ tên"));
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
    ErrCode ret = INSTANCE(PersonCtl)->importFromFile(nullptr, ImportType::IMPORT_CSV_LIST, fpath, &list);
    logd("Import result %d", ret);
    logd("No of import item %d", list.count());
    DlgImportPersonListResult* dlg = new DlgImportPersonListResult();
    dlg->setup(list);
    dlg->exec();
    delete dlg;
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
}

QString UIPersonListView::getTitle()
{
    return tr("Danh sách nữ tu");
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
