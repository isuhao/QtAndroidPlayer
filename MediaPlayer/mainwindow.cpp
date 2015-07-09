#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include "audioplay.h"
#include "mediaimpl.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(timerDone()) );
    //timer->start(40);

    m_oMediaImpl = new MediaImpl;
    m_oMediaImpl->Init_image_handler(ui->DrawImage);
    ui->lineEdit->setText("rtmp://182.92.80.26:1935/live/livestream live=1");
    //ui->lineEdit->setText("rtmp://182.92.177.100/live/livestream live=1");
    ui->lineEdit_2->setText("/sdcard/cuc_ieschool.flv");
    ui->lineEdit_3->setText("rtmp://ftv.sun0769.com/dgrtv1/mp4:b1");

}

MainWindow::~MainWindow()
{
    delete m_oMediaImpl;

    delete ui;
}

void MainWindow::paintEvent( QPaintEvent *event )
{
    QMainWindow::paintEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_oMediaImpl->close();

    QMainWindow::closeEvent(event);
}

void MainWindow::timerDone()
{
     update();
}

void MainWindow::on_pushButton_clicked()
{
    qDebug(QString("while 00").toLocal8Bit().data());
    m_oMediaImpl->close();
    qDebug(QString("while 11").toLocal8Bit().data());
    usleep(100000);
    bool bl = m_oMediaImpl->open(ui->lineEdit->text().toLocal8Bit().data());
    if(bl)
    {
        ui->DrawImage->set_audio_len(m_oMediaImpl->get_media_timer_len());
        qDebug(QString("while 22").toLocal8Bit().data());
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug(QString("while 00").toLocal8Bit().data());
    m_oMediaImpl->close();
    qDebug(QString("while 11").toLocal8Bit().data());
    usleep(100000);
    bool bl = m_oMediaImpl->open(ui->lineEdit_2->text().toLocal8Bit().data());
    if(bl)
    {
        ui->DrawImage->set_audio_len(m_oMediaImpl->get_media_timer_len());
        qDebug(QString("while 22").toLocal8Bit().data());
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug(QString("while 00").toLocal8Bit().data());
    m_oMediaImpl->close();
    qDebug(QString("while 11").toLocal8Bit().data());
    usleep(100000);
    bool bl = m_oMediaImpl->open(ui->lineEdit_3->text().toLocal8Bit().data());
    if(bl)
    {
        ui->DrawImage->set_audio_len(m_oMediaImpl->get_media_timer_len());
        qDebug(QString("while 22").toLocal8Bit().data());
    }
}
