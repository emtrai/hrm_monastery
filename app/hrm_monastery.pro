QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = QuanLyHoiDong
CONFIG += c++17

QT += sql
QT += widgets
QT += gui
QT += gui-private
QT += printsupport

DEFINES += FEATURE_system_sqlite=OFF


#DEFINES += TEST_ENABLE
# disable all for release
CONFIG += console
DEFINES += DEBUG_TRACE
DEFINES += DEBUG_LOG
DEFINES += LOG_LEVEL=4
DEFINES += AUTO_BACKUP_TIME_SEC=60

DEFINES += VER_MAJOR=0
DEFINES += VER_MINOR=1
DEFINES += VER_PATCH=3

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
INCLUDEPATH += $$PWD/image
#QT_NO_DEBUG_OUTPUT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    address.cpp \
    backup/backupctl.cpp \
    backup/backupmetainfo.cpp \
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
    controller/personstatusctl.cpp \
    controller/provincectl.cpp \
    controller/rolectl.cpp \
    controller/workctl.cpp \
    crypto/crypto.cpp \
    datetimeutils.cpp \
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
    db/sqlite/dbmetainfo.cpp \
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
    db/sqlite/handler/dbsqlitepersonstatus.cpp \
    db/sqlite/handler/dbsqliteprovince.cpp \
    db/sqlite/handler/dbsqliterole.cpp \
    db/sqlite/handler/dbsqlitesaint.cpp \
    db/sqlite/handler/dbsqlitespecialist.cpp \
    db/sqlite/handler/dbsqlitework.cpp \
    db/sqlite/handler/dbsqlitearea.cpp \
    db/sqlite/handler/dbsqlitecommunity.cpp \
    db/sqlite/handler/dbsqlitecountry.cpp \
    db/sqlite/handler/dbsqlitecourse.cpp \
    db/sqlite/handler/dbsqlitedept.cpp \
    db/sqlite/handler/dbsqliteedu.cpp \
    db/sqlite/handler/dbsqliteethnic.cpp \
    db/sqlite/handler/dbsqliteevent.cpp \
    db/sqlite/table/dbmetadatatbl.cpp \
    db/sqlite/table/dbsqlitecommunitymanagertbl.cpp \
    db/sqlite/table/dbsqlitepersonstatustbl.cpp \
    db/sqlite/table/dbsqlsequencetbl.cpp \
    dbmodelutils.cpp \
    dialogutils.cpp \
    errcode.cpp \
    exception/exception.cpp \
    image/image.cpp \
    image/imagectl.cpp \
    import/importlistener.cpp \
    import/importxlsx.cpp \
    model/areaperson.cpp \
    model/communitymanager.cpp \
    model/communityperson.cpp \
    model/dbmodelfactory.cpp \
    model/mapdbmodel.cpp \
    model/personstatus.cpp \
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
    view/dialog/dlgarea.cpp \
    view/dialog/dlgareaperson.cpp \
    view/dialog/dlgcommdept.cpp \
    view/dialog/dlgcommmgr.cpp \
    view/dialog/dlgcommoneditmodel.cpp \
    view/dialog/dlgcommunity.cpp \
    view/dialog/dlgconfirmupdatepeoplecomm.cpp \
    view/dialog/dlgcountry.cpp \
    view/dialog/dlgcourse.cpp \
    view/dialog/dlgdeptperson.cpp \
    view/dialog/dlgeditmodel.cpp \
    view/dialog/dlgethnic.cpp \
    view/dialog/dlghtmlviewer.cpp \
    view/dialog/dlgimportareapersonlistresult.cpp \
    view/dialog/dlgimportcommunitylistresult.cpp \
    view/dialog/dlgimportexportselect.cpp \
    view/dialog/dlgimportlistresult.cpp \
    view/dialog/dlgimportlistresultfactory.cpp \
    view/dialog/dlgimportpersonlistresult.cpp \
    view/dialog/dlgmsg.cpp \
    view/dialog/dlgperson.cpp \
    view/dialog/dlgpersoncomm.cpp \
    view/dialog/dlgprovince.cpp \
    view/dialog/dlgsaint.cpp \
    view/dialog/dlgsearch.cpp \
    view/dialog/dlgsearchcommunity.cpp \
    view/dialog/dlgsearchperson.cpp \
    view/dialog/dlgwait.cpp \
    view/widget/baseview.cpp \
    view/widget/uiareacontactpeoplelistview.cpp \
    view/widget/uiarealistview.cpp \
    view/widget/uicommdeptlistview.cpp \
    view/widget/uicommonlistview.cpp \
    view/widget/uicommunitiesofpersonlistview.cpp \
    view/widget/uicommunitylistview.cpp \
    view/widget/uicommunitymgrlistview.cpp \
    view/widget/uicountrylistview.cpp \
    view/widget/uicourselistview.cpp \
    view/widget/uidepartmentlistview.cpp \
    view/widget/uidepartmentpersonlistview.cpp \
    view/widget/uieducationlistview.cpp \
    view/widget/uiethniclistview.cpp \
    view/widget/uiitembutton.cpp \
    view/widget/uimissionlistview.cpp \
    view/widget/uimulticomboxview.cpp \
    view/widget/uipeopleincommunitylistview.cpp \
    view/widget/uipersoneventlistview.cpp \
    view/widget/uipersonlistview.cpp \
    view/widget/uipersonstatuslistview.cpp \
    view/widget/uirolelistview.cpp \
    view/widget/uisaintlistview.cpp \
    view/widget/uispecialistlistview.cpp \
    view/widget/uisummarizeview.cpp \
    view/widget/uitableview.cpp \
    view/widget/uitableviewfactory.cpp \
    view/widget/uitextbrowser.cpp \
    view/widget/uiworklistview.cpp \
    viewutils.cpp

