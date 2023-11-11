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
#include "datetimeutils.h"
#include "logger.h"
#include "errcode.h"
#include "course.h"
#include "stringdefs.h"

#include <QDialogButtonBox>
DlgCourse::DlgCourse(QWidget *parent) :
    DlgCommonEditModel(parent),
    ui(new Ui::DlgCourse)
{
    ui->setupUi(this);
    loadCourseType();
}

DlgCourse::~DlgCourse()
{
    delete ui;
}

void DlgCourse::setupUI()
{
    tracein;
    if (ui->buttonBox->button(QDialogButtonBox::Cancel))
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(STR_CANCEL);
    if (ui->buttonBox->button(QDialogButtonBox::Ok))
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText(STR_OK);
    traceout;
}

ErrCode DlgCourse::buildModel(DbModel *model, QString &errMsg)
{
    tracein;
    ErrCode err = ErrNone;
    Course* comm = (Course*) model;
    if (!model){
        err = ErrInvalidArg;
        loge("Invalid arg");
    }
    if (err == ErrNone){
        comm->setMarkModified(true); // start marking fields which are modified
    }
    if (err == ErrNone) {
        QString name = ui->txtName->text().trimmed();
        if (!name.isEmpty()) {
            comm->setName(name);
        }
    }
    if (err == ErrNone){
        QString name = ui->txtNameId->text().trimmed();
        if (!name.isEmpty()) {
            comm->setNameId(name);
        }
    }
    if (err == ErrNone){
        SET_INT_VAL_FROM_CBOX(ui->cbCourseType, comm->setCourseType, comm->setCourseTypeName);
    }
    if (err == ErrNone){
        comm->setPeriod(ui->txtPeriod->text().trimmed());
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtStartDate, comm->setStartDate, DEFAULT_FORMAT_YMD);
    }
    if (err == ErrNone){
        SET_DATE_FORMAT_VAL_FROM_WIDGET(ui->txtEndDate, comm->setEndDate, DEFAULT_FORMAT_YMD);
    }
    if (err == ErrNone){
        comm->setRemark(ui->txtRemark->toPlainText().trimmed());
    }
    traceret(err);
    return err;
}

ErrCode DlgCourse::fromModel(const DbModel *item)
{
    tracein;
    ErrCode err = ErrNone;
    if (item && item->modelName() == KModelNameCourse) {
        err = DlgCommonEditModel::fromModel(item);
        Course* comm = (Course*)model();
        if (err == ErrNone) {
            ui->txtName->setText(comm->name());
            ui->txtNameId->setText(comm->nameId()); // TODO: auto generate???
            ui->txtPeriod->setText(comm->period());
            Utils::setSelectItemComboxByData(ui->cbCourseType, comm->courseType());
            ui->txtStartDate->setText(DatetimeUtils::date2String(comm->startDate(), DEFAULT_FORMAT_YMD));
            ui->txtEndDate->setText(DatetimeUtils::date2String(comm->endDate(), DEFAULT_FORMAT_YMD));
            ui->txtRemark->setPlainText(comm->remark());
        }
    } else {
        err = ErrInvalidArg;
        loge("null item or invalid type");
    }
    traceret(err);
    return err;
}

DbModel *DlgCourse::newModel()
{
    return Course::build();
}

void DlgCourse::loadCourseType()
{
    tracein;
    ui->cbCourseType->clear();
    const QHash<int, QString>* courses = Course::getCourseTypeNameMap();
    logd("the number of courses %lld", courses->count());
    foreach (int key, courses->keys()) {
        ui->cbCourseType->addItem(courses->value(key), key);
    }
    traceout;
}

void DlgCourse::on_btnChangeNameId_clicked()
{
    DlgCommonEditModel::onEditnameId(ui->txtNameId);
    if (ui->txtNameId->text().length() == 0) { // custome nameid is null, make it auto generate
        onChangeNameIdTxt(ui->txtNameId, ui->txtName->text());
    }
}


void DlgCourse::on_txtName_textChanged(const QString &arg1)
{
    DlgCommonEditModel::onChangeNameIdTxt(ui->txtNameId, arg1);

}

