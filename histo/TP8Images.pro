TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ImageBase.cpp \
    tp8.cpp \
    CBlock.cpp

DISTFILES += \
    ../TP8.pdf \
    TP8.pdf \
    TP8Images.pro.user

HEADERS += \
    image_ppm.h \
    ImageBase.h \
    CBlock.h

