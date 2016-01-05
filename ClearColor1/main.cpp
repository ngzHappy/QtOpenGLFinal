#include "MainWindow.hpp"
#include <QApplication>
#include <QTextCodec>

QList<QString> glslFileSearchPath;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale( QTextCodec::codecForName(LOCAL_LANGUAGE) );
    glslFileSearchPath.append( app.applicationDirPath()+"/glsl" );
    glslFileSearchPath.append( GLSL_PWD );

    MainWindow window;
    window.show();

    return app.exec();
}
