#-------------------------------------------------
#
# Project created by QtCreator 2015-07-20T00:32:51
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += network
CONFIG   += C++11

CODECFORTR = UTF-8

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YufeiDanmaku
TEMPLATE = app
INCLUDEPATH += F:\Qt\Qt5.2.0\Tools\QtCreator\bin\YufeiDanmaku
#LIBS += F:\Qt\Qt5.2.0\Tools\QtCreator\bin\YufeiDanmaku\bssQRCodeGenerator.lib
RC_FILE = \
    project.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    messageWidget.cpp \
    titlewidget.cpp \
    doge.cpp \
    BanListWidget.cpp \
    hideWidget.cpp \
    MsgBox.cpp \
    QRenWidget.cpp \
    BitmapImage.cpp \
    DanmuProducer.cpp \
    LibQREncode/bitstream.c \
    LibQREncode/mask.c \
    LibQREncode/mmask.c \
    LibQREncode/mqrspec.c \
    LibQREncode/qrencode.c \
    LibQREncode/qrinput.c \
    LibQREncode/qrspec.c \
    LibQREncode/rscode.c \
    LibQREncode/split.c \
    Danmu.cpp

HEADERS  += mainwindow.h \
    Danmu.h \
    messageWidget.h \
    titlewidget.h \
    doge.h \
    BanListWidget.h \
    hideWidget.h \
    MsgBox.h \
    QRenWidget.h \
    BitmapImage.h \
    LibQREncode/bitstream.h \
    LibQREncode/mask.h \
    LibQREncode/mmask.h \
    LibQREncode/mqrspec.h \
    LibQREncode/qrencode.h \
    LibQREncode/qrencode_inner.h \
    LibQREncode/qrinput.h \
    LibQREncode/qrspec.h \
    LibQREncode/rscode.h \
    LibQREncode/split.h \
    bitstream.h \
    DanmuProducer.h \
    LibQREncode/configure.h

FORMS    += mainwindow.ui \
    widget.ui

RESOURCES += \
    Image.qrc \
    mbg.qrc

OTHER_FILES += \
    project.rc
