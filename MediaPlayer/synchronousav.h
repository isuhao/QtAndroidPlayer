#ifndef SYNCHRONOUSAV_H
#define SYNCHRONOUSAV_H

#include "synimagehandler.h"
class MediaImagehandler;
class AudioDecodeImpl;
class SynchronousAV : public SynImagehandler
{

public:
    SynchronousAV();
    ~SynchronousAV();
public:
    void set_image_handler(MediaImagehandler *);
    void set_audio_decode_impl(AudioDecodeImpl *);
    void init();
public:
    virtual bool syn_image_handler(unsigned char *data,int size,double pts,int width, int height);
private:
    AudioDecodeImpl *m_audioImpl;

    MediaImagehandler * show_image_handler;
private:
    double       frame_last_pts;     //记录上次图片的pts
    double       frame_last_delay;   //记录上次图片播放时 延时长度
    double       frame_timer;
};

#endif // SYNCHRONOUSAV_H
