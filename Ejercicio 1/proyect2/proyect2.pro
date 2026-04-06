QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authmanager.cpp \
    historymanager.cpp \
    loginwidget.cpp \
    main.cpp \
    mainwidget.cpp \
    noteeditor.cpp \
    workmanager.cpp \
    workboardwidget.cpp \
    workdialog.cpp

HEADERS += \
    authmanager.h \
    historymanager.h \
    loginwidget.h \
    mainwidget.h \
    noteeditor.h \
    workmanager.h \
    workboardwidget.h \
    workdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data/history.json \
    data/session.json \
    data/users.json \
    data/works.json
