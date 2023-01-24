#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QList>
#include "errcode.h"

#include "exportfactory.h"
#include "importfactory.h"
#include "iimporter.h"

class DbModel;
class DbModelHandler;

class Controller: public QObject, public IImporter
{
    Q_OBJECT

public:
    Controller();
    Controller(const QString& name);
    virtual const QString& getName();

    virtual DbModelHandler* getModelHandler();
    virtual ErrCode addNew(DbModel* model);

    virtual ErrCode reloadDb();
    virtual ErrCode exportToFile(DbModel* model, ExportType type, QString* fpath);
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
    virtual int filter(int catetoryid,
                       const QString& catetory,
                       qint64 opFlags,
                       const QString& keywords,
                       QList<DbModel*>* outList = nullptr);

    virtual ErrCode loadFromDb();
    virtual ErrCode getUidListFromName(const QString& name, QHash<QString, QString>* uidList = nullptr, const char* hdlName = nullptr);
    virtual QString getNameFromUidList(const QStringList& uidList, const char* hdlName = nullptr);
    virtual QString getNameFromUidList(const QString& uidList, const char* hdlName = nullptr);

    virtual ErrCode markModelDelete(DbModel* model);
    virtual ErrCode deleteModel(DbModel* model);

protected:
    virtual DbModel *buildModel(void* items, const QString& fmt);

    virtual const char* getPrebuiltFileName();
    virtual const char* getPrebuiltFileType();

    virtual ErrCode check2UpdateDbFromPrebuiltFile(const QString& fname,
                                                   const QString& ftype);
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