HEADERS += \
    address.h \
    backup/backupctl.h \
    backup/backupmetainfo.h \
    config/configdefs.h \
    controller/areactl.h \
    controller/communityctl.h \
    config/config.h \
    controller/communitydeptctl.h \
    controller/controller.h \
    controller/controllerdefs.h \
    controller/modelcontroller.h \
    controller/countryctl.h \
    controller/coursectl.h \
    controller/departctl.h \
    controller/ethnicctl.h \
    controller/eventctl.h \
    controller/missionctl.h \
    controller/personstatusctl.h \
    controller/provincectl.h \
    controller/rolectl.h \
    controller/workctl.h \
    crypto/crypto.h \
    datetimeutils.h \
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
    db/sqlite/dbmetainfo.h \
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
    db/sqlite/handler/dbsqlitepersonstatus.h \
    db/sqlite/handler/dbsqliteprovince.h \
    db/sqlite/handler/dbsqliterole.h \
    db/sqlite/handler/dbsqlitesaint.h \
    db/sqlite/handler/dbsqlitespecialist.h \
    db/sqlite/handler/dbsqlitework.h \
    db/sqlite/table/dbmetadatatbl.h \
    db/sqlite/table/dbsqlitecommunitymanagertbl.h \
    db/sqlite/table/dbsqlitepersonstatustbl.h \
    db/sqlite/table/dbsqlsequencetbl.h \
    dbmodelutils.h \
    dialogutils.h \
    eventdefs.h \
    exception/exception.h \
    exportdefs.h \
    image/image.h \
    image/imagectl.h \
    imagedefs.h \
    import/importlistener.h \
    import/importtype.h \
    import/importxlsx.h \
    importexportdefs.h \
    jsondefs.h \
    model/areaperson.h \
    model/communitymanager.h \
    model/communityperson.h \
    model/dbmodelfactory.h \
    model/mapdbmodel.h \
    model/personstatus.h \
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
    model/work.h \
    controller/personctl.h \
    modeldefs.h \
    prebuiltdefs.h \
    report/errreporterctl.h \
    report/reportctl.h \
    controller/saintctl.h \
    search/searchctl.h \
    controller/specialistctl.h \
    statistic/statistic.h \
    std.h \
    stringdefs.h \
    test/testctl.h \
    utils.h \
    view/dialog/dlgabout.h \
    view/dialog/dlgaddcommunityhistory.h \
    view/dialog/dlgaddevent.h \
    view/dialog/dlgaddpersonevent.h \
    view/dialog/dlgarea.h \
    view/dialog/dlgareaperson.h \
    view/dialog/dlgcommdept.h \
    view/dialog/dlgcommmgr.h \
    view/dialog/dlgcommoneditmodel.h \
    view/dialog/dlgcommunity.h \
    view/dialog/dlgconfirmupdatepeoplecomm.h \
    view/dialog/dlgcountry.h \
    view/dialog/dlgcourse.h \
    view/dialog/dlgdeptperson.h \
    view/dialog/dlgeditmodel.h \
    view/dialog/dlgethnic.h \
    view/dialog/dlghtmlviewer.h \
    view/dialog/dlgimportareapersonlistresult.h \
    view/dialog/dlgimportcommunitylistresult.h \
    view/dialog/dlgimportexportselect.h \
    view/dialog/dlgimportlistresult.h \
    view/dialog/dlgimportlistresultfactory.h \
    view/dialog/dlgimportpersonlistresult.h \
    view/dialog/dlgmsg.h \
    view/dialog/dlgperson.h \
    view/dialog/dlgpersoncomm.h \
    view/dialog/dlgprovince.h \
    view/dialog/dlgsaint.h \
    view/dialog/dlgsearch.h \
    view/dialog/dlgsearchcommunity.h \
    view/dialog/dlgsearchperson.h \
    view/dialog/dlgwait.h \
    view/widget/baseview.h \
    view/widget/uiareacontactpeoplelistview.h \
    view/widget/uiarealistview.h \
    view/widget/uicommdeptlistview.h \
    view/widget/uicommonlistview.h \
    view/widget/uicommunitiesofpersonlistview.h \
    view/widget/uicommunitylistview.h \
    view/widget/uicommunitymgrlistview.h \
    view/widget/uicountrylistview.h \
    view/widget/uicourselistview.h \
    view/widget/uidepartmentlistview.h \
    view/widget/uidepartmentpersonlistview.h \
    view/widget/uieducationlistview.h \
    view/widget/uiethniclistview.h \
    view/widget/uiitembutton.h \
    view/widget/uimissionlistview.h \
    view/widget/uimulticomboxview.h \
    view/widget/uipeopleincommunitylistview.h \
    view/widget/uipersoneventlistview.h \
    view/widget/uipersonlistview.h \
    view/widget/uipersonstatuslistview.h \
    view/widget/uirolelistview.h \
    view/widget/uisaintlistview.h \
    view/widget/uispecialistlistview.h \
    view/widget/uisummarizeview.h \
    view/widget/uitableview.h \
    view/widget/uitableviewfactory.h \
    view/widget/uitextbrowser.h \
    view/widget/uiworklistview.h \
    viewutils.h

