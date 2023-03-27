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
 * Filename: uiitembutton.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#include "uiitembutton.h"
#include "logger.h"

UIItemButton::UIItemButton(QWidget *parent)
    : QPushButton{parent}
{
    connect(this, SIGNAL(clicked()), this, SLOT(reEmitClick()));
    this->setStyleSheet("qproperty-icon: url(:/icon/icon/icons8-delete-64.png);");
}
UIItemButton::~UIItemButton()
{
    tracein;
}

const QVariant &UIItemButton::value() const
{
    return mValue;
}

void UIItemButton::setValue(const QVariant &newValue)
{
    mValue = newValue;
}


void UIItemButton::reEmitClick()
{
    tracein;
    emit clicked(this, mValue);
}
