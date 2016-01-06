#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"
#include <QString>
#include <QList>
#include <QDebug>

extern QList<QString> glslFileSearchPath;

class TriangleProgram{
public:
    GLuint program =0 ;
    TriangleProgram(const TriangleProgram &)=delete;
    TriangleProgram(TriangleProgram &&)=delete;
    TriangleProgram&operator=(const TriangleProgram &)=delete;
    TriangleProgram&operator=(TriangleProgram &&)=delete;
    TriangleProgram(){
        program=createProgram({ 
            {GLSLShaderType::FRAGMENT_SHADER,"glsl/f.frag"},
            {GLSLShaderType::VERTEX_SHADER,"glsl/v.vert"} 
        });
    }
    void draw( GLuint vao ,int firstPoint,int pointSize ){
        glUseProgram( program );
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES,firstPoint,pointSize);
        glUseProgram( 0 );
    }
    ~TriangleProgram(){glDeleteProgram(program);}
    operator GLuint() const { return program; }
};

class VertexArrayObject {
    VertexArrayObject&operator=(const VertexArrayObject &)=delete;
    VertexArrayObject&operator=(VertexArrayObject &&)=delete;
    VertexArrayObject(const VertexArrayObject &)=delete;
    VertexArrayObject(VertexArrayObject &&)=delete;
public:
    GLuint vertexArrayObject=0;
    VertexArrayObject() {
        glCreateVertexArrays(1,&vertexArrayObject);
    }

    ~VertexArrayObject() {
        glDeleteVertexArrays(1,&vertexArrayObject);
    }
    operator GLuint() const { return vertexArrayObject; }
};

class MainWindow::MainWindowData{
public:
    TriangleProgram triangle;
    VertexArrayObject vertexArrayObject;
};

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){
    delete thisData;
}


void MainWindow::paintGL() {
    const static float color_[]{0.125f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1};
    glClearBufferfv(GL_COLOR,0,color_);
    thisData->triangle.draw( thisData->vertexArrayObject,0,3 );
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){
    thisData = new MainWindowData;
}


