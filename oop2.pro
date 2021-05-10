QT       += core gui
QT += sql
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += conan_basic_setup
include(conanbuildinfo.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    account.cpp \
    addgoods.cpp \
    changepass.cpp \
    db.cpp \
    main.cpp \
    mainwindow.cpp \
    file_io.cpp \
    managegoods.cpp \
    management.cpp \
    recharge.cpp

HEADERS += \
    account.h \
    addgoods.h \
    changepass.h \
    mainwindow.h \
    file_io.h \
    managegoods.h \
    management.h \
    db.h \
    recharge.h

FORMS += \
    account.ui \
    addgoods.ui \
    changepass.ui \
    mainwindow.ui \
    managegoods.ui \
    recharge.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    conanfile.txt
