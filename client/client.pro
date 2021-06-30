QT       += core gui network

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
    bill.cpp \
    cartwindow.cpp \
    changepass.cpp \
    client.cpp \
    main.cpp \
    mainwindow.cpp \
    managegoods.cpp \
    recharge.cpp

HEADERS += \
    account.h \
    addgoods.h \
    bill.h \
    cartwindow.h \
    changepass.h \
    client.h \
    mainwindow.h \
    managegoods.h \
    recharge.h

FORMS += \
    account.ui \
    addgoods.ui \
    bill.ui \
    cartwindow.ui \
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
