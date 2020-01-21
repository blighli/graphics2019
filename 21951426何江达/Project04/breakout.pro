QT = core gui
TEMPLATE = app
HEADERS += abstractglscene.h     sceneglwindow.h \
    ballobject.h \
    game.h \
    gamelevel.h \
    gameobject.h \
    resourcemanager.h \
    spriterenderer.h
SOURCES += main.cpp abstractglscene.cpp     sceneglwindow.cpp \
    ballobject.cpp \
    game.cpp \
    gamelevel.cpp \
    gameobject.cpp \
    resourcemanager.cpp \
    spriterenderer.cpp

CONFIG += c++17

DISTFILES += \
    shaders/sprite.frag \
    shaders/sprite.vert

