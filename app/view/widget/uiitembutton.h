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
 * Filename: uiitembutton.h
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#ifndef UIITEMBUTTON_H
#define UIITEMBUTTON_H

#include <QPushButton>

class UIItemButton : public QPushButton
{
    Q_OBJECT
public:
    explicit UIItemButton(QWidget *parent = nullptr);
    virtual ~UIItemButton();

public:
    QVariant mValue;


    const QVariant &value() const;
    void setValue(const QVariant &newValue);

signals:
    void clicked(UIItemButton*, QVariant);
public slots:
    void reEmitClick();
};

#endif // UIITEMBUTTON_H
