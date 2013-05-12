#include<QApplication>
#include<QCleanlooksStyle>
#include<QTextCodec>
#include"mainwindow.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    app.setStyle(new QCleanlooksStyle);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();

    return app.exec();
}
