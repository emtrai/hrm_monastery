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
    tracein;
    ui->setupUi(this);
    traceout;
}

UIMultiComboxView::UIMultiComboxView(const QString &name, QWidget *parent):
    UIMultiComboxView(parent)
{
    tracein;
    mName = name;
    traceout;
}

UIMultiComboxView::~UIMultiComboxView()
{
    delete ui;
}

ErrCode UIMultiComboxView::addItem(const QString &name, const QVariant& value)
{
    tracein;
    ui->cbItems->addItem(name, value);
    traceout;
    return ErrNone;
}

void UIMultiComboxView::on_btnAdd_clicked()
{
    tracein;
    QString currtxt = ui->cbItems->currentText().trimmed();

    if (!currtxt.isEmpty()) {
        addSelectedItemByName(currtxt);
    }
    traceout;
}

void UIMultiComboxView::on_item_clicked( UIItemButton * button, QVariant value){
    tracein;
    logd("Remain count %lld", mValueList.count());
    if (button) {
        mValueList.remove(button->text());
        logd("Remove %s", button->text().toStdString().c_str());
        ui->formLayout->removeWidget(button);
        if (mListener != nullptr) {
            mListener->onComboxItemDeleted(this, button->text(), value);
        }
        if (mButtonList.removeOne(button)){
            logd("Removed button %s", button->text().toStdString().c_str());
        }
        delete button; // TODO: is it safe to delete here????
    } else {
        loge("no button to delete");
    }
    traceout;
}

const QString &UIMultiComboxView::name() const
{
    return mName;
}

void UIMultiComboxView::clearAll()
{
    tracein;
    ui->cbItems->clear();

    mValueList.clear();
    foreach (UIItemButton* btn, mButtonList) {
        if (btn) {
            ui->formLayout->removeWidget(btn);
            delete btn;
        }
    }
    mButtonList.clear();
    if (mListener != nullptr) {
        logd("call callback");
        mListener->onComboxClearAll();
    }
    traceout;
}

void UIMultiComboxView::addSelectedItemByName(const QString &txt)
{
    tracein;
    ErrCode ret = ErrNone;

    int index = ui->cbItems->findText(txt);
    logd("index %d", index);
    if (index < 0) {
        if (mListener != nullptr) {
            ret = mListener->onComboxNewItem(this, txt, false);
        } else {
            ret = ErrNotFound;
            loge("not found data");
        }
        if (ret == ErrNone) {
            index = ui->cbItems->findText(txt);
            if (index < 0) {
                ret = ErrNotFound;
                loge("not found data");
            }
        }
    }
    if (ret == ErrNone) {
        QVariant value = ui->cbItems->itemData(index);

        if (mValueList.keys().contains(txt)) {
            logi("Existed %s", txt.toStdString().c_str());
            return;
        }

        UIItemButton* lbl = new UIItemButton();
        if (lbl) {
            QObject::connect(lbl,
                             SIGNAL(clicked(UIItemButton*,QVariant)),
                             this,
                             SLOT(on_item_clicked(UIItemButton*,QVariant)));
            lbl->setText(txt);
            lbl->setValue(value);
            // TODO: use text/name as key for array here, something quite bad!!!
            mValueList.insert(txt, value);
            ui->formLayout->addWidget(lbl);
            if (mListener != nullptr) {
                mListener->onComboxItemAdded(this, txt, value);
            }
            mButtonList.append(lbl);
        } else {
            loge("not memory to create label");
        }
    }
    traceout;
}

void UIMultiComboxView::addSelectedItemByData(const QVariant &data)
{
    tracein;
    // TODO: assume data is string, how about others kind of value????
    int cnt = ui->cbItems->count();
    logd ("no. item %d", cnt);
    logd ("add item with data %s", data.toString().toStdString().c_str());
    for (int i = 0; i < cnt; i++) {
        QVariant value = ui->cbItems->itemData(i);
        logd("value %s", value.toString().toStdString().c_str());
        if (value.isValid() && data == value) {
            QString txt = ui->cbItems->itemText(i);
            logd("text %s", txt.toStdString().c_str());
            if (mValueList.keys().contains(txt)) {
                logi("Existed %s", txt.toStdString().c_str());
            } else {
                logd("Add item ");
                UIItemButton* lbl = new UIItemButton();

                QObject::connect(lbl,
                                 SIGNAL(clicked(UIItemButton*,QVariant)),
                                 this,
                                 SLOT(on_item_clicked(UIItemButton*,QVariant)));
                lbl->setText(txt);
                lbl->setValue(value);
                // TODO: use text/name as key for array here, something quite bad!!!
                mValueList.insert(txt, value);
                ui->formLayout->addWidget(lbl);
                if (mListener != nullptr) {
                    mListener->onComboxItemAdded(this, txt, value);
                }
                mButtonList.append(lbl);
            }
            break;
        }
    }
    traceout;
}


const QHash<QString, QVariant> &UIMultiComboxView::items() const
{
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
