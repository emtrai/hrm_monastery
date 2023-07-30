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
 * Filename: exportfactory.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#include "exportfactory.h"
#include "logger.h"
#include "errcode.h"
#include "utils.h"
#include "exporthtml.h"
#include "exportcsvlist.h"
#include "exportxlsx.h"
#include "prebuiltdefs.h"
#include "modeldefs.h"
#include "exporttype.h"

#define EXPORT_TEMPLATE_ID(name,type) QString("%1_%2").arg(name).arg(type)

QHash<QString, ExportTemplate*> ExportFactory::s_modelNameExportTemplateMap;

struct ExportTemplate {
    ExportTemplate(ExportType type, QString name, QString path);
    ExportType exportType;
    QString name;
    QString templatePath;
};

ExportTemplate::ExportTemplate(ExportType type, QString name, QString path)
{
    tracein;
    this->exportType = type;
    this->name = name;
    this->templatePath = path;
    traceout;
}

ExportFactory::ExportFactory()
{
    tracein;
}

FileExporter *ExportFactory::getExporter(ExportType type)
{
    FileExporter* ret = nullptr;
    tracein;
    logd("get export type %d", type);
    switch (type) {
    case EXPORT_HTML:
        ret = INSTANCE(ExportHtml);
        break;
    case EXPORT_CSV_LIST:
        ret = INSTANCE(ExportCSVList);
        break;
    case EXPORT_XLSX:
        ret = INSTANCE(ExportXlsx);
        break;
    default:
        loge("Export type %d not support", type);
        ret = nullptr;
        break;
    }
    return ret;
}

ErrCode ExportFactory::exportTo(const DataExporter *item,
                                const QString& datatype,
                                const QString &fpath,
                                ExportType type)
{
    tracein;
    ErrCode ret = ErrNone;
    logi("Export to %d", type);
    FileExporter* exporter = getExporter(type);
    if (exporter != nullptr)
        ret = exporter->saveTo(item, datatype, fpath);
    else {
        ret = ErrNotSupport;
        loge("FileExporter %d not support", type);
    }
    return ret;
}

ErrCode ExportFactory::exportTo(const DataExporter* item,
                                const QString& datatype, // datatype mostly modelname, or controller name
                                QList<DbModel*> data,
                                const QString &fpath,
                                ExportType type)
{
    tracein;
    // TODO: add "tag" parameter here to input to getExporter/saveTo???
    // so that derived class can have more information to judge which data should be used.
    // TODO: add title? (title for export info)
    ErrCode ret = ErrNone;
    logi("Export to %d, datatype '%s'", type, STR2CHA(datatype));
    FileExporter* exporter = getExporter(type);
    if (exporter != nullptr)
        ret = exporter->saveTo(item, datatype, data, fpath);
    else {
        ret = ErrNotSupport;
        loge("FileExporter %d not support", type);
    }
    return ret;
}

// this looks useless, re-consider again
// This cause objects not pure independent. All model/control object need to exist here
// We want each object/class need to independent together.
// currently, no one use this function, just put here for backup
QString ExportFactory::getExportTemplatePath(const QString &modelName, ExportType type, bool* ok)
{
    tracein;
    QString fpath;
    QString id = EXPORT_TEMPLATE_ID(modelName, type);
    ErrCode err = ErrNone;
    initExportTemplate();
    logd("model name '%s', type=%d, id='%s'", STR2CHA(modelName), type, STR2CHA(id));
    if (s_modelNameExportTemplateMap.contains(id)) {
        ExportTemplate* exportTemplate = s_modelNameExportTemplateMap.value(id);
        if (exportTemplate) {
            fpath = exportTemplate->templatePath;
        } else {
            err = ErrNoData;
            loge("export template id '%s' has no data", STR2CHA(id));
        }
    } else {
        fpath = KPrebuiltDefaultExportTemplateName;
        logi("export template id '%s' use default tempalte name", STR2CHA(id));
    }

    if (ok) *ok = (err = ErrNone);
    traceout;
    return fpath;
}

void ExportFactory::initExportTemplate()
{
    if (s_modelNameExportTemplateMap.size() == 0) {
        tracein;
        logd("init template");
        addExportTemplate(EXPORT_XLSX,
                          KModelNameAreaPerson,
                          KPrebuiltAreaContactExportTemplateName);
        traceout;
    }
}

void ExportFactory::addExportTemplate(ExportType type, const QString &modelName, QString fpath)
{
    logd("Add export template type %d name '%s' fpath '%s'",
         type, STR2CHA(modelName), STR2CHA(fpath));
    ExportTemplate *tpl = new ExportTemplate(type,
                                             modelName,
                                             fpath);
    if (tpl) {
        s_modelNameExportTemplateMap.insert(
            EXPORT_TEMPLATE_ID(modelName, type),tpl);
    } else {
        loge("No memory to allocate export template");
    }
}

