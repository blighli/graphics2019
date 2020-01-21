QT = core gui
TEMPLATE = app
HEADERS += AbstractGLScene.h     SceneGLWindow.h         Camera.h     Scene.h \
    modelloader.h
SOURCES += main.cpp AbstractGLScene.cpp     SceneGLWindow.cpp         Camera.cpp     Scene.cpp \
    modelloader.cpp

CONFIG += c++11

RESOURCES +=     res.qrc

unix: !macx {
    INCLUDEPATH +=  /home/happy/GLStudy/learnOpenGL/assimp-5.0.0/include
    LIBS += /home/happy/GLStudy/learnOpenGL/assimp-5.0.0/lib/libassimp.so
}
