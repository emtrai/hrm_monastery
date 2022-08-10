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
 * Filename: uicommunityview.h
 * Author: Anh, Ngo Huy
 * Created date:8/4/2022
 * Brief:
 */
#ifndef UICOMMUNITYVIEW_H
#define UICOMMUNITYVIEW_H

#include <QFrame>

namespace Ui {
class UICommunityView;
}

class UICommunityView : public QFrame
{
    Q_OBJECT

public:
    explicit UICommunityView(QWidget *parent = nullptr);
    ~UICommunityView();
protected:
    void showEvent(QShowEvent *ev);
private:
    Ui::UICommunityView *ui;

    QStringList mHeader;
};

#endif // UICOMMUNITYVIEW_H
