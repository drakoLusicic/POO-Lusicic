QT += widgets sql
CONFIG += c++17

TEMPLATE = app
TARGET = Ejercicio09-Lusicic

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    logindialog.cpp \
    pintura.cpp

HEADERS += \
    mainwindow.h \
    logindialog.h \
    pintura.h

FORMS += \
    mainwindow.ui \
    login.ui
