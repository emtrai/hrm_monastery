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
 * Filename: dlgaddpersonevent.h
 * Author: Anh, Ngo Huy
 * Created date:9/15/2022
 * Brief:
 */
#ifndef DLGADDPERSONEVENT_H
#define DLGADDPERSONEVENT_H

#include <QDialog>
#include <QHash>
class PersonEvent;
class Event;

namespace Ui {
class DlgAddPersonEvent;
}

class DlgAddPersonEvent : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddPersonEvent(QWidget *parent = nullptr);
    ~DlgAddPersonEvent();

    PersonEvent *personEvent() const;

protected:
    void accept();
private:
    Ui::DlgAddPersonEvent *ui;
    PersonEvent* mPersonEvent;
};

#endif // DLGADDPERSONEVENT_H
