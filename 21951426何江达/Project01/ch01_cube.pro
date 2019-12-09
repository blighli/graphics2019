QT = core gui
TEMPLATE = app
HEADERS += abstractglscene.h     sceneglwindow.h     \
    Camera.h \
    Scene.h
SOURCES += main.cpp abstractglscene.cpp     sceneglwindow.cpp     \
    Camera.cpp \
    Scene.cpp

CONFIG += c++11

RESOURCES +=     res.qrc

