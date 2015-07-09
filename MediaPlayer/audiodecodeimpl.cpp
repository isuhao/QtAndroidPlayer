#include "audiodecodeimpl.h"
#include <QDebug>
AudioDecodeImpl::AudioDecodeImpl():
    audio_clock(0),
    m_pAStream(0),
    audio_buf_size(0),
    audio_buf_index(0),
    au_convert_ctx(0)
{
    m_pFrame = avcodec_alloc_frame();

    out_buffer  =(uint8_t *)av_malloc(192000*2);
}

AudioDecodeImpl::~AudioDecodeImpl()
{
    if(m_pFrame)
    {
         av_free(m_pFrame);
         m_pFrame = 0;
    }

    if(au_convert_ctx)
    {
        swr_free(&au_convert_ctx);
        au_convert_ctx = 0;
    }

    if(out_buffer)
    {
        av_free(out_buffer);
        out_buffer = 0;
    }
}

void AudioDecodeImpl::set_audio_stream(AVStream *Stream)
{
    m_pAStream = Stream;

    if(m_pAStream)
    {
        qDebug(QString("while 111").toLocal8Bit().data());

        clear_list();//清理队列

        //Out Audio Param
        uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;//AV_CH_LAYOUT_STEREO;

        int out_nb_samples=m_pAStream->codec->frame_size;

        qDebug(QString("while 122").toLocal8Bit().data());
        AVSampleFormat out_sample_fmt= AV_SAMPLE_FMT_S16;

        int out_sample_rate=m_pAStream->codec->sample_rate;

        int out_channels=av_get_channel_layout_nb_channels(out_channel_layout);

        //Out Buffer Size
        out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);//BUF 大小

         qDebug(QString("while 222").toLocal8Bit().data());

        CCriticalAutoLock loAutoLock(oSwrContextSection);//au_convert_ctx锁住 防止音频播放线程重采样崩溃
        if(au_convert_ctx == 0)
        {
            qDebug(QString("while 333").toLocal8Bit().data());
            au_convert_ctx = swr_alloc();

            int64_t in_channel_layout=av_get_default_channel_layout(m_pAStream->codec->channels);

            au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,
                    in_channel_layout,m_pAStream->codec->sample_fmt , m_pAStream->codec->sample_rate,0, NULL);

            swr_init(au_convert_ctx);
        }
        qDebug(QString("while 444").toLocal8Bit().data());
    }else
    {
        qDebug(QString("while 555").toLocal8Bit().data());

        CCriticalAutoLock loAutoLock(oSwrContextSection); //au_convert_ctx锁住 防止音频播放线程重采样崩溃
        if(au_convert_ctx)
        {
            swr_free(&au_convert_ctx);
            au_convert_ctx = 0;
        }
    }

}

bool AudioDecodeImpl::receive_handler(char *data, int size)
{
    if(get_queue_size() < 50)
    {
        AVPacket pktl;
        memcpy(&pktl,data,size);  //取出数据
        push_audio_packet(pktl);
        return true;
    }
    return false;
}

unsigned int AudioDecodeImpl::read_data(char *data, unsigned int len)
{
    int lenOld = len;
    int len1, audio_size;
    while (len > 0)
    {
        if (audio_buf_index == audio_buf_size)
        {
            // We have already sent all our data; get more
            audio_size = audio_decode_frame();//继续解码处理 并 返回buf大小

            if (audio_size < 0)
            {
                // If error, output silence
                audio_buf_size = 1024;

                memset(out_buffer, 0 , audio_buf_size);
            }
            else
            {
                audio_buf_size = audio_size;
            }

            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;

        if (len1 > len)
            len1 = len;
        memcpy(data, (uint8_t *)out_buffer + audio_buf_index, len1);
        len -= len1;
        data += len1;
        audio_buf_index += len1;
    }
    return lenOld;
}

double AudioDecodeImpl::get_audio_clock()
{
    double pts;
    int hw_buf_size, bytes_per_sec, n;

    pts = audio_clock; // maintained in the audio thread
    hw_buf_size = audio_buf_size - audio_buf_index;
    bytes_per_sec = 0;
    n = m_pAStream->codec->channels * 2;
    if (m_pAStream)
    {
        bytes_per_sec = m_pAStream->codec->sample_rate * n;
    }
    if (bytes_per_sec)
    {
        pts -= (double)hw_buf_size / bytes_per_sec;
    }
    return pts;
}

int AudioDecodeImpl::get_queue_size()
{
    CCriticalAutoLock loAutoLock(oListSection);
    return m_AVPacketList.size();
}

void AudioDecodeImpl::clear_list()
{
    CCriticalAutoLock loAutoLock(oListSection);

    while(!m_AVPacketList.empty())
    {
        AVPacket aVpkt = m_AVPacketList.front();

        if(aVpkt.data)
            av_free_packet(&aVpkt);

        m_AVPacketList.pop();
    }
}

bool AudioDecodeImpl::pop_audio_packet(AVPacket &aVpkt1)
{
    CCriticalAutoLock loAutoLock(oListSection);

    if(!m_AVPacketList.empty())
    {
        aVpkt1 = m_AVPacketList.front();
        m_AVPacketList.pop();
        return  true;
    }else
    {
        return  false;
    }
}

void AudioDecodeImpl::push_audio_packet(AVPacket &aVpkt1)
{
    CCriticalAutoLock loAutoLock(oListSection);
    m_AVPacketList.push(aVpkt1);
}

int AudioDecodeImpl::audio_decode_frame()
{
    int ret, got_picture, n;

    AVPacket  pkt ;

    memset(&pkt,0,sizeof(AVPacket));

    for (;;)
    {
        while (pkt.size != 0)
        {
            got_picture = 0;

            ret = avcodec_decode_audio4(m_pAStream->codec, m_pFrame, &got_picture, &pkt);

            if ( ret < 0 )
            {
                int a= 0;
            }

            if ( got_picture > 0 )
            {
               if (pkt.data)
                    av_free_packet(&pkt); //崩溃的话 就注释 下试试

                memset(out_buffer, 0 , 192000*2);
                CCriticalAutoLock loAutoLock(oSwrContextSection);//au_convert_ctx锁住 防止音频播放线程重采样崩溃
                if(au_convert_ctx)
                    swr_convert(au_convert_ctx,&out_buffer, 192000,(const uint8_t **)m_pFrame->data , m_pFrame->nb_samples);
                else
                {
                    if (pkt.data)
                        av_free_packet(&pkt);
                     return -1;
                }

            }else
            {
                if (pkt.data)
                    av_free_packet(&pkt);
                continue;
            }

            n = 2 * m_pAStream->codec->channels;

            audio_clock += (double)out_buffer_size / (double)(n * m_pAStream->codec->sample_rate);

            return out_buffer_size;
        }

        if (pkt.data)
            av_free_packet(&pkt);

        bool bRet = pop_audio_packet(pkt);

        if(bRet == false)  //没数据包了
        {
            return -1;
        }

        if (pkt.pts != AV_NOPTS_VALUE)
        {
            audio_clock = av_q2d(m_pAStream->time_base) * pkt.pts;
        }
    }
}

