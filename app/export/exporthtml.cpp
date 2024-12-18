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
 * Filename: exporthtml.cpp
 * Author: Anh, Ngo Huy
 * Created date:8/20/2022
 * Brief:
 */
#include "exporthtml.h"
#include "logger.h"
#include "defs.h"
#include "utils.h"
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include "filectl.h"

GET_INSTANCE_IMPL(ExportHtml)

ExportHtml::ExportHtml()
{
    tracein;
}

ErrCode ExportHtml::saveTo(const DataExporter *item, const QString& datatype, const QString &fpath)
{
    tracein;
    dbgtrace;
    ErrCode ret = ErrNone;
    QString ftype;
    QString templatePath;
    QString templateData;
    ret = item->exportTemplatePath(this, datatype, templatePath, &ftype);
    QStringList keywords = item->getListExportKeyWord();

    dbgd("templatePath %s", STR2CHA(templatePath));

    if (ret == ErrNone && templatePath.isEmpty()) {
        ret = ErrInvalidData;
        loge("Export HTML failed, not template found");
    }

    if (ret == ErrNone) {
        templateData = Utils::readAll(templatePath);
    }
    logd("number of keyword: %d", (int)keywords.length());
    logd("templateData len %d", (int)templateData.length());

    if (ret == ErrNone && templateData.isEmpty()) {
        ret = ErrNoData;
        loge("Export HTML failed, no template data");
    }

    if (ret == ErrNone) {
        QRegularExpression re("\\{\\{([^\\}\\}\\{\\{]*)\\}\\}");
        QRegularExpressionMatchIterator i = re.globalMatch(templateData);
        QHash<QString, bool> keywordStatus;

        // TODO: something a stupid searching and replacement, but it works.
        // TODO: find another way to improve it.
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString word = match.captured(1);
            logd("word: %s", word.toStdString().c_str());
            if (keywords.contains(word)){
                logd("keywords list has this one");
                keywordStatus.insert(word, true);
            } else {
                keywordStatus.insert(word, false);
                logd("keywords list DO NOT has this one");
            }
        }
        QString finadata = templateData;
        foreach(QString word, keywordStatus.keys()){
            QString data;

            if (keywordStatus.value(word)){
                ret = item->getExportDataString(word, this, datatype, &data);
            } else {
                data = "";
            }
            finadata = finadata.replace(QRegularExpression(QString("{{%1}}").arg(word)), data);
        }

        dbgv("Write %lld finadata to file %s", finadata.length(), STR2CHA(fpath));
        ret = FileCtl::writeStringToFile(finadata, fpath);

    }
    traceret(ret);
    return ret;
}

ExportType ExportHtml::getExportType() const
{
    return EXPORT_HTML;
}


