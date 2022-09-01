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
 * Filename: uimulticomboxview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#include "uimulticomboxview.h"
#include "ui_uimulticomboxview.h"
#include "logger.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QAbstractButton>

#define COLUMN (2)

UIMultiComboxView::UIMultiComboxView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UIMultiComboxView),
    mListener(nullptr)
{
    ui->setupUi(this);
//    mContainerLayout = new QVBoxLayout;
    //    ui->wContainer->setLayout( layout );
}

UIMultiComboxView::UIMultiComboxView(const QString &name, QWidget *parent):
    UIMultiComboxView(parent)
{
    traced;
    mName = name;
}



UIMultiComboxView::~UIMultiComboxView()
{
    delete ui;
}

ErrCode UIMultiComboxView::addItem(const QString &name, const QVariant& value)
{
    traced;
    ui->cbItems->addItem(name, value);

    return ErrNone;
}

void UIMultiComboxView::on_btnAdd_clicked()
{
    traced;
    ErrCode ret = ErrNone;
    QString currtxt = ui->cbItems->currentText().trimmed();

    int index = ui->cbItems->findText(currtxt);
    logd("index %d", index);
    if (index < 0){
        if (mListener != nullptr) {
            ret = mListener->onNewItem(this, currtxt, false);
        } else {
            ret = ErrNotFound;
            loge("not found data");
        }
        if (ret == ErrNone){
            index = ui->cbItems->findText(currtxt);
            if (index < 0){
                ret = ErrNotFound;
                loge("not found data");
            }
        }
    }
    if (ret == ErrNone){
        QVariant value = ui->cbItems->itemData(index);

        if (mValueList.keys().contains(currtxt)) {
            logi("Existed %s", currtxt.toStdString().c_str());
            return;
        }

        UIItemButton* lbl = new UIItemButton();

        QObject::connect(lbl,
                         SIGNAL(clicked(UIItemButton*,QVariant)),
                         this,
                         SLOT(on_item_clicked(UIItemButton*,QVariant)));
        lbl->setText(currtxt);
        lbl->setValue(value);
        // TODO: use text/name as key for array here, something quite bad!!!
        mValueList.insert(currtxt, value);
        ui->formLayout->addWidget(lbl);
        if (mListener != nullptr) {
            mListener->onItemAdded(this, currtxt, value);
        }
    }
}

void UIMultiComboxView::on_item_clicked( UIItemButton * button, QVariant value){
    traced;
    logd("Remain count %d", mValueList.count());
    mValueList.remove(button->text());
    logd("Remove %s", button->text().toStdString().c_str());
    ui->formLayout->removeWidget(button);
    if (mListener != nullptr) {
        mListener->onItemDeleted(this, button->text(), value);
    }
    delete button;
}

const QString &UIMultiComboxView::name() const
{
    return mName;
}

const QHash<QString, QVariant> &UIMultiComboxView::items() const
{
//    QHash<QString, QString> item;
//    foreach (QVariant key, mValueList.keys()) {
//        item.insert(key.toString(), mValueList.value(key));
//    }
    return mValueList;
}

QList<QVariant> UIMultiComboxView::valueItems() const
{
    return mValueList.values();
}

void UIMultiComboxView::setListener(UIMultiComboxViewListener *newListener)
{
    mListener = newListener;
}



