#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QtGui>
#include<QByteArray>
#include<QList>
#include<QVector>
#include<QTimer>
#include<QMouseEvent>

#include"serial/qextserialport.h"
#include"Graph/qcustomplot.h"
#include"Graph/attitudewidget.h"
#include"GlobalVariables.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void openMyCom();
    void closeMyCom();
    void setupRealtimeDataGraph();

private:
    QLabel *comLabel,*baudLabel,*parityLabel,*dataBitsLabel,*stopBitsLabel;
    QComboBox *comBox,*baudBox,*parityBox,*dataBitsBox,*stopBitsBox;
    QPushButton *openMyComBtn;
    QGroupBox *comGroup;

    QLabel *lockLabel;
    QGroupBox *statusGroup;

    QDockWidget *comWidget;
    QextSerialPort *myCom;
    QCustomPlot *DataPlot;
    AttitudeWidget *myAttitudeWidget;
    QTimer dataTimer;
    double key;

    QList<QByteArray> comData;
    QByteArray temp;
    bool OKtoReadH,OKtoReadM;

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void onMycomBtnClick();
    void readMycom();
    void realtimeDataSlot();
};

#endif // MAINWINDOW_H
