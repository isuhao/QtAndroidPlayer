#ifndef VIDEODECODETASK_H
#define VIDEODECODETASK_H
#include <Task_Base/Task_Base.h>
#include <Receive_handler.h>
#include <Task_Base/CriticalSection.h>
class AVStream;
class SwsContext;
class AVFrame;
class SynImagehandler;
class VideoDecodeTask : public Task_Base, public Receive_handler
{
public:
    VideoDecodeTask();
    ~VideoDecodeTask();
public:
    void set_video_stream(AVStream*);
    void set_video_handler(SynImagehandler*);
    virtual int close();
    virtual bool receive_handler(char *data,int size);
protected:
    virtual int svc();//主动工作线程函数 (视频解码工作)
private:
    double      synchronize_video(AVFrame *src_frame, double pts);
private:
    AVStream    *m_pVStream;

    double      video_clock;

    SwsContext  *img_convert_ctx;

    AVFrame     *pFrameRGB24;

    uint8_t     *bufferRGB ;

    SynImagehandler* syn_image_handler;

    bool             bExit;
};

#endif // VIDEODECODETASK_H
