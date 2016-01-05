#include <QGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QDebug>

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
