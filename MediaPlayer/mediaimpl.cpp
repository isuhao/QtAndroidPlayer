#include "mediaimpl.h"
#include "readstreamtask.h"
#include "videodecodetask.h"
#include "libavinclude.h"
#include "audiodecodeimpl.h"
#include "synchronousav.h"
#include "audioplay.h"
#include <QDebug>
MediaImpl::MediaImpl():
        pFormatCtx(0),
        AVStreamV(0),
        AVStreamA(0),
        pCodecCtxV(0),
        pCodecCtxA(0),
        m_oAudioPlay(0),
        bExit(true)
{
    av_register_all();
    avformat_network_init();

    m_oReadStreamTask   = new ReadStreamTask;
    m_oVideoDecodeTask  = new VideoDecodeTask;
    m_oAudioDecodeImpl  = new AudioDecodeImpl;
    m_oSynchronousAV    = new SynchronousAV;
}

MediaImpl::~MediaImpl()
{
    if(m_oReadStreamTask)
        delete m_oReadStreamTask;
    m_oReadStreamTask = 0;

    if(m_oVideoDecodeTask)
        delete m_oVideoDecodeTask;
    m_oVideoDecodeTask = 0;

    if(m_oAudioDecodeImpl)
        delete m_oAudioDecodeImpl;
    m_oAudioDecodeImpl = 0;

    if(m_oSynchronousAV)
        delete m_oSynchronousAV;
    m_oSynchronousAV = 0;

    avformat_network_deinit();

    av_close_input_file(pFormatCtx);
}

int decode_interrupt_cb(void *ctx)
{
    return *((int*)ctx);
}

bool MediaImpl::open(const char*url)
{
    if(pFormatCtx)
        return false;

    if(url == 0)
        return false;

    int err_code;

    bExit = false;

    avformat_network_init();

    pFormatCtx = avformat_alloc_context();

    pFormatCtx->interrupt_callback.callback = decode_interrupt_cb;

    pFormatCtx->interrupt_callback.opaque   = &bExit;

    qDebug(QString("while 33").toLocal8Bit().data());
    if ((err_code = avformat_open_input(&pFormatCtx, url, NULL, NULL)) != 0)
    {
        char buf[1024] ="";
        av_strerror(err_code, buf, 1024);
        avformat_close_input(&pFormatCtx);
        pFormatCtx = 0;
        return 0;
    }

    if((err_code = av_find_stream_info(pFormatCtx))<0)
    {
        avformat_close_input(&pFormatCtx);
        pFormatCtx = 0;
        return 0;
    }

    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            pCodecCtxV= pFormatCtx->streams[i]->codec;

            AVStreamV = pFormatCtx->streams[i];

            AVCodec  * pCodecV = avcodec_find_decoder(pCodecCtxV->codec_id);

            if (pCodecV && (avcodec_open2(pCodecCtxV, pCodecV,NULL) >= 0))
            {
                m_oReadStreamTask->set_video_index(i);
                qDebug(QString("m_oReadStreamTask->set_video_index(i); xxx :").append(QString::number(i)).toLocal8Bit().data());
            }
        }

        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO )
        {
            pCodecCtxA= pFormatCtx->streams[i]->codec;

            AVStreamA = pFormatCtx->streams[i];

            AVCodec    *pCodecA = avcodec_find_decoder(pCodecCtxA->codec_id);

            if (pCodecA && (avcodec_open2(pCodecCtxA, pCodecA,NULL) >= 0))
            {
               m_oReadStreamTask->set_audio_index(i);
               qDebug(QString("m_oReadStreamTask->set_audio_index(i); ooo :").append(QString::number(i)).toLocal8Bit().data());
            }
        }
    }

    qDebug(QString("while 55").toLocal8Bit().data());
    m_oAudioDecodeImpl->set_audio_stream(AVStreamA); //分配数据 清理队列

    qDebug(QString("while 66").toLocal8Bit().data());

    m_oVideoDecodeTask->set_video_stream(AVStreamV);//分配数据 清理队列
    m_oVideoDecodeTask->set_video_handler(m_oSynchronousAV);
    m_oVideoDecodeTask->activate(); //启动任务  解码 视频

    m_oSynchronousAV->set_audio_decode_impl(m_oAudioDecodeImpl);

    m_oSynchronousAV->init();

    m_oAudioPlay        = new AudioPlay(this);

    m_oReadStreamTask->set_format_context(pFormatCtx);
    m_oReadStreamTask->set_video_handler(m_oVideoDecodeTask);
    m_oReadStreamTask->set_audio_handler(m_oAudioDecodeImpl);
    m_oReadStreamTask->activate();   //启动媒体流 包 拆分线程


    qDebug(QString("while 88").toLocal8Bit().data());
    return true;
}

void MediaImpl::Init_image_handler(MediaImagehandler *handler)
{
    if(m_oSynchronousAV)
        m_oSynchronousAV->set_image_handler(handler);
}

unsigned int MediaImpl::read_media_audio_data(char *data, unsigned int len)
{
    if(bExit)
        return 0;
    return m_oAudioDecodeImpl->read_data(data,len);
}

long long MediaImpl::get_media_timer_len()
{
    if(pFormatCtx)
       return pFormatCtx->duration;

    return 0;
}

double MediaImpl::get_audio_current_clock()
{
    if(m_oAudioDecodeImpl)
        return m_oAudioDecodeImpl->get_audio_clock();
    return 0;
}

unsigned int MediaImpl::get_media_video_width()
{
    if(AVStreamV)
        return AVStreamV->codec->width;

    return 0;
}

unsigned int MediaImpl::get_media_video_height()
{
    if(AVStreamV)
        return AVStreamV->codec->height;

    return 0;
}

unsigned int MediaImpl::get_sample_rate()
{
    if(AVStreamA)
        return AVStreamA->codec->sample_rate;

    return 0;
}

unsigned int MediaImpl::get_channel_count()
{
    if(AVStreamA)
        return AVStreamA->codec->channels;

     return 0;
}

void MediaImpl::close()
{
    if(pFormatCtx == 0)
        return ;

    bExit = true;

    qDebug(QString("1 end").toLocal8Bit().data());

    m_oReadStreamTask->close();//等待线程退出

    qDebug(QString("2 end").toLocal8Bit().data());

    m_oVideoDecodeTask->close();//等待线程退出  必须先close关闭线程
    qDebug(QString("2.5 end").toLocal8Bit().data());
    m_oVideoDecodeTask->set_video_stream(0);// 再 清理分配数据

    if(m_oAudioPlay) { delete m_oAudioPlay; m_oAudioPlay = 0;}//先关音频播放

    m_oAudioDecodeImpl->set_audio_stream(0);//再清理视频解码相关 分配的数据

    qDebug(QString("3 end").toLocal8Bit().data());
    avcodec_close(pCodecCtxV);

    pCodecCtxV = NULL;

    avcodec_close(pCodecCtxA);

    pCodecCtxA = NULL;

    qDebug(QString("ret == end").toLocal8Bit().data());

    //av_close_input_file(pFormatCtx);

    avformat_close_input(&pFormatCtx);

    avformat_free_context(pFormatCtx);

    avformat_network_deinit();
    pFormatCtx = 0;
}

