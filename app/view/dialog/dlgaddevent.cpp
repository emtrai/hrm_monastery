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
 * Filename: dlgaddevent.cpp
 * Author: Anh, Ngo Huy
 * Created date:9/7/2022
 * Brief:
 */
#include "dlgaddevent.h"
#include "ui_dlgaddevent.h"

DlgAddEvent::DlgAddEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddEvent)
{
    ui->setupUi(this);
}

DlgAddEvent::~DlgAddEvent()
{
    delete ui;
}
