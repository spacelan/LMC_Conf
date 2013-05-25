#ifndef ATTITUDEWIDGET_H
#define ATTITUDEWIDGET_H

#include <QGLWidget>
#include <QTimer>

#include "GlobalVariables.h"

//显示四轴飞行器的当前姿态
class AttitudeWidget : public QGLWidget
{
    Q_OBJECT
public:
    AttitudeWidget();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void drawHalfMulticopter();

public:
    QTimer *timer;
    
public slots:

};

#endif // ATTITUDEWIDGET_H
