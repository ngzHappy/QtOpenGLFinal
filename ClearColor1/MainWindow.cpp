#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"
#include <cmath>
#include <QDateTime>
#include <QList>
#include <QString>

extern QList<QString> glslFileSearchPath;

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){

}

void MainWindow::timerEvent(QTimerEvent *){
    updateGL();
}

void MainWindow::paintGL() {
    const float currentTime = (QDateTime::currentMSecsSinceEpoch() & 65535)*0.0001f ;
    const float color_[]{ (std::sin(currentTime)+1)*0.5f,(1+std::cos(currentTime))*0.5f,0.67f, 1};
    glClearBufferfv(GL_COLOR,0,color_);
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){
    startTimer(588);
}


