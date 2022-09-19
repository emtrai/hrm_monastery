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
#include <QHash>
#include "view/widget/uiitembutton.h"

class UIMultiComboxView;

//typedef ErrCode (*onNewItem)(UIMultiComboxView* ui, const QString& value);

namespace Ui {
class UIMultiComboxView;
}

class UIMultiComboxViewListener {
public:
    // slient: add new item silently, don't ask/popup up anything
    virtual ErrCode onNewItem(UIMultiComboxView* ui, const QString& value, bool silent) = 0;
    virtual void onItemAdded(UIMultiComboxView* ui, const QString& name, const QVariant& value) = 0;
    virtual void onItemDeleted(UIMultiComboxView* ui, const QString& name, const QVariant& value) = 0;
    virtual void onClearAll() = 0;
};

class UIMultiComboxView : public QFrame
{
    Q_OBJECT

public:
    explicit UIMultiComboxView(QWidget *parent = nullptr);
    explicit UIMultiComboxView(const QString& name, QWidget *parent = nullptr);
    ~UIMultiComboxView();

    ErrCode addItem(const QString& name, const QVariant& value=QVariant());


    void setListener(UIMultiComboxViewListener *newListener);

    const QHash<QString, QVariant> &items() const;
    QList<QVariant> valueItems() const;

    const QString &name() const;
    void clearAll();

private slots:
    void on_btnAdd_clicked();
    void on_item_clicked( UIItemButton * button, QVariant value );

private:
    Ui::UIMultiComboxView *ui;
    QVBoxLayout *mContainerLayout;
    QHash<QString, QVariant> mValueList;
    UIMultiComboxViewListener* mListener;
    QString mName;
    QList<UIItemButton*> mButtonList;
};

#endif // UIMULTICOMBOXVIEW_H
