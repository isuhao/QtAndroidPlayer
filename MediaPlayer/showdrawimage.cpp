#include "showdrawimage.h"
#include <QPainter>
#include <QDesktopWidget>
ShowDrawImage::ShowDrawImage(QWidget *parent) :
    QWidget(parent),
    buf_show(0,0),
    image_size(0,0),
    db_audio_wsec(0),
    ll_audio_len(0)
{

}

ShowDrawImage::~ShowDrawImage()
{

}

void ShowDrawImage::set_audio_len(long long audio_len)
{
     CCriticalAutoLock loAutoLock(oCriSection);
    ll_audio_len = audio_len;
}

bool ShowDrawImage::media_image_handler(unsigned char *data, int size, int width, int height,double audio_wsec)
{
    QString strD;
    strD.sprintf("show  <<<<<<.  : %d  %d  %d  %d ",width,height,(int)data,size);
    qDebug(strD.toLocal8Bit().data());

    {
        CCriticalAutoLock *ploAutoLock = new CCriticalAutoLock(oCriSection);
        buf_show = CStreamBuf(data,size);
        image_size = QSize(width,height);
        db_audio_wsec = audio_wsec;
        delete ploAutoLock;
    }
    update();
    return true;
}

void ShowDrawImage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter( this );

   // painter.setPen(QColor(220,230,240,200));
    desktopRc = rect();
    //QLinearGradient linearGradient(0, 0, rcshow.width(), 0);
    //linearGradient.setColorAt(0.2, Qt::white);
    //linearGradient.setColorAt(0.6, Qt::green);
    //linearGradient.setColorAt(1.0, Qt::black);
    //linearGradient.setColorAt(0, QColor(255, 0, 0, 120));
    //linearGradient.setColorAt(0.5, QColor(0, 255, 0, 100));
    //linearGradient.setColorAt(1, QColor(0, 255, 255, 100));
    //painter.setBrush(QBrush(linearGradient));

    //3. 指定渐变区域以外的区域的扩散方式
    //QGradient::PadSpread 使用最接近的颜色进行填充(....y, y, r, g, g....)
    //QGradient::ReflectSpread 在渐变区域以外的区域反射渐变（y, r, g, g, r, y）
    //QGradient::RepeatSpread 在渐变区域以外的区域重复渐变(y, r, g, y, r, g)
    //linearGradient.setSpread(QGradient::RepeatSpread);
    bool bDraw = false;

    double  ratio_duration = 0;

    QRect rcshow = QRect(0,0,0,0);

    {
        qDebug(QString("QPainter painter").toLocal8Bit().data());
        CCriticalAutoLock loAutoLock(oCriSection);
        rcshow = QRect(QPoint(0,0),image_size);
        rcshow.moveCenter(desktopRc.center());

        if((rcshow.width()== 0)||(rcshow.height()==0))
        {
            return ;
        }

        unsigned char* ptr = (unsigned char*)buf_show.Buf();
        if((buf_show.Len() >0)&&(ptr))
        {
            QImage img =(QImage(ptr,rcshow.width(),rcshow.height(),QImage::Format_RGB888));
            if(!img.isNull())
            {
                bDraw = true;
                painter.drawImage(rcshow,/*QPixmap::fromImage*/img);
            }

        }

        if(ll_audio_len >0 )
            ratio_duration = db_audio_wsec/ll_audio_len*1000000;

    }
    if(bDraw == false)
    {
        QString strD;
        strD.sprintf("paintEvent  <<<<<<.  : %d  %d ",rcshow.width(),rcshow.height());
        qDebug(strD.toLocal8Bit().data());
    }


    if(ratio_duration > 0)
    {
        QRect rc_progress_bar_draw = QRect(rcshow.left(),rcshow.bottom()-20,rcshow.width(),15);
        QRect rc_progress_bar_fill = QRect(rcshow.left(),rcshow.bottom()-20,rcshow.width()*ratio_duration,15);

        painter.drawRect(rc_progress_bar_draw);
        painter.fillRect(rc_progress_bar_fill,QBrush(QColor(255,0,0,128)));
    }
}

