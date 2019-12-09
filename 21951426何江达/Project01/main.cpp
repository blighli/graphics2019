#include <QGuiApplication>
#include "sceneglwindow.h"
#include "Scene.h"
#include <QPropertyAnimation>

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    SceneGLWindow window;

    // 反走样
    QSurfaceFormat fmt;
    fmt.setSamples(16);
    window.setFormat(fmt);

    Scene scene(&window);
    window.setScene(&scene);

    window.resize(320, 240);
    window.show();

    //动画
    QPropertyAnimation animation(&scene, "val");
    animation.setDuration(20000);
    animation.setStartValue(0);
    animation.setKeyValueAt(0.5, 360);
    animation.setEndValue(0);
    animation.setLoopCount(-1);
    animation.start();
    return app.exec();
}
