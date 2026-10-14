QT += widgets

CONFIG += c++17

TARGET = hacknova
TEMPLATE = app

SOURCES += \
    main.cpp \
    integration.cpp

HEADERS += \
    integration.h

FORMS += \
    integration.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target