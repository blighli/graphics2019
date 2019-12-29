#include "Application.hpp"
#if defined(WIN32) || defined(_WIN32)
#include <direct.h>
#define chdir _chdir
#else
#include <unistd.h>
#endif

int main(int argc, char *argv[]) {
    chdir("..");
    Application::initializeContext();
    Application app(argc, argv);
    return app.exec();
}
