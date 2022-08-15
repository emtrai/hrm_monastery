#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include "errcode.h"

class DbModel;

class Controller: public QObject
{
    Q_OBJECT

public:
    Controller();
    Controller(const QString& name);

protected:
    virtual DbModel *buildModel(void* items, const QString& fmt);
    virtual ErrCode check2UpdateDbFromPrebuiltFile(const QString& fname,
                                                   const QString& ftype);
    virtual ErrCode doOneCSVItemCallback(const QStringList& items, void* param);

    virtual ErrCode parsePrebuiltFile(const QString &fpath, const QString &ftype);
private:
    static ErrCode oneCSVItemCallback(const QStringList& items, void* caller, void* param);

protected:
    QString mName;

    // event
signals:
   void load();
public slots:
    virtual void onLoad();
};

#endif // CONTROLLER_H
