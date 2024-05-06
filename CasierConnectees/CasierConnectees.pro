QT       += core gui sql network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ABE_IoPi.cpp \
    accesgache.cpp \
    accessbdd.cpp \
    imagedownloader.cpp \
    main.cpp \
    gestionnairecasier.cpp \
    wiegand.cpp

HEADERS += \
    ABE_IoPi.h \
    accesgache.h \
    accessbdd.h \
    gestionnairecasier.h \
    imagedownloader.h \
    wiegand.h

FORMS += \
    gestionnairecasier.ui

LIBS += -lpthread -lwiringPi -lrt
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mesressources.qrc
