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
 * Filename: dlgprovince.h
 * Author: Anh, Ngo Huy
 * Created date:9/1/2022
 * Brief:
 */
#ifndef DLGPROVINCE_H
#define DLGPROVINCE_H

#include <QDialog>

class Province;

namespace Ui {
class DlgProvince;
}

class DlgProvince : public QDialog
{
    Q_OBJECT

protected:
    void accept();
public:
    explicit DlgProvince(QWidget *parent = nullptr);
    ~DlgProvince();
    void loadCountry();
    void loadProvince();

    Province *province() const;

private slots:
    void on_btnAddCountry_clicked();

    void on_btnAddCountry_triggered(QAction *arg1);

private:
    Ui::DlgProvince *ui;
    Province* mProvince;
};

#endif // DLGPROVINCE_H
