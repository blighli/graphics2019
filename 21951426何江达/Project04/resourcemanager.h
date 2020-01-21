#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QMap>
#include <QString>

class ResourceManager
{
public:
    static QMap<QString,QOpenGLShaderProgram*> Shaders;
    static QMap<QString,QOpenGLTexture*> Textures;
    static QOpenGLShaderProgram& LoadShader(QString vShaderFile,QString fShaderFile,QString gShaderFile,QString name);
    static QOpenGLShaderProgram& GetShader(QString name);
    static QOpenGLTexture& LoadTexture(QString file,bool alpha,QString name);
    static QOpenGLTexture& GetTexture(QString name);
    static void Clear();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { };
};

#endif // RESOURCEMANAGER_H
