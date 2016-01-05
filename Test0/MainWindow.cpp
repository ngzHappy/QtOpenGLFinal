#include <ZGL/QGLTool.hpp>
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent){
    qGLWidgetInitializeGlew(this);
}

MainWindow::~MainWindow(){

}


void MainWindow::paintGL() {
    const static float color_[]{(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,(rand()%1000)/1000.0f,1};
    glClearBufferfv(GL_COLOR,0,color_);
}

void MainWindow::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}

void MainWindow::initializeGL(){

}


