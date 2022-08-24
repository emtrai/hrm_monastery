QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

CONFIG += console
QT += sql
QT += widgets
QT += gui

DEFINES += FEATURE_system_sqlite=OFF

DEFINES += TEST_ENABLE
# DEFINES += LOG_LEVEL=4
DEFINES = DEBUG_TRACE

INCLUDEPATH += $$PWD/db
INCLUDEPATH += $$PWD/db/sqlite
INCLUDEPATH += $$PWD/model
INCLUDEPATH += $$PWD/view
INCLUDEPATH += $$PWD/view/dialog
INCLUDEPATH += $$PWD/config
INCLUDEPATH += $$PWD/file
INCLUDEPATH += $$PWD/controller
INCLUDEPATH += $$PWD/crypto
INCLUDEPATH += $$PWD/location
INCLUDEPATH += $$PWD/export
#QT_NO_DEBUG_OUTPUT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    address.cpp \
    cache/cachectl.cpp \
    controller/areactl.cpp \
    controller/communityctl.cpp \
    config/config.cpp \
    controller/controller.cpp \
    controller/countryctl.cpp \
    controller/departctl.cpp \
    controller/ethnicctl.cpp \
    controller/missionctl.cpp \
    controller/provincectl.cpp \
    crypto/crypto.cpp \
    db/dbctl.cpp \
    db/dbinfo.cpp \
    db/dbmodel.cpp \
    db/dbmodelhandler.cpp \
    db/idatabase.cpp \
    db/sqlite/dbsqlite.cpp \
    db/sqlite/dbsqlitearea.cpp \
    db/sqlite/dbsqlitecommunity.cpp \
    db/sqlite/dbsqlitecountry.cpp \
    db/sqlite/dbsqlitedefs.cpp \
    db/sqlite/dbsqlitedept.cpp \
    db/sqlite/dbsqliteedu.cpp \
    db/sqlite/dbsqliteethnic.cpp \
    db/sqlite/dbsqliteinsertbuilder.cpp \
    db/sqlite/dbsqlitemission.cpp \
    db/sqlite/dbsqlitemodelhandler.cpp \
    db/sqlite/dbsqliteprovince.cpp \
    db/sqlite/dbsqlitesaint.cpp \
    db/sqlite/dbsqlitespecialist.cpp \
    db/sqlite/table/dbsqliteareatbl.cpp \
    db/sqlite/table/dbsqlitecommunitytbl.cpp \
    db/sqlite/table/dbsqlitecountrytbl.cpp \
    db/sqlite/table/dbsqlitedeparttbl.cpp \
    db/sqlite/table/dbsqliteedutbl.cpp \
    db/sqlite/table/dbsqliteethnictbl.cpp \
    db/sqlite/table/dbsqlitemissiontbl.cpp \
    db/sqlite/table/dbsqlitepersoneventtbl.cpp \
    db/sqlite/table/dbsqlitepersontbl.cpp \
    db/sqlite/dbsqlitetablebuilder.cpp \
    db/sqlite/table/dbsqliteprovincetbl.cpp \
    db/sqlite/table/dbsqlitesainttbl.cpp \
    db/sqlite/table/dbsqlitespecialisttbl.cpp \
    db/sqlite/table/dbsqlitetbl.cpp \
    db/sqlite/table/dbsqliteworktbl.cpp \
    controller/eductl.cpp \
    export/exporter.cpp \
    export/exportfactory.cpp \
    export/exporthtml.cpp \
    export/iexporter.cpp \
    file/filectl.cpp \
    loader/loaderctl.cpp \
    location/location.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    model/area.cpp \
    model/church.cpp \
    model/community.cpp \
    model/country.cpp \
    model/department.cpp \
    model/education.cpp \
    model/ethnic.cpp \
    model/mission.cpp \
    model/person.cpp \
    model/personbasic.cpp \
    model/personevent.cpp \
    model/province.cpp \
    model/saint.cpp \
    model/specialist.cpp \
    model/work.cpp \
    controller/personctl.cpp \
    report/reportctl.cpp \
    controller/saintctl.cpp \
    search/searchctl.cpp \
    controller/specialistctl.cpp \
    statistic/statistic.cpp \
    test/testctl.cpp \
    utils.cpp \
    view/dialog/dlgaddcommunityhistory.cpp \
    view/dialog/dlgcommunity.cpp \
    view/dialog/dlgdepartment.cpp \
    view/dialog/dlghtmlviewer.cpp \
    view/dialog/dlgperson.cpp \
    view/widget/uicommonlistview.cpp \
    view/widget/uicommunitylistview.cpp \
    view/widget/uiitembutton.cpp \
    view/widget/uimulticomboxview.cpp \
    view/widget/uipersonlistview.cpp \
    view/widget/uisaintlistview.cpp \
    view/widget/uisummarizeview.cpp \
    view/widget/uitableview.cpp \
    view/widget/uitableviewfactory.cpp

