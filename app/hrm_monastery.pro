QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = QuanLyHoiDong
CONFIG += c++17

CONFIG += console
QT += sql
QT += widgets
QT += gui

DEFINES += FEATURE_system_sqlite=OFF

DEFINES += TEST_ENABLE
DEFINES += LOG_LEVEL=4
DEFINES += DEBUG_TRACE
DEFINES += DEBUG_LOG

DEFINES += VER_MAJOR=0
DEFINES += VER_MINOR=1
DEFINES += VER_PATCH=1

# Supporting provine for person is quite complicated
# as it need to sync up with country
# Need to re-check login before enable it again
# don't want to clean up code, just keep it for later use if have time
# TODO: check to support person province info
SUPPORT_PROVINE = OFF


# QXlsx code for Application Qt project
QXLSX_PARENTPATH=QXlsx/QXlsx/         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=QXlsx/QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=QXlsx/QXlsx/source/  # current QXlsx source path is ./source/
include(QXlsx/QXlsx/QXlsx.pri)


INCLUDEPATH += $$PWD/db
INCLUDEPATH += $$PWD/db/sqlite
INCLUDEPATH += $$PWD/db/sqlite/handler
INCLUDEPATH += $$PWD/model
INCLUDEPATH += $$PWD/view
INCLUDEPATH += $$PWD/view/dialog
INCLUDEPATH += $$PWD/config
INCLUDEPATH += $$PWD/file
INCLUDEPATH += $$PWD/controller
INCLUDEPATH += $$PWD/crypto
INCLUDEPATH += $$PWD/location
INCLUDEPATH += $$PWD/export
INCLUDEPATH += $$PWD/import
INCLUDEPATH += $$PWD/report
INCLUDEPATH += $$PWD/exception
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
    controller/communitydeptctl.cpp \
    controller/controller.cpp \
    controller/modelcontroller.cpp \
    controller/countryctl.cpp \
    controller/coursectl.cpp \
    controller/departctl.cpp \
    controller/ethnicctl.cpp \
    controller/eventctl.cpp \
    controller/missionctl.cpp \
    controller/provincectl.cpp \
    controller/rolectl.cpp \
    controller/statusctl.cpp \
    controller/workctl.cpp \
    crypto/crypto.cpp \
    db/dbareamodelhandler.cpp \
    db/dbcommdepatmodelhandler.cpp \
    db/dbcommunitymodelhandler.cpp \
    db/dbctl.cpp \
    db/dbinfo.cpp \
    db/dbmodel.cpp \
    db/dbmodelhandler.cpp \
    db/dbpersonmodelhandler.cpp \
    db/dbspecialistmodelhandler.cpp \
    db/idatabase.cpp \
    db/sqlite/dbsqlite.cpp \
    db/sqlite/dbsqlitedefs.cpp \
    db/sqlite/dbsqlitedeletebuilder.cpp \
    db/sqlite/dbsqliteinsertbuilder.cpp \
    db/sqlite/dbsqliteupdatebuilder.cpp \
    db/sqlite/handler/dbsqlitecommunitydept.cpp \
    db/sqlite/handler/dbsqlitemission.cpp \
    db/sqlite/handler/dbsqlitemodelhandler.cpp \
    db/sqlite/handler/dbsqliteperson.cpp \
    db/sqlite/handler/dbsqlitepersonevent.cpp \
    db/sqlite/handler/dbsqliteprovince.cpp \
    db/sqlite/handler/dbsqliterole.cpp \
    db/sqlite/handler/dbsqlitesaint.cpp \
    db/sqlite/handler/dbsqlitespecialist.cpp \
    db/sqlite/handler/dbsqlitestatus.cpp \
    db/sqlite/handler/dbsqlitework.cpp \
    db/sqlite/handler/dbsqlitearea.cpp \
    db/sqlite/handler/dbsqlitecommunity.cpp \
    db/sqlite/handler/dbsqlitecountry.cpp \
    db/sqlite/handler/dbsqlitecourse.cpp \
    db/sqlite/handler/dbsqlitedept.cpp \
    db/sqlite/handler/dbsqliteedu.cpp \
    db/sqlite/handler/dbsqliteethnic.cpp \
    db/sqlite/handler/dbsqliteevent.cpp \
    exception/exception.cpp \
    import/importlistener.cpp \
    model/areacommunity.cpp \
    model/areaperson.cpp \
    model/communityperson.cpp \
    model/mapdbmodel.cpp \
    model/specialistperson.cpp \
    db/sqlite/table/dbsqliteareamgrtbl.cpp \
    db/sqlite/table/dbsqliteareatbl.cpp \
    db/sqlite/table/dbsqlitecommdeptpersontbl.cpp \
    db/sqlite/table/dbsqlitecommunitydepttbl.cpp \
    db/sqlite/table/dbsqlitecommunitypersontbl.cpp \
    db/sqlite/table/dbsqlitecommunitytbl.cpp \
    db/sqlite/table/dbsqlitecountrytbl.cpp \
    db/sqlite/table/dbsqlitecoursetbl.cpp \
    db/sqlite/table/dbsqlitedeparttbl.cpp \
    db/sqlite/table/dbsqliteedutbl.cpp \
    db/sqlite/table/dbsqliteethnictbl.cpp \
    db/sqlite/table/dbsqliteeventtbl.cpp \
    db/sqlite/table/dbsqlitemaptbl.cpp \
    db/sqlite/table/dbsqlitemissiontbl.cpp \
    db/sqlite/table/dbsqlitepersoneventtbl.cpp \
    db/sqlite/table/dbsqlitepersontbl.cpp \
    db/sqlite/dbsqlitetablebuilder.cpp \
    db/sqlite/table/dbsqliteprovincetbl.cpp \
    db/sqlite/table/dbsqliteroletbl.cpp \
    db/sqlite/table/dbsqlitesaintpersonmaptbl.cpp \
    db/sqlite/table/dbsqlitesainttbl.cpp \
    db/sqlite/table/dbsqlitespecialistpersontbl.cpp \
    db/sqlite/table/dbsqlitespecialisttbl.cpp \
    db/sqlite/table/dbsqlitestatustbl.cpp \
    db/sqlite/table/dbsqlitetbl.cpp \
    db/sqlite/table/dbsqliteworktbl.cpp \
    controller/eductl.cpp \
    export/dataexporter.cpp \
    export/exportcsvlist.cpp \
    export/exportfactory.cpp \
    export/exporthtml.cpp \
    export/exporttype.cpp \
    export/exportxlsx.cpp \
    export/fileexporter.cpp \
    file/filectl.cpp \
    filter.cpp \
    import/idataimporter.cpp \
    import/importcsv.cpp \
    import/importcsvlist.cpp \
    import/importer.cpp \
    import/importfactory.cpp \
    loader/loaderctl.cpp \
    location/location.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    model/area.cpp \
    model/community.cpp \
    model/communitydept.cpp \
    model/country.cpp \
    model/course.cpp \
    model/department.cpp \
    model/education.cpp \
    model/ethnic.cpp \
    model/event.cpp \
    model/mission.cpp \
    model/person.cpp \
    model/persondept.cpp \
    model/personevent.cpp \
    model/province.cpp \
    model/role.cpp \
    model/saint.cpp \
    model/saintperson.cpp \
    model/specialist.cpp \
    model/status.cpp \
    model/work.cpp \
    controller/personctl.cpp \
    report/errreporterctl.cpp \
    report/reportctl.cpp \
    controller/saintctl.cpp \
    search/searchctl.cpp \
    controller/specialistctl.cpp \
    statistic/statistic.cpp \
    test/testctl.cpp \
    utils.cpp \
    view/dialog/dlgabout.cpp \
    view/dialog/dlgaddcommunityhistory.cpp \
    view/dialog/dlgaddevent.cpp \
    view/dialog/dlgaddpersonevent.cpp \
    view/dialog/dlgcommoneditmodel.cpp \
    view/dialog/dlgcommunity.cpp \
    view/dialog/dlgcountry.cpp \
    view/dialog/dlgcourse.cpp \
    view/dialog/dlgdepartment.cpp \
    view/dialog/dlgdeptmgr.cpp \
    view/dialog/dlgeditmodel.cpp \
    view/dialog/dlgethnic.cpp \
    view/dialog/dlghtmlviewer.cpp \
    view/dialog/dlgimportcommunitylistresult.cpp \
    view/dialog/dlgimportexportselect.cpp \
    view/dialog/dlgimportlistresult.cpp \
    view/dialog/dlgimportpersonlistresult.cpp \
    view/dialog/dlgperson.cpp \
    view/dialog/dlgprovince.cpp \
    view/dialog/dlgsaint.cpp \
    view/dialog/dlgsearch.cpp \
    view/dialog/dlgsearchcommunity.cpp \
    view/dialog/dlgsearchperson.cpp \
    view/dialog/dlgwait.cpp \
    view/widget/baseview.cpp \
    view/widget/uiarealistview.cpp \
    view/widget/uicommdeptlistview.cpp \
    view/widget/uicommonlistview.cpp \
    view/widget/uicommunitylistview.cpp \
    view/widget/uicommunitypersonlistview.cpp \
    view/widget/uicountrylistview.cpp \
    view/widget/uicourselistview.cpp \
    view/widget/uidepartmentlistview.cpp \
    view/widget/uidepartmentpersonlistview.cpp \
    view/widget/uieducationlistview.cpp \
    view/widget/uiethniclistview.cpp \
    view/widget/uiitembutton.cpp \
    view/widget/uimissionlistview.cpp \
    view/widget/uimulticomboxview.cpp \
    view/widget/uipersonlistview.cpp \
    view/widget/uirolelistview.cpp \
    view/widget/uisaintlistview.cpp \
    view/widget/uispecialistlistview.cpp \
    view/widget/uisummarizeview.cpp \
    view/widget/uitableview.cpp \
    view/widget/uitableviewfactory.cpp \
    view/widget/uitextbrowser.cpp \
    view/widget/uiworklistview.cpp

