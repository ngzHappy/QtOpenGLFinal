#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QGLWidget>

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void paintGL() override;
    void resizeGL(int w, int h)override;
    void initializeGL()override;
};

#endif // MAINWINDOW_HPP
