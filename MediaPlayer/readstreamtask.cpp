#include "readstreamtask.h"
#include "libavinclude.h"
#include <QDebug>
ReadStreamTask::ReadStreamTask()
{
    video_index = -1;
    audio_index = -1;
    video_handler = 0;
    audio_handler = 0;
    FormatContext = 0;
    bExit = true;
}

ReadStreamTask::~ReadStreamTask()
{
    video_index = -1;
    audio_index = -1;
    video_handler = 0;
    audio_handler = 0;
    FormatContext = 0;
}

void ReadStreamTask::set_format_context(AVFormatContext *Context)
{
    FormatContext = Context;
}

void ReadStreamTask::set_video_index(int index)
{
    video_index = index;
}

void ReadStreamTask::set_audio_index(int index)
{
    audio_index = index;
}

void ReadStreamTask::set_video_handler(Receive_handler * handler)
{
    video_handler = handler;
}

void ReadStreamTask::set_audio_handler(Receive_handler * handler)
{
    audio_handler = handler;
}

int ReadStreamTask::close()
{
    bExit = true;
    wait();//等待线程退出
    return 0;
}

int ReadStreamTask::svc()
{
    bExit = false;
    AVPacket packet;
    av_init_packet(&packet);

    while(true)
    {
        int ret = av_read_frame(FormatContext, &packet);

        if(ret >= 0)
        {
           if(packet.stream_index==video_index)    //视频
           {
               if(video_handler &&(!bExit))
               {
                   //qDebug(QString("video_handler->receive_handler  :").append(QString::number(packet.pts)).toLocal8Bit().data());
                   video_handler->receive_handler((char*)&packet,sizeof(AVPacket));
               }


           }else if(packet.stream_index==audio_index)//音频
           {

               if(audio_handler &&(!bExit))
               {
                   //qDebug(QString("audio_handler->receive_handler   :").append(QString::number(packet.pts)).toLocal8Bit().data());
                   while(audio_handler->receive_handler((char*)&packet,sizeof(AVPacket)) == false)//如果满了的话 循环等待
                   {
                       if(bExit) break; //退出的话

                        usleep(20*1000);
                   }
                   // qDebug(QString("audio_handler->receive_handler1  :").toLocal8Bit().data());
               }
           }
        }else
        {
            if( bExit == false)
            {
                usleep(25*1000);
            }else
            {
                if(ret == AVERROR_EOF)
                {
                    qDebug(QString("ret == AVERROR_EOF").toLocal8Bit().data());
                     break;
                }
            }
        }
    }
    return 0;
}

