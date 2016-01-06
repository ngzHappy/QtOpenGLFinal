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
            {GLSLShaderType::VERTEX_SHADER,readGLSLFile("glsl/v.vert",glslFileSearchPath)} ,
            {GLSLShaderType::FRAGMENT_SHADER,readGLSLFile("glsl/f.frag",glslFileSearchPath)},
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
    GLuint buffer[3];
    VertexArrayObject() {
        glCreateVertexArrays(1,&vertexArrayObject);
        glCreateBuffers(3,buffer);
        const static constexpr float data_0[]{
            -0.5f,-0.5f,0.0f,1,
             0.5f,-0.5f,0.0f,1,
             0.0f, 0.0f,0.0f,1,
        };
        const static constexpr float data_1[]{
            -0.5f,-0.5f,0.0f,1,
            0.5f,-0.5f,0.0f,1,
            0.0f, .25f,0.0f,1,
        };
        const static constexpr float data_2[]{
            -0.5f,-0.5f,0.0f,1,
            0.5f,-0.5f,0.0f,1,
            0.0f, 0.5f,0.0f,1,
        };

        glNamedBufferData(buffer[0],sizeof(data_0),data_0,GL_STATIC_DRAW);
        glNamedBufferData(buffer[1],sizeof(data_1),data_1,GL_STATIC_DRAW);
        glNamedBufferData(buffer[2],sizeof(data_2),data_2,GL_STATIC_DRAW);

        glVertexArrayVertexBuffer(vertexArrayObject,0,buffer[0],0,sizeof(float[4]));
        glVertexArrayVertexBuffer(vertexArrayObject,1,buffer[1],0,sizeof(float[4]));
        glVertexArrayVertexBuffer(vertexArrayObject,2,buffer[2],0,sizeof(float[4]));

        glEnableVertexArrayAttrib(vertexArrayObject,0);
        glVertexArrayAttribFormat(vertexArrayObject,0,4,GL_FLOAT,false,0);
        glVertexArrayAttribBinding(vertexArrayObject,0,rand()%3);

    }

    ~VertexArrayObject() {
        glDeleteVertexArrays(1,&vertexArrayObject);
        glDeleteBuffers(3,buffer);
    }
    operator GLuint() const { return vertexArrayObject; }
};

class MainWindow::MainWindowData{
public:
    TriangleProgram triangle;
};

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){
    delete thisData;
}


void MainWindow::paintGL() {
    VertexArrayObject vertexArrayObject;
    const static float color_[]{0.325f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1};
    glClearBufferfv(GL_COLOR,0,color_);
    thisData->triangle.draw( vertexArrayObject,0,3 );
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){
    if (thisData) { return; }
    setSimpleCallbackFunction();
    thisData = new MainWindowData;
    startTimer(888);
}

void MainWindow::timerEvent(QTimerEvent *) {
    updateGL();
}
