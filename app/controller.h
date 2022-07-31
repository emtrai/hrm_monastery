#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
class Controller: public QObject
{
    Q_OBJECT

public:
    Controller();
    Controller(const QString& name);
protected:
    QString mName;

    // event
signals:
   void load();
public slots:
    virtual void onLoad();
};

#endif // CONTROLLER_H
