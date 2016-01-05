#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){

}

void MainWindow::paintGL() {
    glClearColor((rand()%1000)/1000.0f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){
    startTimer(999);
}

void MainWindow::timerEvent(QTimerEvent *){
    updateGL();
}
