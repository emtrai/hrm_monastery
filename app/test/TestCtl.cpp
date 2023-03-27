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
 * Filename: TestCtl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/23/2022
 * Brief:
 */

#include "testctl.h"
#include "logger.h"
#include "errcode.h"

#include <QRandomGenerator>
#include <QDateTime>

#include "community.h"
#include "dbmodel.h"

QString TestCtl::GetRandomString(qint32 length)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    qint32 lengthOfPossiblechars = possibleCharacters.length();
    QString randomString;
    for(int i=0; i<length; ++i)
    {
        int index = QRandomGenerator::global()->bounded(lengthOfPossiblechars) % lengthOfPossiblechars;
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}


QString TestCtl::GetRandomName(qint32 length){
    QString name;
    for (int i = 0; i < length; i++){
        name += GetRandomString() + " ";

    }
    logd("random name '%s'", name.toStdString().c_str());
    return name.trimmed();
}


Person *TestCtl::genRandomPerson()
{
    tracein;
    Person* person = (Person*)Person::build();

    person->setName(GetRandomName());
    person->dump();

    return person;
}

Community* TestCtl::genRandomCommunity(){

    tracein;
    Community* commObj = (Community*)Community::build();

    commObj->setName(GetRandomName(2));
    commObj->setStatus(DbModelStatus::MODEL_ACTIVE);
    commObj->setCreateDate(QDateTime::currentMSecsSinceEpoch());
    commObj->dump();

    return commObj;

}
