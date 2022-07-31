QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

CONFIG += console
QT += sql
QT += widgets

DEFINES += FEATURE_system_sqlite=OFF

DEFINES += TEST_ENABLE
# DEFINES += LOG_LEVEL=4
DEFINES = DEBUG_TRACE

INCLUDEPATH += $$PWD/db
INCLUDEPATH += $$PWD/db/sqlite
INCLUDEPATH += $$PWD/model
INCLUDEPATH += $$PWD/person
INCLUDEPATH += $$PWD/community
INCLUDEPATH += $$PWD/view
INCLUDEPATH += $$PWD/view/dialog
INCLUDEPATH += $$PWD/config
INCLUDEPATH += $$PWD/file
INCLUDEPATH += $$PWD/saint
INCLUDEPATH += $$PWD/crypto
#QT_NO_DEBUG_OUTPUT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    address.cpp \
    cache/cachectl.cpp \
    community/communityctl.cpp \
    config/config.cpp \
    controller.cpp \
    crypto/crypto.cpp \
    db/dbctl.cpp \
    db/dbinfo.cpp \
    db/idatabase.cpp \
    db/sqlite/dbsqlite.cpp \
    db/sqlite/dbsqlitecommunity.cpp \
    db/sqlite/dbsqlitedefs.cpp \
    db/sqlite/dbsqliteinsertbuilder.cpp \
    db/sqlite/dbsqlitesaint.cpp \
    db/sqlite/table/dbsqlitecommunitytbl.cpp \
    db/sqlite/table/dbsqlitedeparttbl.cpp \
    db/sqlite/table/dbsqliteedutbl.cpp \
    db/sqlite/table/dbsqlitepersoneventtbl.cpp \
    db/sqlite/table/dbsqlitepersontbl.cpp \
    db/sqlite/dbsqlitetablebuilder.cpp \
    db/sqlite/table/dbsqlitesainttbl.cpp \
    db/sqlite/table/dbsqlitetbl.cpp \
    db/sqlite/table/dbsqliteworktbl.cpp \
    file/filectl.cpp \
    loader/loaderctl.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    model/church.cpp \
    model/community.cpp \
    model/department.cpp \
    model/education.cpp \
    model/person.cpp \
    model/personbasic.cpp \
    model/personevent.cpp \
    model/saint.cpp \
    model/work.cpp \
    monastery/monasteryctl.cpp \
    person/personctl.cpp \
    report/reportctl.cpp \
    saint/saintctl.cpp \
    search/searchctl.cpp \
    statistic/statistic.cpp \
    test/testctl.cpp \
    utils.cpp \
    view/dialog/dlgcommunity.cpp \
    view/dialog/dlgperson.cpp

HEADERS += \
    address.h \
    cache/cachectl.h \
    community/communityctl.h \
    config/config.h \
    controller.h \
    crypto/crypto.h \
    db/dbctl.h \
    db/dbinfo.h \
    db/idatabase.h \
    db/idbcommunity.h \
    db/idbsaint.h \
    db/sqlite/dbsqlite.h \
    db/sqlite/dbsqlitecommunity.h \
    db/sqlite/dbsqlitedefs.h \
    db/sqlite/dbsqliteinsertbuilder.h \
    db/sqlite/dbsqlitesaint.h \
    db/sqlite/table/dbsqlitecommunitytbl.h \
    db/sqlite/table/dbsqlitedeparttbl.h \
    db/sqlite/table/dbsqliteedutbl.h \
    db/sqlite/table/dbsqlitepersoneventtbl.h \
    db/sqlite/table/dbsqlitepersontbl.h \
    db/sqlite/dbsqlitetablebuilder.h \
    db/sqlite/table/dbsqlitesainttbl.h \
    db/sqlite/table/dbsqlitetbl.h \
    db/sqlite/table/dbsqliteworktbl.h \
    defs.h \
    errcode.h \
    file/filectl.h \
    loader/loaderctl.h \
    logger.h \
    mainwindow.h \
    model/church.h \
    model/community.h \
    model/department.h \
    model/education.h \
    model/person.h \
    model/personbasic.h \
    model/personevent.h \
    model/saint.h \
    model/work.h \
    monastery/monasteryctl.h \
    person/personctl.h \
    report/reportctl.h \
    saint/saintctl.h \
    search/searchctl.h \
    statistic/statistic.h \
    std.h \
    test/testctl.h \
    utils.h \
    view/dialog/dlgcommunity.h \
    view/dialog/dlgperson.h

FORMS += \
    mainwindow.ui \
    view/dialog/dlgcommunity.ui \
    view/dialog/dlgperson.ui

TRANSLATIONS += \
    hrm_monastery_vi_VN.ts
CONFIG += lrelease
CONFIG += embed_translations

appFiles.files = \
    res
appFiles.path = /data/user/qt/$$TARGET
INSTALLS += appFiles

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    license.template \
    res/Saints.csv

RESOURCES += \
    resource.qrc