HEADERS += \
    address.h \
    cache/cachectl.h \
    controller/areactl.h \
    controller/communityctl.h \
    config/config.h \
    controller/communitydeptctl.h \
    controller/controller.h \
    controller/modelcontroller.h \
    controller/countryctl.h \
    controller/coursectl.h \
    controller/departctl.h \
    controller/ethnicctl.h \
    controller/eventctl.h \
    controller/missionctl.h \
    controller/provincectl.h \
    controller/rolectl.h \
    controller/statusctl.h \
    controller/workctl.h \
    crypto/crypto.h \
    db/dbareamodelhandler.h \
    db/dbcommdepatmodelhandler.h \
    db/dbcommunitymodelhandler.h \
    db/dbctl.h \
    db/dbdefs.h \
    db/dbinfo.h \
    db/dbmodel.h \
    db/dbmodelhandler.h \
    db/dbpersonmodelhandler.h \
    db/dbspecialistmodelhandler.h \
    db/idatabase.h \
    db/sqlite/dbsqlite.h \
    db/sqlite/dbsqlitedefs.h \
    db/sqlite/dbsqlitedeletebuilder.h \
    db/sqlite/dbsqliteinsertbuilder.h \
    db/sqlite/dbsqliteupdatebuilder.h \
    db/sqlite/handler/dbsqlitearea.h \
    db/sqlite/handler/dbsqlitecommunity.h \
    db/sqlite/handler/dbsqlitecommunitydept.h \
    db/sqlite/handler/dbsqlitecountry.h \
    db/sqlite/handler/dbsqlitecourse.h \
    db/sqlite/handler/dbsqlitedept.h \
    db/sqlite/handler/dbsqliteedu.h \
    db/sqlite/handler/dbsqliteethnic.h \
    db/sqlite/handler/dbsqliteevent.h \
    db/sqlite/handler/dbsqlitemission.h \
    db/sqlite/handler/dbsqlitemodelhandler.h \
    db/sqlite/handler/dbsqliteperson.h \
    db/sqlite/handler/dbsqlitepersonevent.h \
    db/sqlite/handler/dbsqliteprovince.h \
    db/sqlite/handler/dbsqliterole.h \
    db/sqlite/handler/dbsqlitesaint.h \
    db/sqlite/handler/dbsqlitespecialist.h \
    db/sqlite/handler/dbsqlitestatus.h \
    db/sqlite/handler/dbsqlitework.h \
    exception/exception.h \
    import/importlistener.h \
    model/areacommunity.h \
    model/areaperson.h \
    model/communityperson.h \
    model/mapdbmodel.h \
    model/specialistperson.h \
    db/sqlite/table/dbsqliteareamgrtbl.h \
    db/sqlite/table/dbsqliteareatbl.h \
    db/sqlite/table/dbsqlitecommdeptpersontbl.h \
    db/sqlite/table/dbsqlitecommunitydepttbl.h \
    db/sqlite/table/dbsqlitecommunitypersontbl.h \
    db/sqlite/table/dbsqlitecommunitytbl.h \
    db/sqlite/table/dbsqlitecountrytbl.h \
    db/sqlite/table/dbsqlitecoursetbl.h \
    db/sqlite/table/dbsqlitedeparttbl.h \
    db/sqlite/table/dbsqliteedutbl.h \
    db/sqlite/table/dbsqliteethnictbl.h \
    db/sqlite/table/dbsqliteeventtbl.h \
    db/sqlite/table/dbsqlitemaptbl.h \
    db/sqlite/table/dbsqlitemissiontbl.h \
    db/sqlite/table/dbsqlitepersoneventtbl.h \
    db/sqlite/table/dbsqlitepersontbl.h \
    db/sqlite/dbsqlitetablebuilder.h \
    db/sqlite/table/dbsqliteprovincetbl.h \
    db/sqlite/table/dbsqliteroletbl.h \
    db/sqlite/table/dbsqlitesaintpersonmaptbl.h \
    db/sqlite/table/dbsqlitesainttbl.h \
    db/sqlite/table/dbsqlitespecialistpersontbl.h \
    db/sqlite/table/dbsqlitespecialisttbl.h \
    db/sqlite/table/dbsqlitestatustbl.h \
    db/sqlite/table/dbsqlitetbl.h \
    db/sqlite/table/dbsqliteworktbl.h \
    defs.h \
    controller/eductl.h \
    errcode.h \
    export/dataexporter.h \
    export/exportcsvlist.h \
    export/exportfactory.h \
    export/exporthtml.h \
    export/exporttype.h \
    export/exportxlsx.h \
    export/fileexporter.h \
    file/filectl.h \
    filter.h \
    import/idataimporter.h \
    import/importcsv.h \
    import/importcsvlist.h \
    import/importer.h \
    import/importfactory.h \
    loader/loaderctl.h \
    location/location.h \
    logger.h \
    mainwindow.h \
    model/area.h \
    model/community.h \
    model/communitydept.h \
    model/country.h \
    model/course.h \
    model/department.h \
    model/education.h \
    model/ethnic.h \
    model/event.h \
    model/mission.h \
    model/person.h \
    model/persondept.h \
    model/personevent.h \
    model/province.h \
    model/role.h \
    model/saint.h \
    model/saintperson.h \
    model/specialist.h \
    model/status.h \
    model/work.h \
    controller/personctl.h \
    report/errreporterctl.h \
    report/reportctl.h \
    controller/saintctl.h \
    search/searchctl.h \
    controller/specialistctl.h \
    statistic/statistic.h \
    std.h \
    test/testctl.h \
    utils.h \
    view/dialog/dlgabout.h \
    view/dialog/dlgaddcommunityhistory.h \
    view/dialog/dlgaddevent.h \
    view/dialog/dlgaddpersonevent.h \
    view/dialog/dlgcommoneditmodel.h \
    view/dialog/dlgcommunity.h \
    view/dialog/dlgcountry.h \
    view/dialog/dlgcourse.h \
    view/dialog/dlgdepartment.h \
    view/dialog/dlgdeptmgr.h \
    view/dialog/dlgeditmodel.h \
    view/dialog/dlgethnic.h \
    view/dialog/dlghtmlviewer.h \
    view/dialog/dlgimportcommunitylistresult.h \
    view/dialog/dlgimportexportselect.h \
    view/dialog/dlgimportlistresult.h \
    view/dialog/dlgimportpersonlistresult.h \
    view/dialog/dlgperson.h \
    view/dialog/dlgprovince.h \
    view/dialog/dlgsaint.h \
    view/dialog/dlgsearch.h \
    view/dialog/dlgsearchcommunity.h \
    view/dialog/dlgsearchperson.h \
    view/dialog/dlgwait.h \
    view/widget/baseview.h \
    view/widget/uiarealistview.h \
    view/widget/uicommdeptlistview.h \
    view/widget/uicommonlistview.h \
    view/widget/uicommunitylistview.h \
    view/widget/uicommunitypersonlistview.h \
    view/widget/uicountrylistview.h \
    view/widget/uicourselistview.h \
    view/widget/uidepartmentlistview.h \
    view/widget/uidepartmentpersonlistview.h \
    view/widget/uieducationlistview.h \
    view/widget/uiethniclistview.h \
    view/widget/uiitembutton.h \
    view/widget/uimissionlistview.h \
    view/widget/uimulticomboxview.h \
    view/widget/uipersonlistview.h \
    view/widget/uirolelistview.h \
    view/widget/uisaintlistview.h \
    view/widget/uispecialistlistview.h \
    view/widget/uisummarizeview.h \
    view/widget/uitableview.h \
    view/widget/uitableviewfactory.h \
    view/widget/uitextbrowser.h \
    view/widget/uiworklistview.h

