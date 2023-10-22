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
 * Filename: statistic.h
 * Author: Anh, Ngo Huy
 * Created date:7/17/2022
 * Brief:
 */
#ifndef STATISTIC_H
#define STATISTIC_H

#include "idataimporter.h"
#include "dataexporter.h"
#include "utils.h"
#include "fileexporter.h"

#define STAT Statistic::getInstance()
typedef std::function<QString(const FileExporter*,const QString&)> StatExportCallbackFunc;


class Statistic: public DataExporter
{
    GET_INSTANCE_DECL(Statistic);
public:
    Statistic();
    virtual QString getName() const;

    /**
     * @brief export general statistic to file
     * @param fpath output file path, if it's null, returned with file in tmp folder
     * @param type export type, i.e. HTML
     * @return
     */
    ErrCode exportGeneralStatistic(QString* fpath, ExportType type = EXPORT_HTML);

    /**
     * @brief get path of template to export
     * @param exporter
     * @param name
     * @param fpath
     * @param ftype
     * @return
     */
    virtual ErrCode exportTemplatePath(FileExporter* exporter,
                                       const QString& name,
                                       QString& fpath,
                                       QString* ftype = nullptr) const;

    /**
     * @brief Get data to be exported
     * @param[in] item keyword/data/item to be exported
     * @param[out] data data to be exported
     * @return
     */
    virtual ErrCode getExportDataString(const QString& item,
                                        const FileExporter* fileexporter,
                                        const QString& datatype, QString* data) const;

    /**
     * @brief return supported export type/target, i.e HTMO
     *        in bitwise form, \ref ExportType
     * @return bitwise form of supported export type, \ref ExportType
     */
    virtual quint64 getExportTypeList();

    /**
     * @brief get list of supported keyword, i.e "TONG_NU_TU", etc.
     * @return \ref exportdefs.h
     */
    virtual const QStringList getListExportKeyWord() const;
private:
    void initExportFields();
private:
    QHash<QString, StatExportCallbackFunc> mExportCallbacks;
};

#endif // STATISTIC_H
