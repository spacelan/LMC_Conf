#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QTextEdit>
#include<QLabel>
#include<QPushButton>
#include<QComboBox>
#include<QToolButton>
#include<QMenuBar>
#include<QToolBar>
#include<QDockWidget>
#include<QFormLayout>
#include<QGridLayout>

#include"serial/win_qextserialport.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void openMyCom();
    void closeMyCom();

private:
    QLabel *comLabel;
    QComboBox *comBox;
    QPushButton *openMyComBtn;
    QWidget *widget;
    QDockWidget *comWidget;
    QTextEdit *textEdit;
    Win_QextSerialPort *myCom;

public slots:
    void onMycomBtnClick();
    void readMycom();
};

#endif // MAINWINDOW_H