FORMS += \
    mainwindow.ui \
    view/dialog/dlgabout.ui \
    view/dialog/dlgaddcommunityhistory.ui \
    view/dialog/dlgaddevent.ui \
    view/dialog/dlgaddpersonevent.ui \
    view/dialog/dlgcommunity.ui \
    view/dialog/dlgcountry.ui \
    view/dialog/dlgcourse.ui \
    view/dialog/dlgdepartment.ui \
    view/dialog/dlgdeptmgr.ui \
    view/dialog/dlgeditmodel.ui \
    view/dialog/dlgethnic.ui \
    view/dialog/dlghtmlviewer.ui \
    view/dialog/dlgimportexportselect.ui \
    view/dialog/dlgimportlistresult.ui \
    view/dialog/dlgperson.ui \
    view/dialog/dlgprovince.ui \
    view/dialog/dlgsaint.ui \
    view/dialog/dlgsearch.ui \
    view/dialog/dlgwait.ui \
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
    res/comm_dept_vi.json \
    res/common_template.html \
    res/community_export_template_vi.json \
    res/course_vi.csv \
    res/default_export_template_vi.json \
    res/department_vi.json \
    res/edu_vi.csv \
    res/event_category_vi.csv \
    res/home.html \
    res/person_info_template.html \
    res/person_list_export_template.csv \
    res/person_list_export_template_vi.json \
    res/role_vi.csv \
    res/status_vi.csv \
    res/work_vi.csv

RESOURCES += \
    icon.qrc \
    resource.qrc

equals(SUPPORT_PROVINE, OFF) {
DEFINES += SKIP_PERSON_PROVINE
#SOURCES -= \
#    controller/provincectl.cpp \
#    db/sqlite/handler/dbsqliteprovince.cpp \
#    db/sqlite/table/dbsqliteprovincetbl.cpp \
#    model/province.cpp \
#    view/dialog/dlgprovince.cpp \

#FORMS -= \
#    view/dialog/dlgprovince.ui \


}
