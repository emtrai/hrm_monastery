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
 * Filename: statistic.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/17/2022
 * Brief:
 */
#include "statistic.h"
#include "logger.h"
#include "errcode.h"
#include "controllerdefs.h"
#include "exporttype.h"
#include "prebuiltdefs.h"
#include "filectl.h"
#include "exportdefs.h"
#include "defs.h"
#include "personctl.h"
#include "communityctl.h"
#include "areactl.h"
#include "personstatusctl.h"
#include "stringdefs.h"

GET_INSTANCE_IMPL(Statistic)

#define EXPORT_GENERAL_STAT "statistic@general"

Statistic::Statistic()
{
    tracein;
    initExportFields();
    traceout;
}

QString Statistic::getName() const
{
    return KControllerStatistic;
}

void Statistic::initExportFields()
{
    tracein;
    mExportCallbacks.insert(KItemTotalPeople, [](const FileExporter* fileexporter,
                                                 const QString& datatype){
        UNUSED(fileexporter);
        UNUSED(datatype);
        return QString::number(PERSONCTL->getTotalItems());
    });
    mExportCallbacks.insert(KItemTotalCommunity, [](const FileExporter* fileexporter,
                                                    const QString& datatype){
        UNUSED(fileexporter);
        UNUSED(datatype);
        return QString::number(COMMUNITYCTL->getTotalActiveItems());
    });
    mExportCallbacks.insert(KItemTotalActivePeople, [](const FileExporter* fileexporter,
                                                       const QString& datatype){
        UNUSED(fileexporter);
        UNUSED(datatype);
        return QString::number(PERSONCTL->getTotalActiveItems());
    });
    mExportCallbacks.insert(KItemTotalPeopleStatus, [](const FileExporter* fileexporter,
                                                       const QString& datatype){
        UNUSED(fileexporter);
        UNUSED(datatype);
        QList<DbModel*> personStatus = PERSONSTATUSCTL->getAllItems();
        QStringList str;
        ExportType exportType = fileexporter->getExportType();
        logd("get total item by person status, exportType=%d", exportType);
        int cnt = PERSONCTL->getTotalItemsByPersonStatus(nullptr);
        if (cnt >= 0) {
            if (exportType == EXPORT_HTML) {
                str.append(QString("<tr><td>%1</td><td>%2</td></tr>").arg(STR_UNKNOWN).arg(cnt));
            } else {
                str.append(QString("%1:%2").arg(STR_UNKNOWN).arg(cnt));
            }
        } else {
            logd("no item unknown");
        }
        dbg(LOG_DEBUG, "personStatus sz %lld", personStatus.size());
        foreach (DbModel* status, personStatus) {
            if (!status) continue;

            cnt = PERSONCTL->getTotalItemsByPersonStatus(status->uid());
            logd("status uid '%s' cnt '%d'", STR2CHA(status->uid()), cnt);
            if (cnt >= 0) {
                if (exportType == EXPORT_HTML) {
                    str.append(QString("<tr><td>%1</td><td>%2</td></tr>")
                        .arg(status->name()).arg(cnt));
                } else {
                    str.append(QString("%1:%2").arg(status->name()).arg(cnt));
                }
            } else {
                loge("person status '%s' failed %d", STR2CHA(status->uid()), -(cnt));
                if (exportType == EXPORT_HTML) {
                    str.append(QString("<tr><td>%1</td><td>%2 (%3)</td></tr>")
                                   .arg(status->name(), STR_ERROR)
                                   .arg(-(cnt)));
                } else {
                    str.append(QString("%1:%2 (%3)")
                               .arg(status->name(), STR_ERROR)
                               .arg(-(cnt)));
                }
            }
        }
        RELEASE_LIST_DBMODEL(personStatus);
        return str.join("\n");
    });
    mExportCallbacks.insert(KItemTotalArea, [](const FileExporter* fileexporter,
                                               const QString& datatype){
        UNUSED(fileexporter);
        UNUSED(datatype);
        return QString::number(AREACTL->getTotalActiveItems());
    });
    mExportCallbacks.insert(KItemTongPhuTrach, [](const FileExporter* fileexporter,
                                                  const QString& datatype){
        UNUSED(fileexporter);
        UNUSED(datatype);
        return "";
    });
    mExportCallbacks.insert(KItemTongPhuTrachNhiemKy, [](const FileExporter* fileexporter,
                                                         const QString& datatype){
        UNUSED(fileexporter);
        UNUSED(datatype);
        return "";
    });

    traceout;
}
ErrCode Statistic::exportGeneralStatistic(QString *fpath, ExportType type)
{
    tracein;
    ErrCode ret = ErrNone;
    logi("Export general statistic, type %d", type);
    ret = getExportFileName(type,
                            "statistic",
                            fpath);
    logd("fpath '%s'", fpath?STR2CHA(*fpath):"(unknown)");

    if (ret == ErrNone) {
        ret = ExportFactory::exportTo(this,
                                      EXPORT_GENERAL_STAT,
                                      *fpath,
                                      type);
    }
    logife(ret, "export general static failed");
    traceret(ret);
    return ret;
}

ErrCode Statistic::exportTemplatePath(FileExporter *exporter, const QString &name,
                                      QString &fpath, QString *ftype) const
{
    tracein;
    ErrCode err = ErrNone;
    UNUSED(name);
    UNUSED(fpath);
    UNUSED(ftype);
    if (exporter) {
        ExportType type = exporter->getExportType();
        dbg(LOG_VERBOSE,"get export tmplate for type %d", type);
        switch (type) {
        case EXPORT_HTML:
            fpath = FileCtl::getPrebuiltDataFilePath(KPrebuiltGeneralStatExportTemplateName);
            break;
        default:
            loge("export type %d not support", type);
            err = ErrNotSupport;
        };
    } else {
        err = ErrInvalidArg;
        loge("invalid exporter");
    }
    dbg(LOG_VERBOSE, "export template path '%s'", STR2CHA(fpath));
    traceret(err);
    return err;
}

ErrCode Statistic::getExportDataString(const QString &item,
                                       const FileExporter* fileexporter,
                                       const QString& datatype, QString *data) const
{
    ErrCode ret = ErrNone;
    tracein;
    dbg(LOG_DEBUG, "export keyword '%s'", STR2CHA(item));
    if (mExportCallbacks.contains(item)) {
        StatExportCallbackFunc func = mExportCallbacks.value(item);
        if (func != nullptr) *data = func(fileexporter, datatype);
    } else {
        dbg(LOG_INFO, "export keyword '%s' not found", STR2CHA(item));
    }
    // TODO: raise exception when error occur???

    return ret;
}

quint64 Statistic::getExportTypeList()
{
    return ExportType::EXPORT_HTML;
}

const QStringList Statistic::getListExportKeyWord() const
{
    tracein;
    return mExportCallbacks.keys();
}
