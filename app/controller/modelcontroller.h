#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include <QObject>
#include <QString>
#include <QList>
#include "errcode.h"

#include "importfactory.h"
#include "idataimporter.h"
#include "dataexporter.h"
#include "defs.h"
#include "dbmodel.h"
#include "controller.h"
#include "dbmodelhandler.h"

class DbModel;
class DbModelHandler;

class OnModelControllerListener {
public:
    virtual QString getName() = 0;
    virtual void onModelControllerDataUpdated() = 0;
};

class ModelController: public QObject, public Controller,
                        public DataExporter, public IDataImporter,
                        public onDbModelHandlerListener
{
    Q_OBJECT
public:
    ModelController();
    virtual ~ModelController();
    /**
     * @brief constructor
     * @param name controller name, should be same as model handler name??
     */
    ModelController(const QString& name);
    ModelController(const ModelController&) = delete; // not allow copy constructor
    virtual void init();

    virtual QString getName();

    virtual DbModelHandler* getModelHandler();

    void addListener(OnModelControllerListener* listener);
    void delListener(OnModelControllerListener* listener);

    /**
     * @brief Get list of item all items from db, must be implemented by derived class
     *        Caller must free resource after usage
     * @param status Status of db record, can multiple by bitwise
     * @param from from which index, mainly used for paging
     * @param noItems the number of items to be returned, mainly used for paging. if it's 0, load all
     * @param total return the number of items found
     * @param ret return error code
     * @return List of items. Caller must free resource after usage
     */
    virtual QList<DbModel*> getAllItemsFromDb(qint64 status = DB_RECORD_ACTIVE,
                                               int from = 0, int noItems = 0,
                                               int* total = nullptr,
                                               ErrCode* ret = nullptr
                                               );

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


    /**
     * @brief Search model from keyword. Search anything, only search active model
     * @param[in] keyword
     * @param[out] outList output list
     * @param[in] from from item idx
     * @param[in] noItems the number of items needs
     * @param[out] total Total found items
     * @return ErrNone if succeed
     */
    virtual ErrCode search(const QString& keyword, QList<DbModel*>* outList = nullptr,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

    /**
     * @brief Filter items, only filter active model
     * @param[in] fieldId Field id to be filter, \ref FilterField
     * @param[in] opFlags Operators, \ref FilterOperation
     * @param[in] keyword
     * @param[in] targetModelName name of target model of output list
     * @param[in] outList output list
     * @param[in] from from item idx
     * @param[in] noItems the number of items needs
     * @param[out] total Total found items
     * @return ErrNone if succeed
     */
    virtual ErrCode filter(int fieldId,
                       qint64 opFlags,
                       const QString& keywords,
                       const char* targetModelName = nullptr,
                       QList<DbModel*>* outList = nullptr,
                       int from = 0,
                       int noItems = 0,
                       int* total = nullptr);

    /**
     * @brief Get list of uid & name from name
     * @param[in] name name to be searched, multiple delimited by "," (\ref NAME_SPLIT)
     * @param[out] uidList Output list, in hash map of uid & name
     * @param[in] hdlName handler name used to search, get default one if it's null
     * @return error code
     */
    virtual ErrCode getUidListFromName(const QString& name, QHash<QString, QString>* uidList = nullptr,
                                       const char* hdlName = nullptr);
    /**
     * @brief Get name from uid list, name is separted by "," (\ref NAME_SPLIT)
     * @param uidList List of uid to be searched
     * @param hdlName handler name, if null default is used
     * @return Name in string
     */
    virtual QString getNameFromUidList(const QStringList& uidList, const char* hdlName = nullptr);
    virtual QString getNameFromUidList(const QString& uidList, const char* hdlName = nullptr);


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

    /* Derived class should implement below functions
     * virtual const char* getPrebuiltFileName();
     * virtual const char* getPrebuiltFileType();
     * virtual QList<DbModel*> getAllItemsFromDb(qint64 status = DB_RECORD_ACTIVE, int from, int noItems, int* total);
     */

    /**
     * @brief Import data from file
     * @param[in] importName import name, usually it's model handle name, i.e Person, Area
     * @param[in] type import type, i.e. xlsx, csv, etc. (\ref ImportType)
     * @param[in] fpath Path to file to be imported
     * @param[out] outList outout list
     * @return ErrCode
     */
    virtual ErrCode importFromFile(const QString& importName, ImportType type,
                                   const QString& fpath, QList<DbModel*>*outList = nullptr);

    /**
     * @brief Export model to file
     * @param model model to be exported
     * @param type Export type, i.e. html, etc.
     * @param fpath Output path, if null, it'll be saved into tmp dir
     * @return err code
     */
    virtual ErrCode exportToFile(DbModel* model, ExportType type, QString* fpath = nullptr) ;
    virtual ErrCode exportToFile(const QList<DbModel*>* listModel, ExportType type, QString* fpath = nullptr);
    virtual void reloadDb();
protected:
    /**
     * @brief Return list of supported export file type, i.e. xlsx, csv
     * @return supported export file type in bitwise
     */
    virtual quint64 getExportTypeList();// overide from DataExporter

    /**
     * @brief get prebuilt template name
     * @return prebuilt template name, empty if not support
     */
    virtual const QString exportListPrebuiltTemplateName() const;
    virtual const QString exportHtmlPrebuiltTemplateName() const;
    /**
     * @brief path to template file for export
     * @param exporter
     * @return
     */
    virtual const QString exportTemplatePath(FileExporter* exporter, QString* ftype = nullptr) const;

    /**
     * @brief get path to export file
     * @param[in] type export type, i.e CSV, xlsx
     * @param[in] fnameNoExt fname without extention
     * @param[in/out] fpath output path. if null, tmpdir is used
     * @return
     */
    virtual ErrCode getExportFileName(ExportType type, QString fnameNoExt, QString* fpath);

    /**
     * @brief Get data to be exported
     * @param[in] item keyword/data/item to be exported
     * @param[in] data Model
     * @param[in] exportData data to be exported
     * @return
     */
    virtual ErrCode getExportDataString(const QString& item, const DbModel* data, QString* exportData) const;

    virtual ErrCode onImportDataItem(const QString& importName, int importFileType,
                                 const QStringList& items, quint32 idx = 0,
                                 QList<DbModel *>* outList = nullptr);
    virtual ErrCode onImportDataItem(const QString& importName, int importFileType,
                                 const QHash<QString, QString>& items, quint32 idx = 0,
                                 QList<DbModel *>* outList = nullptr);
    // in list, with header
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType,
                                     const QStringList& items, quint32 idx);
    // hash map, in key:value style
    virtual DbModel* doImportOneItem(const QString& importName, int importFileType,
                                     const QHash<QString, QString>& items, quint32 idx);

    /**
     * @brief return function to allocate suitable db model
     * @return
     */
    virtual DbModelBuilder getMainBuilder() = 0;

    /**
     * @brief allocate model using main builder
     * @return
     */
    virtual DbModel* allocMainModel();

    void clearCacheItemList();

    /**
     * @brief Prebuilt file type, i.e. csv, etc.
     * @return
     */
    virtual const char* getPrebuiltFileType();

    /**
     * @brief insert model to cache
     * @param model model to be added
     * @param clone should clone model or not
     * @return err code
     */
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

    /**
     * @brief Check prebuilt file if it's already loaded, then update db
     * @param fname prebuilt file name
     * @param ftype prebuilt file type, i.e. csv, etc.
     * @return
     */
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
    /**
     * @brief Called when db model handle update data of dbmodel \ref onDbModelHandlerListener
     * @param model
     * @param type type of change, i.e. update, delete, add \ref DbModelChangeType
     * @param err
     */
    virtual void onDbModelHandlerDataUpdate(DbModel* model, int type, ErrCode err);
private:
    static ErrCode onCsvParseOneItemCallback(const QStringList& items, void* caller, void* param, quint32 idx);
signals:
    void dataUpdate();
protected slots:
    void onModelControllerDataUpdated();
protected:
    QString mName;
    bool mEnableCache;
    // cached item list, must clone data from cache, not get from it directly!!!!
    QHash<QString, DbModel*> mCacheItemList;
    bool mReloadDb;
    QList<OnModelControllerListener*> mListeners;
public:
    virtual ErrCode onLoad();
};

#endif // MODELCONTROLLER_H
