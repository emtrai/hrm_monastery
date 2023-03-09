#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include <QObject>
#include <QString>
#include <QList>
#include "errcode.h"

#include "exportfactory.h"
#include "importfactory.h"
#include "idataimporter.h"
#include "idataexporter.h"
#include "defs.h"
#include "dbmodel.h"
#include "controller.h"

class DbModel;
class DbModelHandler;

class ModelController: public QObject, public Controller, public IDataExporter, public IDataImporter
{
    Q_OBJECT

public:
    ModelController();
    virtual ~ModelController();
    ModelController(const QString& name);
    virtual QString getName();

    virtual DbModelHandler* getModelHandler();
    virtual ErrCode addNew(DbModel* model);

    /**
     * @brief Get list of item all items from db, must be implemented by derived class
     *        Caller must free resource after usage
     * @param reloadDb true to reload data from db
     * @param from from which index, mainly used for paging
     * @param noItems the number of items to be returned, mainly used for paging. if it's 0, load all
     * @param total return the number of items found
     * @return List of items. Caller must free resource after usage
     */
    virtual QList<DbModel*> getAllItemsFromDb(qint64 status = DB_RECORD_ACTIVE, int from = 0, int noItems = 0, int* total = nullptr);


    virtual int search(const QString& keyword, QList<DbModel*>* outList = nullptr);
    virtual DbModel* getModelByName(const QString& name);
    virtual int filter(int catetoryid,
                       const QString& catetory,
                       qint64 opFlags,
                       const QString& keywords,
                       QList<DbModel*>* outList = nullptr);

    virtual ErrCode getUidListFromName(const QString& name, QHash<QString, QString>* uidList = nullptr, const char* hdlName = nullptr);
    virtual QString getNameFromUidList(const QStringList& uidList, const char* hdlName = nullptr);
    virtual QString getNameFromUidList(const QString& uidList, const char* hdlName = nullptr);

    /**
     * @brief reload data from database
     *        Should be called to refresh data from db, used when data is cached in memory
     * @return
     */
    virtual ErrCode reloadDb();


    /**
     * @brief Get all data
     *        Caller must free data returned by this API after using
     * @param readFromDb true to reload data from db
     * @param from from which index, mainly used for paging
     * @param noItems the number of items to be returned, mainly used for paging. if it's 0, load all
     * @param total return the number of items found
     * @return List of data (dbmodel). Caller must free this list after use
     */
    virtual QList<DbModel*> getAllItems(bool readFromDb = false, int from = 0,
                                         int noItems = 0, int* total = nullptr);


    /* Derived class should implement below functions
     * virtual const char* getPrebuiltFileName();
     * virtual const char* getPrebuiltFileType();
     * virtual QList<DbModel*> getAllItemsFromDb(qint64 status = DB_RECORD_ACTIVE, int from, int noItems, int* total);
     */

    /**
     * @brief Return list of supported export file type, i.e. xlsx, csv
     * @return supported export file type in bitwise
     */
    virtual quint64 getExportTypeList();// overide from IDataExporter

    /**
     * @brief Get data/model via uid
     *        Caller must free after use
     * @param uid uid
     * @return Model, or nullptr if not found
     *         Caller must free after use
     */
    virtual DbModel* getModelByUid(const QString& uid);
    virtual DbModel* getModelByUidFromDb(const QString& uid);

    /**
     * @brief Get data/model via name id
     *        Caller must free after use
     * @param nameId name id
     * @return Model, or nullptr if not found
     *         Caller must free after use
     */
    virtual DbModel* getModelByNameId(const QString& nameId);
    virtual DbModel* getModelByNameIdFromDb(const QString& nameId);

    virtual ErrCode importFromFile(const QString& importName, ImportType type, const QString& fpath, QList<DbModel*>*outList = nullptr);

    virtual ErrCode exportToFile(DbModel* model, ExportType type, QString* fpath);
    virtual ErrCode exportToFile(const QList<DbModel*>* listModel, ExportType type, QString* fpath);
protected:
    virtual const QString exportTemplatePath(Exporter* exporter) const;

    virtual ErrCode getExportFileName(ExportType type, QString fnameNoExt, QString* fpath);

    virtual ErrCode onImportItem(const QString& importName, int importFileType, const QStringList& items, quint32 idx = 0, void* tag = nullptr);
    virtual ErrCode onImportItem(const QString& importName, int importFileType, const QHash<QString, QString>& items, quint32 idx = 0, void* tag = nullptr);
    // in list, with header
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QStringList& items, quint32 idx);
    // hash map, in key:value style
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType, const QHash<QString, QString>& items, quint32 idx);

    /**
     * @brief return function to allocate suitable db model
     * @return
     */
    virtual DbModelBuilder getMainBuilder() = 0;
    virtual DbModel* allocMainModel();

    virtual void clearCacheItemList();

    /**
     * @brief Prebuilt file type, i.e. csv, etc.
     * @return
     */
    virtual const char* getPrebuiltFileType();

    /**
     * @brief get list of export keyword
     * @param exporter exporter
     * @param outMap list of keyword and its name (keyword:name)
     * @return
     */
    virtual ErrCode getListExportKeywords(Exporter* exporter,
                                          QHash<QString, QString>& outMap) const;

    virtual ErrCode insertModelToCache(DbModel* model, bool clone = true);

    /**
     * @brief Build model from items/data
     * @param items data to be parsed
     * @param fmt data format, i.e. QStringList
     * @return
     */
    virtual DbModel *buildModel(void* items, const QString& fmt = KDataFormatStringList);

    /**
     * @brief get file name of prebuilt data, used to load prebuilt data and update to DB
     * @return
     */
    virtual const char* getPrebuiltFileName();

    virtual ErrCode check2UpdateDbFromPrebuiltFile(const QString& fname,
                                                   const QString& ftype);
    virtual ErrCode check2UpdateDbFromPrebuiltFile();

    virtual ErrCode doCsvParseOneItem(const QStringList& items, void* param);

    /**
     * @brief parse one item of prebuilt file
     *        Called when parse prebuilt file
     * @param jobj
     * @param ok true if parsed ok, false otherwise
     * @return
     */
    virtual DbModel* onJsonParseOneItem(const QJsonObject& jobj, bool* ok = nullptr);
    virtual ErrCode parsePrebuiltFile(const QString &fpath, const QString &ftype);
private:
    static ErrCode onCsvParseOneItemCallback(const QStringList& items, void* caller, void* param, quint32 idx);

protected:
    QString mName;
    bool mEnableCache;
    // cached item list, must clone data from cache, not get from it directly!!!!
    QHash<QString, DbModel*> mCacheItemList;
public:
    virtual ErrCode onLoad();
};

#endif // MODELCONTROLLER_H
