
#include <GL/glu.h>

#include "attitudewidget.h"
int xxx = 0;
AttitudeWidget::AttitudeWidget()
{
    yawAngle = 0;
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(updateGL()));
    timer->start(25);
}

void AttitudeWidget::initializeGL()
{
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,0);
}

void AttitudeWidget::resizeGL(int w, int h)
{
    glViewport(0,0,(GLint)w,(GLint)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0,(float)w/(float)h,0.1,140.0);
}

void AttitudeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,50,50,0,0,0,0,1,0);


    glPushMatrix();

    glRotatef(90,0,1,0);
    glRotatef(90,-1,0,0);

    glRotatef(yawAngle,0,0,1);
    glRotatef(pitchAngle,0,1,0);
    glRotatef(rollAngle,1,0,0);

//    xxx++;
//    if(xxx==2)
//    {
//        xxx=0;
//        yawAngle++;
//    }

    glColor3f(1,0,0);
    drawHalfMulticopter();
    glColor3f(1,1,1);
    renderText( 20,0,4,"Front");
    glColor3f(0,1,0);
    renderText(-20,0,4,"Back");

    glRotatef(90,0,0,1);
    glColor3f(0,0,1);
    drawHalfMulticopter();
    glColor3f(1,1,0);
    renderText( 20,0,4,"Left");
    renderText(-20,0,4,"Right");

    glPopMatrix();
}

void AttitudeWidget::drawHalfMulticopter()
{
    //绘制四轴的简单模型
    glBegin(GL_LINE_LOOP);
    glVertex3f(-20,-1,-1);
    glVertex3f(-20,-1, 1);
    glVertex3f(-20, 1, 1);
    glVertex3f(-20, 1,-1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(20,-1,-1);
    glVertex3f(20,-1, 1);
    glVertex3f(20, 1, 1);
    glVertex3f(20, 1,-1);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(-20,-1,-1);
    glVertex3f(20,-1,-1);
    glVertex3f(-20,-1,1);
    glVertex3f(20,-1,1);
    glVertex3f(-20,1,-1);
    glVertex3f(20,1,-1);
    glVertex3f(-20,1,1);
    glVertex3f(20,1,1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(-20,0,-1);
    glVertex3f(-15,0,-5);
    glVertex3f(-18,0,-1);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(20,0,-1);
    glVertex3f(15,0,-5);
    glVertex3f(18,0,-1);
    glEnd();
}
