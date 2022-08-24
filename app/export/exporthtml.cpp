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

GET_INSTANCE_IMPL(gInstance, ExportHtml)

ExportHtml::ExportHtml()
{
    traced;
}

ErrCode ExportHtml::saveTo(const IExporter *item, const QString &fpath)
{
    traced;
    ErrCode ret = ErrNone;
    const QString templatePath = item->exportTemplatePath();
    QStringList keywords = item->getListKeyWord();
    QString templateData;


    logd("templatePath %s", templatePath.toStdString().c_str());

    if (!templatePath.isEmpty()) {
        templateData = Utils::readAll(templatePath);
    }

    logd("number of keyword: %d", keywords.length());
    logd("templateData len %d", templateData.length());

    QRegularExpression re("{{(.*)}}");
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
        bool isFile = false;
        if (keywordStatus.value(word)){
            ret = item->getDataString(word, &data, &isFile);
        } else {
            data = "";
        }
        finadata = finadata.replace(QRegularExpression(QString("{{%1}}").arg(word)), data);
    }

    logd("Write %d finadata to file %s", finadata.length(), fpath.toStdString().c_str());
    ret = FileCtl::writeStringToFile(finadata, fpath);

    tracedr(ret);
    return ret;
}


