 
#include <ZGL/QGLTool.hpp>
#include <QGLWidget>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QDebug>
#include <QList>
#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <vector>

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

GLuint createProgram( const std::initializer_list<GLSLFile> & glslFiles )try{
    const auto shaderSize=glslFiles.size();
    if( shaderSize<1 ){return 0;}

    class Shader{
    public:
        GLuint value;
        Shader(GLuint v):value(v){ if(v==0){throw v;} }
        Shader(Shader && o):value(o.value){o.value=0;}
        Shader(const Shader &)=delete;
        Shader&operator=(const Shader &)=delete;
        Shader&operator=(Shader &&)=delete;
        ~Shader() { glDeleteShader(value); }
    };

    std::vector< Shader > shaders;
    shaders.reserve( shaderSize );

    for (const auto & i :glslFiles) {/*create shaders*/
        shaders.emplace_back( glCreateShader(i.type) );
    }

    {/*set shader source*/
        auto fp=glslFiles.begin(); const auto fe=glslFiles.end();
        auto sp=shaders.begin(); 
        for (;fp!=fe;++fp,++sp) {
            const QByteArray shader_data_=fp->data.toUtf8();
            if (shader_data_.isEmpty()) { throw "shader data empty"; }
            const char * shader_data[]{ shader_data_.constData()};
            glShaderSource(sp->value,1,shader_data,nullptr);
        }
    }

    {/*build shader source*/
        for (const auto &shader:shaders) {glCompileShader(shader.value);}
    }

    class Program {
    public:
        GLuint value;
        Program(GLuint v):value(v) {if (0==value) { throw "null program"; }}
        ~Program() { glDeleteProgram(value); }
    };

    Program program{ glCreateProgram() };

    {/*attch shader*/
        for (const auto &shader:shaders) {glAttachShader(program.value,shader.value);}
    }

    /*link program*/
    glLinkProgram(program.value);

    {
        const auto ans=program.value;
        program.value=0;
        return ans;
    }
}catch( ... ){
return 0;
}

