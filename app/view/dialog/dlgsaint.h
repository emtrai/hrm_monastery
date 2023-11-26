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
 * Filename: dlgsaint.h
 * Author: Anh, Ngo Huy
 * Created date:8/25/2022
 * Brief:
 */
#ifndef DLGSAINT_H
#define DLGSAINT_H

#include <QDialog>

class Saint;

namespace Ui {
class DlgSaint;
}

class DlgSaint : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSaint(QWidget *parent = nullptr);
    ~DlgSaint();

    Saint *saint() const;
    void setName(const QString& name);
protected:

    virtual void accept();
private slots:

    void on_txtName_textChanged(const QString &arg1);

    void on_btnChangeNameId_clicked();

private:
    Ui::DlgSaint *ui;
    Saint* mSaint;
    bool mCustomNameId;
};

#endif // DLGSAINT_H
