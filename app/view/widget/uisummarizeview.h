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
 * Filename: uisummarizeview.h
 * Author: Anh, Ngo Huy
 * Created date:8/4/2022
 * Brief:
 */
#ifndef UISUMMARIZEVIEW_H
#define UISUMMARIZEVIEW_H

#include <QFrame>
#include "baseview.h"
namespace Ui {
class UISummarizeView;
}

class UISummarizeView : public QFrame, public BaseView
{
    Q_OBJECT

public:
    explicit UISummarizeView(QWidget *parent = nullptr);
    ~UISummarizeView();
    virtual QWidget* getWidget() {return this; }
protected:
    virtual int getViewType() { return VIEW_SUMMARY;}
    virtual void showEvent(QShowEvent *ev);
private slots:
    void on_btnExport_clicked();
//    void load();
private slots:
    void onLoad();
private:
    Ui::UISummarizeView *ui;
    QString mGeneralStatFpath;
};

#endif // UISUMMARIZEVIEW_H
