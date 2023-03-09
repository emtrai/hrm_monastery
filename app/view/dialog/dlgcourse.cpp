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
 * Filename: dlgcourse.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/30/2022
 * Brief:
 */
#include "dlgcourse.h"
#include "ui_dlgcourse.h"
#include "utils.h"
#include "logger.h"
#include "errcode.h"
#include "course.h"
#include "coursectl.h"

DlgCourse::DlgCourse(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCourse),
    mCourse(nullptr)
{
    ui->setupUi(this);
}

DlgCourse::~DlgCourse()
{
    if (mCourse != nullptr)
        delete mCourse;
    delete ui;
}

void DlgCourse::accept()
{
    traced;
    ErrCode ret = ErrNone;
    QString name = ui->txtName->text().trimmed();

    if (!name.isEmpty()){
        if (mCourse == nullptr)
            mCourse = (Course*)Course::build();
        if (mCourse != nullptr){
            mCourse->setName(name);

            mCourse->setUid(Utils::UidFromName(name));


            mCourse->setPeriod(ui->txtPeriod->text().trimmed());
            mCourse->setStartDate(Utils::dateFromString(ui->txtStartDate->text().trimmed()));
            mCourse->setEndDate(Utils::dateFromString(ui->txtEndDate->text().trimmed()));
            mCourse->setRemark(ui->txtRemark->toPlainText().trimmed());

            logi("Save new course to db");
            ret = mCourse->save();
            if (ret == ErrNone){
                INSTANCE(CourseCtl)->reloadDb();
            }
        } else {
            ret = ErrNoMemory;
            loge("No memory");
        }
    } else {
        ret = ErrInvalidData;
        loge("Name field is empty");
    }

    logi("Add Course, ret %d", ret);
    if (ret == ErrNone)
        QDialog::accept();
    else
        Utils::showErrorBox(QString(tr("Lỗi ! Mã lỗi %1").arg(ret)));

}

Course *DlgCourse::course() const
{
return mCourse;
}
