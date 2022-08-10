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
 * Filename: uimulticomboxview.h
 * Author: Anh, Ngo Huy
 * Created date:8/7/2022
 * Brief:
 */
#ifndef UIMULTICOMBOXVIEW_H
#define UIMULTICOMBOXVIEW_H

#include <QFrame>

#include "errcode.h"
#include <QString>
#include <QVariant>
#include <QVBoxLayout>
#include <QPushButton>

#include "view/widget/uiitembutton.h"

namespace Ui {
class UIMultiComboxView;
}

class UIMultiComboxView : public QFrame
{
    Q_OBJECT

public:
    explicit UIMultiComboxView(QWidget *parent = nullptr);
    ~UIMultiComboxView();

    ErrCode addItem(const QString& name, const QVariant &data = QVariant());

private slots:
    void on_btnAdd_clicked();
    void on_item_clicked( UIItemButton * button, QObject* data );

private:
    Ui::UIMultiComboxView *ui;
    QVBoxLayout *mContainerLayout;
};

#endif // UIMULTICOMBOXVIEW_H
