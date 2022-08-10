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
 * Filename: uicommunityview.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/4/2022
 * Brief:
 */
#include "uicommunityview.h"
#include "ui_uicommunityview.h"
#include <QShowEvent>

#include "errcode.h"
#include "logger.h"
#include "communityctl.h"

UICommunityView::UICommunityView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UICommunityView)
{
    ui->setupUi(this);

    mHeader.append("ID");
    mHeader.append("Name");
    ui->tblCommunity->setHorizontalHeaderLabels(mHeader);
}

UICommunityView::~UICommunityView()
{
    delete ui;
}

void UICommunityView::showEvent(QShowEvent *ev)
{
    QFrame::showEvent(ev);
    traced;


}
