/*
 * Copyright (C) 2023 Ngo Huy Anh
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
 * Filename: dlgeditmodel.h
 * Author: Anh, Ngo Huy
 * Created date:2/16/2023
 * Brief:
 */
#ifndef DLGEDITMODEL_H
#define DLGEDITMODEL_H

#include <QDialog>
#include "dlgcommoneditmodel.h"

namespace Ui {
class DlgEditModel;
}

class DlgEditModel : public DlgCommonEditModel
{
    Q_OBJECT
    DLG_BUILDER(DlgEditModel)

public:
    explicit DlgEditModel(QWidget *parent = nullptr);
    virtual ~DlgEditModel();
    /**
     * @brief build data for model, using dialog/wiget data
     * @return
     */
    virtual ErrCode buildModel(DbModel* model, QString& errMsg);
    /**
     * @brief build model from input model object
     * @param model
     * @return
     */
    virtual ErrCode fromModel(const DbModel* model);
protected:
    virtual DbModel *newModel();
    /**
     * @brief Validate data of dialog
     * @param msg: output message if validate is not ok
     * @return
     */
    virtual bool onValidateData(QString& msg);

private:
    Ui::DlgEditModel *ui;
};

#endif // DLGEDITMODEL_H