FORMS += \
    mainwindow.ui \
    view/dialog/dlgabout.ui \
    view/dialog/dlgaddcommunityhistory.ui \
    view/dialog/dlgaddevent.ui \
    view/dialog/dlgaddpersonevent.ui \
    view/dialog/dlgarea.ui \
    view/dialog/dlgareaperson.ui \
    view/dialog/dlgcommdept.ui \
    view/dialog/dlgcommmgr.ui \
    view/dialog/dlgcommunity.ui \
    view/dialog/dlgconfirmupdatepeoplecomm.ui \
    view/dialog/dlgcountry.ui \
    view/dialog/dlgcourse.ui \
    view/dialog/dlgdeptperson.ui \
    view/dialog/dlgeditmodel.ui \
    view/dialog/dlgethnic.ui \
    view/dialog/dlghtmlviewer.ui \
    view/dialog/dlgimportexportselect.ui \
    view/dialog/dlgimportlistresult.ui \
    view/dialog/dlgmsg.ui \
    view/dialog/dlgperson.ui \
    view/dialog/dlgpersoncomm.ui \
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
    res/area_contact_export_template_vi.json \
    res/area_export_template_vi.json \
    res/area_person_info_template.html \
    res/area_vi.csv \
    res/comm_dept_vi.json \
    res/comm_mgr_info_template.html \
    res/common_template.html \
    res/communities_of_person_export_template_vi.json \
    res/communities_of_person_info_template.html \
    res/community_dept_export_template_vi.json \
    res/community_export_template_vi.json \
    res/course_template.html \
    res/course_vi.csv \
    res/default_export_template_vi.json \
    res/department_person_export_template_vi.json \
    res/department_person_info_template.html \
    res/department_vi.json \
    res/edu_vi.csv \
    res/ethnic_info_template_vi.html \
    res/event_category_vi.csv \
    res/general_statistic_template.html \
    res/home.html \
    res/people_in_community_export_template_vi.json \
    res/person_event_info_template.html \
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
