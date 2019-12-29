#include <QApplication>
#include <iostream>

#include "Cube.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QGLFormat::hasOpenGL()) {
        std::cerr << "This system has no OpenGL support" << std::endl;
        return 1;
    }

    Cube cube;
    cube.setWindowTitle(QObject::tr("My Cube"));
    cube.setMinimumSize(200, 200);
    cube.resize(450, 450);
    cube.show();

    return app.exec();
}
