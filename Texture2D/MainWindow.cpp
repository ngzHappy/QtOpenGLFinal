#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"
#include <QString>
#include <QList>
#include <QDebug>

extern QList<QString> glslFileSearchPath;

class Program {
    Program(const Program &)=delete;
    Program(Program &&)=delete;
    Program&operator=(const Program &)=delete;
    Program&operator=(Program &&)=delete;
public:
    GLuint program=0;
    GLuint texture=0;
    GLuint data,index;
    GLuint vao;
    GLuint sampler;
    Program() {
        glCreateSamplers(1,&sampler);
        glCreateVertexArrays(1,&vao);
        glCreateBuffers(1,&data); 
        glCreateBuffers(1,&index);
        program=createProgram({ 
            {GLSLShaderType::FRAGMENT_SHADER,readGLSLFile("glsl/f.frag",glslFileSearchPath)},
            {GLSLShaderType::VERTEX_SHADER,readGLSLFile("glsl/v.vert",glslFileSearchPath)} 
        });
        glCreateTextures(GL_TEXTURE_2D,1,&texture);
        auto image_=readGLSLImage("image/test.jpg",glslFileSearchPath);
        image_ = image_.mirrored(false,true);
        glTextureStorage2D(texture,1, GL_RGBA32F ,image_.width(),image_.height());
        glTextureSubImage2D(texture,0,0,0,image_.width(),image_.height(),GL_RGBA,GL_UNSIGNED_BYTE,image_.constBits());
        const constexpr static GLfloat data_[]{
            -1.0f, 1.0f,0,1,
            -1.0f,-1.0f,0,1,
             1.0f,-1.0f,0,1,
             1.0f, 1.0f,0,1
        };
        glNamedBufferData(data,sizeof(data_),data_,GL_STATIC_DRAW);
        const constexpr static GLuint index_[]{0,1,3,3,1,2};
        glNamedBufferData(index,sizeof(index_),index_,GL_STATIC_DRAW);
        glEnableVertexArrayAttrib(vao,0);
        glVertexArrayAttribFormat(vao,0,4,GL_FLOAT,false,0);
        glVertexArrayVertexBuffer(vao,0,data,0,sizeof(GLfloat[4]));
        glVertexArrayAttribBinding(vao,0,0);
        glVertexArrayElementBuffer(vao,index);
        glSamplerParameteri(sampler,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glSamplerParameteri(sampler,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    void draw() {
        glUseProgram(program);
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D,texture);
        glBindSampler(0,sampler);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr );
    }
    ~Program() { 
        glDeleteSamplers(1,&sampler);
        glDeleteVertexArrays(1,&vao);
        glDeleteBuffers(1,&index);
        glDeleteBuffers(1,&data);
        glDeleteProgram(program);
        glDeleteTextures(1,&texture);
    }
};

class MainWindow::MainWindowData {
public:
    Program program;
};

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){
    delete thisData;
}


void MainWindow::paintGL() {
    const static float color_[]{(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1};
    glClearBufferfv(GL_COLOR,0,color_);
    thisData->program.draw();
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){
    if (thisData) { return; }
    setSimpleCallbackFunction();
    thisData=new MainWindowData;
    this->resize(512,512);
}


