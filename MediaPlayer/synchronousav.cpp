#include "synchronousav.h"
#include <libavinclude.h>
#include "audiodecodeimpl.h"
#include "mediaimagehandler.h"
#include "unistd.h"
#include <QDebug>
#define AV_SYNC_THRESHOLD   0.01
#define AV_NOSYNC_THRESHOLD 10.0
SynchronousAV::SynchronousAV()
{
    show_image_handler  = 0;

    m_audioImpl = 0;

    frame_last_delay = 40e-3;

    frame_last_pts = 0;

    frame_timer     = 0;
}

SynchronousAV::~SynchronousAV()
{

}

void SynchronousAV::set_image_handler(MediaImagehandler *handler)
{
    show_image_handler = handler;
}

void SynchronousAV::set_audio_decode_impl(AudioDecodeImpl *audio_impl)
{
    m_audioImpl = audio_impl;
}

void SynchronousAV::init()
{
    frame_timer     = (double)av_gettime() / 1000000.0;

    frame_last_delay = 40e-3;

    frame_last_pts = 0;
}

bool SynchronousAV::syn_image_handler(unsigned char *data, int size, double pts,int width, int height)//rgb 数据
{
    double actual_delay, delay, sync_threshold, ref_clock, diff;

    delay = pts - frame_last_pts;
    if (delay <= 0 || delay >= 1.0)  //
    {
        delay =  frame_last_delay;  //如果延迟大于一秒 或小于0秒 就用上一次的延迟（一般视频不能1s 一帧）
    }
    frame_last_delay = delay;	//保存这一次播放的延时
    frame_last_pts =  pts;      //保存这一次播放的视频时刻

    if(m_audioImpl == 0)
        return false;

    ref_clock = m_audioImpl->get_audio_clock();//获取音频时间戳
    diff = pts - ref_clock;			//算出这张图pts 与当前音频 的时刻配不配 差距过大 就丢弃
    sync_threshold = (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;//算出 延迟

    if (fabs(diff) < AV_NOSYNC_THRESHOLD)	//差距小于10秒
    {
        if (diff <= -sync_threshold)// 如果  diff 小于0  说明  音频播放太快
        {
            delay = 0;
        }
        else if (diff >= sync_threshold)//说明  视频频播放太快
        {
            delay = 2 * delay;
        }
    }else{qDebug(QString("VideoDecodeTask::svc() 0000 :").toLocal8Bit().data());
        return false;
    }

    // delay 就是此时  此帧图片 应该播放时的真实延时
     double timer = (av_gettime() / 1000000.0);

     //frame_timer += delay;
    // computer the REAL delay //计算实际延迟
    if (frame_timer + delay < timer)
    {
        frame_timer += delay;
        actual_delay = frame_timer - timer;//微秒 转换 秒

//        QString strD;
//        strD.sprintf("frame_timer <<<<<<.  : %5.7f  %5.7f  %5.7f  %5.7f  ",actual_delay,delay,frame_last_delay,diff);
//        qDebug(strD.toLocal8Bit().data());

        //qDebug(QString("actual_delay < 0... ").toLocal8Bit().data());
        return true;//目的是视频放慢了的话 就跳过
        //actual_delay = 0.01;
    }else
    {
        frame_timer += delay;
        actual_delay = frame_timer - timer;//微秒 转换 秒
    }

    if(delay == 0)
    {
        qDebug(QString("if(delay == 0)... ").toLocal8Bit().data());
        return true;
    }

    if(show_image_handler)
        show_image_handler->media_image_handler(data,size,width,height,ref_clock);

    usleep((actual_delay * 1000 + 0.5)*1000);//播放后延迟

    return true;
}
