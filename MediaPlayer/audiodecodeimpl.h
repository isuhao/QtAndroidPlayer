#ifndef AUDIODECODEIMPL_H
#define AUDIODECODEIMPL_H
#include <Receive_handler.h>
#include <Task_Base/CriticalSection.h>
#include <libavinclude.h>
#include <queue>
using namespace std;
class AVStream;
class AudioDecodeImpl:  public Receive_handler
{
public:
    AudioDecodeImpl();
    ~AudioDecodeImpl();
public:
    void set_audio_stream(AVStream*);
    virtual bool receive_handler(char *data,int size);
public:
    unsigned int read_data(char *data, unsigned int maxlen);
    double get_audio_clock();

private:
    int get_queue_size();
    bool pop_audio_packet(AVPacket &);
    void push_audio_packet(AVPacket &);
    int audio_decode_frame();
    void clear_list();
private:
    AVStream    *m_pAStream;

    CCriticalSection oListSection;
    std::queue<AVPacket>  m_AVPacketList;

    AVFrame         *m_pFrame;

    CCriticalSection oSwrContextSection;
    struct SwrContext *au_convert_ctx;

    uint8_t          *out_buffer;

    int              out_buffer_size;

    unsigned int     audio_buf_size;

    unsigned int     audio_buf_index;

    double           audio_clock;

};

#endif // AUDIODECODEIMPL_H
