#include "mainwindow.h"
#include <QApplication>
#include "qDebug2Logcat.h"
#include <QDebug>
#include <QFile>
int main(int argc, char *argv[])
{
    installLogcatMessageHandler("yourLogTag");

    QApplication a(argc, argv);
    //加载样式表
    QFile loFile(":/qss/qss/style.qss");
    if(loFile.open(QFile::ReadOnly))
    {
       a.setStyleSheet(QString::fromUtf8(loFile.readAll()));
       loFile.close();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
