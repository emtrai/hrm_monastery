#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QList>
#include "errcode.h"

#include "exportfactory.h"
#include "importfactory.h"
#include "iimporter.h"
#include "iexporter.h"
#include "defs.h"
#include "dbmodel.h"

class DbModel;
class DbModelHandler;

class Controller: public QObject, public IImporter, public IExporter
{
    Q_OBJECT

public:
    Controller();
    Controller(const QString& name);
    virtual const QString& getName();

    virtual DbModelHandler* getModelHandler();
    virtual ErrCode addNew(DbModel* model);

    /**
     * @brief Get list of item all items from db, must be implemented by derived class
     *        Caller must free resource after usage
     * @return List of items. Caller must free resource after usage
     */
    virtual QList<DbModel*> getAllItemsFromDb(qint64 status = DB_RECORD_ACTIVE, int from = 0, int noItems = 0, int* total = nullptr);


    virtual ErrCode reloadDb();

    virtual ErrCode getExportFileName(ExportType type, QString fnameNoExt, QString* fpath);
    virtual ErrCode exportToFile(DbModel* model, ExportType type, QString* fpath);
    virtual ErrCode exportToFile(const QList<DbModel*>* listModel, ExportType type, QString* fpath);

    virtual ErrCode importFromFile(IImporter *importer, ImportType type, const QString& fpath, QList<DbModel*>*outList = nullptr);
    virtual ErrCode onImportItem(int importFileType, const QStringList& items, quint32 idx = 0, void* tag = nullptr);
    virtual ErrCode onImportItem(int importFileType, const QHash<QString, QString>& items, quint32 idx = 0, void* tag = nullptr);
    // in list, with header
    virtual DbModel* doImportOneItem(int importFileType, const QStringList& items, quint32 idx);
    // hash map, in key:value style
    virtual DbModel* doImportOneItem(int importFileType, const QHash<QString, QString>& items, quint32 idx);

    virtual int search(const QString& keyword, QList<DbModel*>* outList = nullptr);
    virtual DbModel* getModelByName(const QString& name);
    virtual DbModel* getModelByUid(const QString& uid);
    virtual DbModel* getModelByNameId(const QString& nameId);
    virtual int filter(int catetoryid,
                       const QString& catetory,
                       qint64 opFlags,
                       const QString& keywords,
                       QList<DbModel*>* outList = nullptr);

    virtual ErrCode getUidListFromName(const QString& name, QHash<QString, QString>* uidList = nullptr, const char* hdlName = nullptr);
    virtual QString getNameFromUidList(const QStringList& uidList, const char* hdlName = nullptr);
    virtual QString getNameFromUidList(const QString& uidList, const char* hdlName = nullptr);

    virtual ErrCode markModelDelete(DbModel* model);
    virtual ErrCode deleteModel(DbModel* model);


    virtual const QString exportTemplatePath(Exporter* exporter) const;
    virtual quint64 getExportTypeList(); // overide from IExporter
protected:
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
    virtual const char* getPrebuiltFileType();

    virtual ErrCode check2UpdateDbFromPrebuiltFile(const QString& fname,
                                                   const QString& ftype);
    virtual ErrCode check2UpdateDbFromPrebuiltFile();

    virtual ErrCode doOneCSVItemCallback(const QStringList& items, void* param);

    virtual ErrCode parsePrebuiltFile(const QString &fpath, const QString &ftype);
private:
    static ErrCode oneCSVItemCallback(const QStringList& items, void* caller, void* param, quint32 idx);

protected:
    QString mName;

    // event
signals:
   void load();
public slots:
    virtual void onLoad();
};

#endif // CONTROLLER_H
