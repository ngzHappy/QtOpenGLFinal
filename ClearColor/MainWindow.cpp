#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"
#include <QString>
#include <QList>

extern QList<QString> glslFileSearchPath;

class MainWindow::MainWindowData {};

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){
    delete thisData;
}

void MainWindow::paintGL() {
    glClearColor((rand()%1000)/1000.0f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){
    if (thisData) { return; }
    setSimpleCallbackFunction();
    thisData=new MainWindowData;
    startTimer(999);
}

void MainWindow::timerEvent(QTimerEvent *){
    updateGL();
}
