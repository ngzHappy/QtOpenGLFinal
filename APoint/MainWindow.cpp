#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"
#include <QString>
#include <QList>
#include <QDebug>

extern QList<QString> glslFileSearchPath;

class MainWindow::MainWindowData{
public:
    MainWindowData(){
        program=createProgram({ 
            {GLSLShaderType::VERTEX_SHADER,readGLSLFile("glsl/v.vert",glslFileSearchPath)},
            {GLSLShaderType::FRAGMENT_SHADER,readGLSLFile("glsl/f.frag",glslFileSearchPath)}
        });
    }
    ~MainWindowData(){
        glDeleteProgram(program);
    }
    GLuint program = 0;
};

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){
    delete thisData;
}

void MainWindow::paintGL() {
    const static float color_[]{0.1f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1};
    glClearBufferfv(GL_COLOR,0,color_);

    GLuint vao;
    glCreateVertexArrays(1,&vao);
    glUseProgram(thisData->program);
    glPointSize(32);
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS,0,1);
    glDeleteVertexArrays(1,&vao);
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){
    if(nullptr == thisData){
        thisData = new MainWindowData;
    }
}


