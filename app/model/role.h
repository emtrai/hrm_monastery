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
 * Filename: role.h
 * Author: Anh, Ngo Huy
 * Created date:10/22/2022
 * Brief:
 */
#ifndef ROLE_H
#define ROLE_H

#include <dbmodel.h>

class Role : public DbModel
{
public:
    Role();
    static DbModel *builder();
    virtual QString modelName() const;

    const QString &remark() const;
    void setRemark(const QString &newRemark);

protected:
    virtual DbModelHandler *getDbModelHandler();
private:
    QString mRemark;
};

#endif // ROLE_H
