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
 * Filename: personctl.cpp
 * Author: Anh, Ngo Huy
 * Created date:7/16/2022
 * Brief:
 */
#include "person.h"
#include "std.h"
#include "dbctl.h"

Person::Person()
{

}


Person::~Person()
{
    traced;
}

Person* Person::build(){
    return new Person();
}

const QStringList &Person::hollyName() const
{
    return mHollyName;
}

QString Person::hollyNameString() const
{
    QString hollyName;
    foreach (QString name, mHollyName){
        if (!hollyName.isEmpty())
            hollyName += " ";
        hollyName += name;
    }

    // TODO: safe? this is stack or pointer????
    return hollyName;
}

void Person::addHollyName(const QString &newHollyName)
{
    logd("Add holy name %s", newHollyName.toStdString().c_str());
    mHollyName.append(newHollyName);
}

ErrCode_t Person::save()
{
    return DbCtl::getDb()->addPerson(this);
}




void Person::dump()
{
    traced;
    QString log = QStringLiteral(
                      "first name '%1'\n"
                      "last name '%2'"
                      )
                      .arg(firstName(), lastName());
    logd("%s", log.toStdString().c_str());
}

PersonBasic *Person::dad() const
{
    return mDad;
}

void Person::setDad(PersonBasic *newDad)
{
    mDad = newDad;
}

qint64 Person::enlistmentDate() const
{
    return mEnlistmentDate;
}

void Person::setEnlistmentDate(qint64 newEnlistmentDate)
{
    mEnlistmentDate = newEnlistmentDate;
}

const QString &Person::family() const
{
    return mFamily;
}

void Person::setFamily(const QString &newFamily)
{
    mFamily = newFamily;
}
