#include "controller.h"
#include "logger.h"
#include "errcode.h"
Controller::Controller()
{

}

Controller::Controller(const QString& name):mName(name)
{

}

void Controller::onLoad(){
    traced;
    logd("Onload %s", mName.toStdString().c_str());
}
