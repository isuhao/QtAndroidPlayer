#include "videodecodetask.h"
#include "libavinclude.h"
#include "synimagehandler.h"
#include <QDebug>
VideoDecodeTask::VideoDecodeTask():
        m_pVStream(0),
        video_clock(0),
        img_convert_ctx(0),
        pFrameRGB24(0),
        bufferRGB(0),
        syn_image_handler(0),
        bExit(true)
{
    pFrameRGB24 = avcodec_alloc_frame();
}

VideoDecodeTask::~VideoDecodeTask()
{
    syn_image_handler = 0;

    if(img_convert_ctx)
    {
        sws_freeContext(img_convert_ctx);

        img_convert_ctx = 0;
    }
    if(pFrameRGB24)
    {
        av_free(pFrameRGB24);

        pFrameRGB24 = 0;

    }
    if(bufferRGB)
    {
        free(bufferRGB);

        bufferRGB = 0;
    }
}

void VideoDecodeTask::set_video_stream(AVStream *Stream)
{
    m_pVStream = Stream;

    if(m_pVStream)
    {
        if(img_convert_ctx)
        {
            sws_freeContext(img_convert_ctx);

            img_convert_ctx = 0;
        }
        if(bufferRGB)
        {
            free(bufferRGB);

            bufferRGB = 0;
        }

        video_clock = 0;

        discard();


        int width   = m_pVStream->codec->width;
        int height  = m_pVStream->codec->height;

        QString strD;
        strD.sprintf("init  <<<<<<.  : %d  %d ",width,height);
        qDebug(strD.toLocal8Bit().data());

        img_convert_ctx = sws_getContext(width,height ,m_pVStream->codec->pix_fmt,width,height,//AV_PIX_FMT_YUV420P
                                         AV_PIX_FMT_RGB24 , SWS_FAST_BILINEAR, NULL, NULL, NULL);

         int numBytesRGB = avpicture_get_size(AV_PIX_FMT_RGB24, width,height);

         bufferRGB = (uint8_t *)malloc(numBytesRGB * sizeof(uint8_t));

         avpicture_fill((AVPicture *)pFrameRGB24, bufferRGB, AV_PIX_FMT_RGB24,width , height);
    }else
    {
        if(img_convert_ctx)
        {
            sws_freeContext(img_convert_ctx);

            img_convert_ctx = 0;
        }
        if(bufferRGB)
        {
            free(bufferRGB);

            bufferRGB = 0;
        }

        syn_image_handler = 0;
    }

}

void VideoDecodeTask::set_video_handler(SynImagehandler * handler)
{
    syn_image_handler = handler;
}

int VideoDecodeTask::close()
{
    bExit = true;
    std::string str("");
    int nthread  =thr_count();

    for (int i =0 ;i<nthread ;i++)
        putq(str);

    wait();
    return 0;
}

bool VideoDecodeTask::receive_handler(char *data, int size)
{
    putq(std::string(data,size));
    return true;
}
double VideoDecodeTask::synchronize_video(AVFrame * src_frame, double pts)
{
    double frame_delay;

    if (pts != 0)
    {
        // if we have pts, set video clock to it
        video_clock = pts;
    }
    else
    {
        // if we aren't given a pts, set it to the clock
        pts = video_clock;
    }
    // update the video clock
    frame_delay = av_q2d(m_pVStream->codec->time_base);
    // if we are repeating a frame, adjust clock accordingly
    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);

    video_clock += frame_delay;

    return pts;
}
int VideoDecodeTask::svc()
{
    double pts;

    int len1, frameFinished;

    bExit = false;

    AVFrame * pFrame = avcodec_alloc_frame();

    AVPacket pktl;
    memset(&pktl,0,sizeof(AVPacket));

    std::string str ="";

    int width   = m_pVStream->codec->width;
    int height  = m_pVStream->codec->height;

    unsigned char *pBuffer = new unsigned char[width*height*3];

    while(getq(str))
    {
        if (str=="")
        {
            break;
        }

        qDebug(QString("VideoDecodeTask::svc() 0 :").toLocal8Bit().data());

        memcpy(&pktl,str.data(),str.length());  //取出数据

        QString strD;
        strD.sprintf("AVPacket  : %d  %d  %lld  ",(int)str.length(),pktl.stream_index,pktl.pts);
        qDebug(strD.toLocal8Bit().data());

        frameFinished = 0;
        len1 = avcodec_decode_video2(m_pVStream->codec, pFrame, &frameFinished,&pktl);

        if (frameFinished)
        {
            //qDebug(QString("VideoDecodeTask::svc() 1 :").toLocal8Bit().data());
            pts = av_frame_get_best_effort_timestamp(pFrame);

            if (pts == AV_NOPTS_VALUE)
            {
                pts = 0;
            }

            pts *= av_q2d(m_pVStream->time_base);


            QString strD;
            strD.sprintf("svc  <<<<<<.  : %d  %d ",width,height);
            qDebug(strD.toLocal8Bit().data());

            pts = synchronize_video(pFrame, pts);
            //qDebug(QString("VideoDecodeTask::svc() 2 :").toLocal8Bit().data());
            //先不判断长度
            sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, height, pFrameRGB24->data, pFrameRGB24->linesize);
            //qDebug(QString("VideoDecodeTask::svc() 3 :").toLocal8Bit().data());
            memset(pBuffer,0,width*height*3);
            memcpy(pBuffer,pFrameRGB24->data[0],pFrameRGB24->linesize[0]*height);

            if(bExit)
            {
                break;
            }else
            {
                if(syn_image_handler)
                {
                     //qDebug(QString("pts  :").append(QString::number((long)pts)).toLocal8Bit().data());
                     syn_image_handler->syn_image_handler(pBuffer,pFrameRGB24->linesize[0]*m_pVStream->codec->height,pts,width,height);
                }
            }
        }
        av_free_packet(&pktl);
    }
    avcodec_free_frame(&pFrame);

    delete []pBuffer;

    return 1;
}

