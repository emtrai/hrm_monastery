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
 * Filename: dbinfo.h
 * Author: Anh, Ngo Huy
 * Created date:7/20/2022
 * Brief:
 */
#ifndef DBINFO_H
#define DBINFO_H

#include <QString>
class DbInfo
{
public:
    DbInfo();
    const QString &uri() const;
    void setUri(const QString &newUri);

    const QString &username() const;
    void setUsername(const QString &newUsername);

    const QString &password() const;
    void setPassword(const QString &newPassword);

    int fromJson(const QString& jsonString);
    QString toJsonString();

    const QString &metaUri() const;
    void setMetaUri(const QString &newMetaUri);

private:
    QString mUri;
    QString mUsername;
    QString mPassword;
    QString mMetaUri;

};

#endif // DBINFO_H
