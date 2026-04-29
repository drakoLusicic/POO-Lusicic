QT += widgets network
CONFIG += c++17

TARGET = Ejercicio-07-Lusicic
TEMPLATE = app

SOURCES += \
    main.cpp \
    pantalla.cpp \
    clima.cpp \
    login.cpp \
    ventana.cpp

HEADERS += \
    pantalla.h \
    clima.h \
    login.h \
    ventana.h

FORMS += \
    login.ui \
    ventana.ui

DISTFILES += config.ini
