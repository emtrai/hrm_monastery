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
#include "personctl.h"
#include "dbctl.h"
#include "logger.h"
#include "errcode.h"
#include "logger.h"
#include "utils.h"
#include "person.h"
#include "personevent.h"
#include "filectl.h"
#include "specialistctl.h"
#include "event.h"
#include "dbpersonmodelhandler.h"
#include "filter.h"
#include "dbspecialistmodelhandler.h"
#include "exportfactory.h"

#include "communityctl.h"
#include "eductl.h""
#include "coursectl.h"
#include "workctl.h"

GET_INSTANCE_CONTROLLER_IMPL(PersonCtl)

ErrCode PersonCtl::getListPersonInCommunity(const QString &communityUid, QList<DbModel *>& list, qint32 status)
{
    traced;
    ErrCode err = ErrNone;
    if (!communityUid.isEmpty()) {
        logi("get person in community uid '%s'", STR2CHA(communityUid));
        err = personModelHdl()->getListPersonInCommunity(communityUid, status, list);
        logd("found %lld item", list.size());
    } else {
        loge("Invalid community Uid");
    }
    tracedr(err);
    return err;
}

ErrCode PersonCtl::getListEvents(const QString &personUid, QList<DbModel*>& list)
{
    traced;
    ErrCode err = ErrNone;
    if (!personUid.isEmpty()) {
        logd("get person event of uid '%s'", STR2CHA(personUid));
        err = personModelHdl()->getListEvents(personUid, list);
        logd("found %lld item", list.size());
        if (err != ErrNone) {
            loge("get List person event failed, err=%d", err);
        }
    } else {
        loge("Invalid person Uid");
        err = ErrInvalidArg;
    }
    tracede;
    return err;
}


DbModel *PersonCtl::doImportOneItem(const QString& importName, int importFileType, const QStringList &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    Person* person = nullptr;
    int i = 0;
    logd("idx = %d", idx);
    logd("no items %lld", items.count());
    if (idx == 0) {
        logd("HEADER, save it");
        foreach (QString item, items) {
            logd("Header %s", item.toStdString().c_str());
            mImportFields.append(item.trimmed());
        }
    } else {

        person = (Person*)Person::build();
        if (person) {
            foreach (QString item, items) {
                QString field = mImportFields[i++];
                logd("Import field %s", field.toStdString().c_str());
                ret = person->onImportDataItem(importName, importFileType, field, item, idx);
            }
        } else {
            ret = ErrNoMemory;
            loge("Faild to allocate person, no memory");
        }
    }
    if (ret != ErrNone && person) {
        delete person;
    }

    tracedr(ret);
    return person;
}

DbModel *PersonCtl::doImportOneItem(const QString& importName, int importFileType, const QHash<QString, QString> &items, quint32 idx)
{
    ErrCode ret = ErrNone;
    Person* person = nullptr;
    logd("idx = %d", idx);
    person = (Person*)Person::build();
    if (person) {
        foreach (QString field, items.keys()) {
            QString value = items.value(field);
            logd("Import field %s", field.toStdString().c_str());
            logd("Import value %s", value.toStdString().c_str());
            ret = person->onImportDataItem(importName, importFileType, field, value, idx);
        }
    } else {
        ret = ErrNoMemory;
        loge("Faild to allocate person, no memory");
    }

    if (ret != ErrNone && person) {
        delete person;
    }
    tracedr(ret);
    return person;
}

