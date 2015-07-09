#ifndef MEDIAIMPL_H
#define MEDIAIMPL_H
class ReadStreamTask;
class VideoDecodeTask;
class SynchronousAV;
class AudioDecodeImpl;
class AVFormatContext;
class AVCodec;
class AVStream;
class AVCodecContext;
class MediaImagehandler;
class AudioPlay;
class MediaImpl
{
public:
    MediaImpl();
    ~MediaImpl();
public:
  bool open(const char*url);
  void close();
  void Init_image_handler(MediaImagehandler*);
public:
   long long  get_media_timer_len();         //获取媒体播放时间长度
public:
   unsigned int  read_media_audio_data(char *data, unsigned int len);//读取音频数据

public:
   double  get_audio_current_clock(); //获取媒体播放时间戳
public:
    unsigned int get_media_video_width();       //获取媒体视频宽度
    unsigned int get_media_video_height();      //获取媒体视频高度
    unsigned int get_sample_rate();             //获取音频比特率
    unsigned int get_channel_count();           //获取音频位宽

private:
    ReadStreamTask  *   m_oReadStreamTask;
    VideoDecodeTask *   m_oVideoDecodeTask;
    AudioDecodeImpl *   m_oAudioDecodeImpl;
    SynchronousAV   *   m_oSynchronousAV;
    AudioPlay       *   m_oAudioPlay;
private:
    AVFormatContext *pFormatCtx;
    AVStream        *AVStreamV;
    AVStream        *AVStreamA;
    AVCodecContext  *pCodecCtxV;
    AVCodecContext  *pCodecCtxA;
private:
    int             bExit;
};

#endif // MEDIAIMPL_H
