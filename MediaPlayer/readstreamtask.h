#ifndef READSTREAMTASK_H
#define READSTREAMTASK_H
#include <Task_Base/Task_Base.h>
#include <Receive_handler.h>
#include <Task_Base/CriticalSection.h>
class AVFormatContext;
class ReadStreamTask : public Task_Base
{
public:
    ReadStreamTask();
    ~ReadStreamTask();
public:
    void set_format_context(AVFormatContext *);
    void set_video_index(int);
    void set_audio_index(int);
    void set_video_handler(Receive_handler*);
    void set_audio_handler(Receive_handler*);
    virtual int close();
protected:
    virtual int svc();//主动工作线程函数(读取文件 网络IO流 包拆分到音视频对应队列)
private:
    int     video_index;
    int     audio_index;
    Receive_handler *video_handler;
    Receive_handler *audio_handler;
    AVFormatContext *FormatContext;
    bool             bExit;

};

#endif // READSTREAMTASK_H
