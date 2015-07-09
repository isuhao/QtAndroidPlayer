#ifndef SHOWDRAWIMAGE_H
#define SHOWDRAWIMAGE_H

#include <QWidget>
#include <StreamBuf/StreamBuf.h>
#include "Task_Base/CriticalSection.h"
#include "mediaimagehandler.h"
class ShowDrawImage : public QWidget,public MediaImagehandler
{
    Q_OBJECT
public:
    explicit ShowDrawImage(QWidget *parent = 0);
    ~ShowDrawImage();
public:
    void set_audio_len(long long  audio_len);
public:
   virtual bool media_image_handler(unsigned char *data,int size,int width, int height,double audio_wsec);
protected:
    void paintEvent( QPaintEvent *event );
signals:

public slots:

private:
    CCriticalSection oCriSection;

    CStreamBuf  buf_show;
    QRect desktopRc;
    QSize image_size;

    double db_audio_wsec;
    long long  ll_audio_len;
};

#endif // SHOWDRAWIMAGE_H
