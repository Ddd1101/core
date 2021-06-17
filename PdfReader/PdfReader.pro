QT       -= core gui

VERSION = 1.0.0.4
TARGET = PdfReader
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

DEFINES += PDFREADER_USE_DYNAMIC_LIBRARY
DEFINES += HTMLRENDERER_USE_DYNAMIC_LIBRARY

ADD_DEPENDENCY(graphics, kernel, UnicodeConverter, HtmlRenderer)

DEFINES += CRYPTOPP_DISABLE_ASM
LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lCryptoPPLib

core_windows {
LIBS += -lgdi32 \
        -ladvapi32 \
        -luser32 \
        -lshell32
}

INCLUDEPATH += \
    $$PWD/lib/goo \
    $$PWD/lib/fofi \
    $$PWD/lib/splash \
    $$PWD/lib

HEADERS += \
    $$PWD/lib/aconf.h \
    $$$files($$PWD/lib/*.h)

SOURCES += $$files($$PWD/lib/*.c, true)
SOURCES += $$files($$PWD/lib/*.cc, true)

SOURCES -= \
    $$PWD/lib/xpdf/HTMLGen.cc \
    $$PWD/lib/xpdf/pdftohtml.cc \
    $$PWD/lib/xpdf/pdftopng.cc \
    $$PWD/lib/xpdf/pdftoppm.cc \
    $$PWD/lib/xpdf/pdftops.cc \
    $$PWD/lib/xpdf/pdftotext.cc \
    $$PWD/lib/xpdf/pdfimages.cc \
    $$PWD/lib/xpdf/pdfdetatch.cc \
    $$PWD/lib/xpdf/pdfinfo.cc \ 
    $$PWD/lib/xpdf/pdffonts.cc

SOURCES += \
    PdfReader.cpp

HEADERS +=\
    Resources/Fontd050000l.h \
    Resources/Fontn019003l.h \
    Resources/Fontn019004l.h \
    Resources/Fontn019023l.h \
    Resources/Fontn019024l.h \
    Resources/Fontn021003l.h \
    Resources/Fontn021004l.h \
    Resources/Fontn021023l.h \
    Resources/Fontn021024l.h \
    Resources/Fontn022003l.h \
    Resources/Fontn022004l.h \
    Resources/Fontn022023l.h \
    Resources/Fontn022024l.h \
    Resources/Fonts050000l.h \
    PdfReader.h
