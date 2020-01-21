#include <QGuiApplication>
#include "sceneglwindow.h"
#include "game.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);
    SceneGLWindow window;
    // 反走样
    QSurfaceFormat fmt;
    fmt.setSamples(16);
    QSurfaceFormat::setDefaultFormat(fmt);
    window.setFormat(fmt);

    Game scene(&window);
    window.setScene(&scene);

    window.resize(600, 400);
    window.show();
    return app.exec();
}
