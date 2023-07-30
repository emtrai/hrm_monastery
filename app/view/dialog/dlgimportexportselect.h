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
 * Filename: dlgimportexportselect.h
 * Author: Anh, Ngo Huy
 * Created date:2/19/2023
 * Brief:
 */
#ifndef DLGIMPORTEXPORTSELECT_H
#define DLGIMPORTEXPORTSELECT_H

#include <QDialog>
#include "errcode.h"
#include "exporttype.h"

namespace Ui {
class DlgImportExportSelect;
}

class DlgImportExportSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgImportExportSelect(QWidget *parent = nullptr);
    ~DlgImportExportSelect();

    ErrCode setExportTypes(quint64 exportTypes);
    void setImportExport (bool isExport, const QString& title);
    const QString &path() const;
    void setTitle(const QString& title);

    ExportType selectedExportType() const;
    void setSelectedExportType(ExportType newSelectedExportType);
protected:
    virtual void accept();
private:
    Ui::DlgImportExportSelect *ui;
    quint64 mExportType;
    bool mIsExport;
    QString mTitle;
    ExportType mSelectedExportType;
    QString mPath;
};

#endif // DLGIMPORTEXPORTSELECT_H
