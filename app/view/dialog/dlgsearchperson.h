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
 * Filename: dlgsearchperson.h
 * Author: Anh, Ngo Huy
 * Created date:9/20/2022
 * Brief:
 */
#ifndef DLGSEARCHPERSON_H
#define DLGSEARCHPERSON_H

#include <QDialog>

class Person;
namespace Ui {
class DlgSearchPerson;
}

class DlgSearchPerson : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSearchPerson(QWidget *parent = nullptr);
    ~DlgSearchPerson();


    Person *person() const;

protected:
    void accept();
private slots:
    void on_btnSearch_clicked();

private:
    Ui::DlgSearchPerson *ui;
    Person* mPerson;
    QList<Person*> mListPerson;
};

#endif // DLGSEARCHPERSON_H
