//#include <ZGL/QInitializeDebugContext.hpp>
#include <QGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QDebug>
#include <QList>
#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>

namespace  {

static void __initialization( QGLWidget * glWidget ){
    if(0==glWidget){return;}
    QOpenGLContext * ch = glWidget->context()->contextHandle() ;
    if(ch){
        QSurfaceFormat format =ch->format() ;
        format.setProfile( QSurfaceFormat::CoreProfile );
        format.setOption( QSurfaceFormat::DebugContext );
        format.setVersion(4,5);
        format.setSamples(8);
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        ch->setFormat(format);
        if(ch->create()){return;}
        else{
            qDebug()<<"error create opengl debug";
        }
    }else{
        qDebug()<<"error init opengl debug";
    }
}

}


extern void _i_qInitializeQGLWidget(QGLWidget *glWidget){
    __initialization(glWidget);
}

extern QString readGLSLFile( const QString & fileName,const QList<QString> & filePath ){
    QString fullFileName;
    for( const auto & path_:filePath ){
        QFileInfo info( path_+"/"+fileName );
        if(info.exists()){fullFileName=info.absoluteFilePath();}
    }
    if(fullFileName.isEmpty()){return QString();}
    QFile file( fullFileName );
    if( file.open(QIODevice::ReadOnly|QIODevice::Text ) ){
        return file.readAll() ;
    }
    return QString();
}

