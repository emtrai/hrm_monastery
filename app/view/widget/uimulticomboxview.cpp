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
    ui(new Ui::UIMultiComboxView)
{
    ui->setupUi(this);
    mContainerLayout = new QVBoxLayout;
//    ui->wContainer->setLayout( layout );
}



UIMultiComboxView::~UIMultiComboxView()
{
    delete ui;
}

ErrCode UIMultiComboxView::addItem(const QString &name, const QVariant& data)
{
    traced;
    ui->cbItems->addItem(name, data);
    return ErrNone;
}

void UIMultiComboxView::on_btnAdd_clicked()
{
    traced;
    UIItemButton* lbl = new UIItemButton();
    lbl->setText(ui->cbItems->currentText());

    QObject::connect(lbl,
                     SIGNAL(clicked(UIItemButton*,QObject*)),
                     this,
                     SLOT(on_item_clicked(UIItemButton*,QObject*)));
    ui->formLayout->addWidget(lbl);
}

void UIMultiComboxView::on_item_clicked( UIItemButton * button, QObject* data ){
    traced;
    ui->formLayout->removeWidget(button);
    delete button;
}

