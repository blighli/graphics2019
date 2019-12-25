#-------------------------------------------------
#
# Project created by QtCreator 2019-12-14T17:24:40
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp \
    Cube.cpp

HEADERS  += \
    Cube.h

FORMS    += \
    Cube.ui

LIBS += -lopengl32\
    -lglu32\
    -lglut\
