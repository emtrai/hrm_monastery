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
 * Filename: crypto.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/31/2022
 * Brief:
 */
#include "crypto.h"
#include <QCryptographicHash>
#include "std.h"
#include <QFile>
#include <QByteArray>

QString Crypto::hashFile(const QString &filename, QCryptographicHash::Algorithm algo)
{
    traced;
    QString hashString;

    logd("hash file '%s' alg %d", filename.toStdString().c_str(), algo);
    if (!filename.isEmpty()){ // TODO: check valid enum
        QFile file(filename);
        if (file.exists())
        {
            logi("Hash file %s, algo %d",
                 file.fileName().toStdString().c_str(), algo);
            if (file.open(QFile::ReadOnly)) {
                QCryptographicHash hash(algo);
                // TODO read by blocks to reduce memory consumption
                if (hash.addData(&file)) {
                    hashString = (hash.result().toHex().toLower());
                }
                else {
                    loge("Failed to hash");
                }
            }
            else{
                loge("Open file failed");
            }
        }
        else{
            loge("File %s not exist", file.fileName().toStdString().c_str());
        }
    }
    else {
        loge("Invalid file name or algo %d", algo);
    }


    if (!hashString.isEmpty()){
        logi("Hash of file '%s'", hashString.toStdString().c_str());
    }
    // TODO: is it value copy?????
    return hashString;
}

QString Crypto::hashString(const QString &value, QCryptographicHash::Algorithm algo)
{
    traced;
    QString hashString;
    if (!value.isEmpty()){
        QCryptographicHash hash(algo);
        hash.addData(value.toUtf8());
        hashString = (hash.result().toHex().toLower());
        logd("Hash string '%s'", hashString.toStdString().c_str());
    }
    else{
        loge("hash string failed, empty string");
    }
    return hashString;
}

Crypto::Crypto()
{

}
