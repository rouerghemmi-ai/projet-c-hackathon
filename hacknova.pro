QT += core gui widgets sql

CONFIG += c++17

TARGET = hacknova
TEMPLATE = app

# Tous les fichiers doivent etre dans CE MEME dossier que hacknova.pro
INCLUDEPATH += $$PWD

SOURCES += \
    connection.cpp \
    evaluateur.cpp \
    oracletable.cpp \
    integration_db.cpp \
    main.cpp \
    integration.cpp \
    gestion_evaluateurs.cpp

HEADERS += \
    connection.h \
    evaluateur.h \
    oracletable.h \
    integration.h \
    gestion_evaluateurs.h

FORMS += \
    integration.ui \
    gestion_evaluateurs.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
