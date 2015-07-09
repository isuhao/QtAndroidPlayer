#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <ClientImpl/Receive_handler.h>
namespace Ui {
class MainWindow;
}
class QAudioFormat;
class QAudioOutput;
class Generator;
class MediaImpl;
class AudioPlay;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
public:

protected:
    void paintEvent( QPaintEvent *event );
     void closeEvent(QCloseEvent *event);
private slots:

     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

     void timerDone();
     void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    AudioPlay *audio_play;

    MediaImpl *m_oMediaImpl;

    QTimer *timer;
};

#endif // MAINWINDOW_H