ErrCode PersonCtl::filter(int catetoryid, qint64 opFlags,
                      const QString &keywords,
                      const char* targetModelName,
                      QList<DbModel *> *outList,
                      int from,
                      int noItems,
                      int* total)
{
    traced;
    ErrCode err = ErrNone;
    logd("category id %d", catetoryid);
    logd("opFlags id %lld", opFlags);
    logd("keywords %s", STR2CHA(keywords));
    logd("targetModelName %s", targetModelName?targetModelName:"(null)");
    ModelController* controller = nullptr;
    DbModelHandler* modelHdl = nullptr;
    bool directFilter = false;

    if (!outList) {
        err = ErrInvalidArg;
        loge("Invalid argument");
    }

    /* There are 2 filter cases:
     * - Filter directly with fields of model, i.e. name, etc.
     * - Filter via other fields like community, specialist
     */
    if (err == ErrNone) {
        switch (catetoryid) {
        case FILTER_FIELD_SPECIALIST:
            controller = SPECIALISTCTL;
            modelHdl = DB->getModelHandler(KModelHdlSpecialist);
            break;
        case FILTER_FIELD_COMMUNITY:
            controller = COMMUNITYCTL;
            modelHdl = DB->getModelHandler(KModelHdlCommunity);
            break;
        case FILTER_FIELD_EDUCATION:
            controller = EDUCTL;
            modelHdl = DB->getModelHandler(KModelHdlEdu);
            break;
        case FILTER_FIELD_COURSE:
            controller = COURSECTL;
            modelHdl = DB->getModelHandler(KModelHdlCourse);
            break;
        case FILTER_FIELD_WORK:
            controller = WORKCTL;
            modelHdl = DB->getModelHandler(KModelHdlWork);
            break;
        case FILTER_FIELD_NAME:
        case FILTER_FIELD_FULL_NAME:
        case FILTER_FIELD_ADDRESS:
        case FILTER_FIELD_BIRTHDAY:
            directFilter = true;
            err = ModelController::filter(catetoryid, opFlags, keywords, targetModelName, outList, from, noItems, total);
            break;
        default:
            err = ErrNotSupport;
            loge("filter id %d not support", catetoryid);
            break;
        }
    }
    logd("directFilter=%d", directFilter);
    if (!directFilter) {
        QList<DbModel *> models;
        if (err == ErrNone) {
            logd("Get list of target category, via name");
            err = controller->filter(FILTER_FIELD_NAME, FILTER_OP_CONTAIN, keywords,
                                     nullptr, /* use main/default target model name*/
                                     &models, from, noItems, total);
            logd("Search ret=%d, found=%lld models", err, models.size());
        }
        if (err == ErrNone) {
            logd("search model for each models");
            foreach (DbModel* item, models) {
                if (item) {
                    logd("uid %s, name %s",
                         item->uid().toStdString().c_str(),
                         item->name().toStdString().c_str());
                    QList<DbModel *> perList = modelHdl->getAll(KModelHdlPerson);
                    if (perList.count() > 0) {
                        logd("found %lld person", perList.count());
                        outList->append(perList);
                    } else {
                        logd("not found any person");
                    }
                    delete item;
                } else {
                    // just debug log for check, ignore error
                    logd("Error: something wrong, empty model in list");
                }
            }
        }
    }
    if (err != ErrNone && outList) {
        loge("Error err=%d, cleanup model resources", err);
        DbModelHandler::cleanUpModelList(*outList);
    }

//    if (catetoryid == FILTER_FIELD_SPECIALIST) {
//        logd("filder by specialist");
//        QList<DbModel *> specialistList;
//        ret = SPECIALISTCTL->filter(catetoryid, opFlags, keywords,
//                                    &specialistList, from, noItems, total);
//        logd("Search ret=%d, found=%ld specialist", ret, specialistList.size());
//        DbSpecialistModelHandler* specialistHdl = dynamic_cast<DbSpecialistModelHandler*>(DB->getModelHandler(KModelHdlSpecialist));
//        if (ret == ErrNone){
//            logd("search person for each specialist");
//            foreach (DbModel* item, specialistList) {

//                logd("specialist uid %s, name %s",
//                     item->uid().toStdString().c_str(),
//                     item->name().toStdString().c_str());
//                QList<DbModel *> perList = specialistHdl->getListPerson(item->uid());
//                if (perList.count() > 0) {
//                    logd("found %d person", perList.count());
//                    outList->append(perList);
//                } else {
//                    logd("not found any person");
//                }
//            }
//        } else {
//            logi("No specialist found");
//        }
//    } else {
//        logd("generic filter");
//        ret = ModelController::filter(catetoryid, opFlags, keywords, outList,
//                                      from, noItems, total);
//    }
    tracedr(err);
    return err;
}

const QString PersonCtl::exportTemplatePath(FileExporter *exporter, QString* ftype) const
{
    QString ret;
    traced;
    if (exporter) {
        logd("exporter type %d", exporter->getExportType());
        switch (exporter->getExportType()) {
        case EXPORT_CSV_LIST:
        case EXPORT_XLSX:
            ret = FileCtl::getPrebuiltDataFilePath(KPrebuiltPersonListJSONTemplateFileName);
            if (ftype) *ftype = KFileTypeJson;
            break;
        default:
            loge("unsupported export type %d", exporter->getExportType());
            break;
        }
    } else {
        loge("empty exporter");
    }
    return ret;
}

ErrCode PersonCtl::exportListPersonInCommunity(const QString &communityUid, ExportType exportType, const QString &fpath)
{
    traced;
    ErrCode err = ErrNone;
    QList<DbModel*> items;
    if (communityUid.isEmpty() || fpath.isEmpty()) {
        err = ErrInvalidArg;
        loge("invalid argument");
    }

    if (err == ErrNone) {
        logd("Community uid '%s'", STR2CHA(communityUid));
        err = getListPersonInCommunity(communityUid, items);
    }

    if (err == ErrNone) {
        if (items.length() > 0) {
            logi("found %ld person in community uid '%s', start export to '%s'",
                 items.length(), STR2CHA(communityUid), STR2CHA(fpath));
            err = ExportFactory::exportTo(this, items, fpath, exportType);
        } else {
            err = ErrNoData;
            loge("nothing to export, no person in specified community uid '%s'", STR2CHA(communityUid));
        }
    }
    RELEASE_LIST_DBMODEL(items);
    tracedr(err);
    return err;
}

quint64 PersonCtl::getExportTypeList()
{
    return ExportType::EXPORT_CSV_LIST | ExportType::EXPORT_XLSX;
}

PersonCtl::PersonCtl():
    ModelController(KModelHdlPerson)
{
    mEnableCache = false;
}


PersonCtl::~PersonCtl()
{
    traced;
}

DbPersonModelHandler *PersonCtl::personModelHdl()
{
    return dynamic_cast<DbPersonModelHandler*>(DB->getModelHandler(KModelHdlPerson));
}


DbModelBuilder PersonCtl::getMainBuilder()
{
    return &Person::build;
}
