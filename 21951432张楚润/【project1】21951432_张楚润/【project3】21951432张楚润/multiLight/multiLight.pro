#-------------------------------------------------
#
# Project created by QtCreator 2019-12-29T14:30:14
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = multiLight
TEMPLATE = app


SOURCES += main.cpp

HEADERS  += \
    Camera.h \
    Shader.h

FORMS    += \
    multiLight.ui

LIBS += -lopengl32\
    -lglu32\
    -lglut\

OTHER_FILES += \
    lamp_vertex.vs \
    lamp_fragement.fs \
    obj_vertex.vs \
    obj_fragement.fs