HEADERS += \
    address.h \
    cache/cachectl.h \
    controller/areactl.h \
    controller/communityctl.h \
    config/config.h \
    controller/controller.h \
    controller/countryctl.h \
    controller/departctl.h \
    controller/ethnicctl.h \
    controller/missionctl.h \
    controller/provincectl.h \
    crypto/crypto.h \
    db/dbctl.h \
    db/dbinfo.h \
    db/dbmodel.h \
    db/dbmodelhandler.h \
    db/idatabase.h \
    db/sqlite/dbsqlite.h \
    db/sqlite/dbsqlitearea.h \
    db/sqlite/dbsqlitecommunity.h \
    db/sqlite/dbsqlitecountry.h \
    db/sqlite/dbsqlitedefs.h \
    db/sqlite/dbsqlitedept.h \
    db/sqlite/dbsqliteedu.h \
    db/sqlite/dbsqliteethnic.h \
    db/sqlite/dbsqliteinsertbuilder.h \
    db/sqlite/dbsqlitemission.h \
    db/sqlite/dbsqlitemodelhandler.h \
    db/sqlite/dbsqliteprovince.h \
    db/sqlite/dbsqlitesaint.h \
    db/sqlite/dbsqlitespecialist.h \
    db/sqlite/table/dbsqliteareatbl.h \
    db/sqlite/table/dbsqlitecommunitytbl.h \
    db/sqlite/table/dbsqlitecountrytbl.h \
    db/sqlite/table/dbsqlitedeparttbl.h \
    db/sqlite/table/dbsqliteedutbl.h \
    db/sqlite/table/dbsqliteethnictbl.h \
    db/sqlite/table/dbsqlitemissiontbl.h \
    db/sqlite/table/dbsqlitepersoneventtbl.h \
    db/sqlite/table/dbsqlitepersontbl.h \
    db/sqlite/dbsqlitetablebuilder.h \
    db/sqlite/table/dbsqliteprovincetbl.h \
    db/sqlite/table/dbsqlitesainttbl.h \
    db/sqlite/table/dbsqlitespecialisttbl.h \
    db/sqlite/table/dbsqlitetbl.h \
    db/sqlite/table/dbsqliteworktbl.h \
    defs.h \
    controller/eductl.h \
    errcode.h \
    export/exporter.h \
    export/exportfactory.h \
    export/exporthtml.h \
    export/iexporter.h \
    file/filectl.h \
    loader/loaderctl.h \
    location/location.h \
    logger.h \
    mainwindow.h \
    model/area.h \
    model/church.h \
    model/community.h \
    model/country.h \
    model/department.h \
    model/education.h \
    model/ethnic.h \
    model/mission.h \
    model/person.h \
    model/personbasic.h \
    model/personevent.h \
    model/province.h \
    model/saint.h \
    model/specialist.h \
    model/work.h \
    controller/personctl.h \
    report/reportctl.h \
    controller/saintctl.h \
    search/searchctl.h \
    controller/specialistctl.h \
    statistic/statistic.h \
    std.h \
    test/testctl.h \
    utils.h \
    view/dialog/dlgaddcommunityhistory.h \
    view/dialog/dlgcommunity.h \
    view/dialog/dlgdepartment.h \
    view/dialog/dlghtmlviewer.h \
    view/dialog/dlgperson.h \
    view/widget/uicommonlistview.h \
    view/widget/uicommunitylistview.h \
    view/widget/uiitembutton.h \
    view/widget/uimulticomboxview.h \
    view/widget/uipersonlistview.h \
    view/widget/uisaintlistview.h \
    view/widget/uisummarizeview.h \
    view/widget/uitableview.h \
    view/widget/uitableviewfactory.h

FORMS += \
    mainwindow.ui \
    view/dialog/dlgaddcommunityhistory.ui \
    view/dialog/dlgcommunity.ui \
    view/dialog/dlgdepartment.ui \
    view/dialog/dlghtmlviewer.ui \
    view/dialog/dlgperson.ui \
    view/widget/uimulticomboxview.ui \
    view/widget/uisummarizeview.ui \
    view/widget/uitableview.ui

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
    res/Saints.csv \
    res/area_vi.csv \
    res/department_vi.json \
    res/edu_vi.csv \
    res/home.html \
    res/person_info_template.html \
    res/role_vi.csv

RESOURCES += \
    icon.qrc \
    resource.qrc
