#ifndef AUDIOTASK_H
#define AUDIOTASK_H
using namespace std;

class QAudioFormat;
class QAudioOutput;
class Generator;
class MediaImpl;
class AudioPlay
{
public:
    AudioPlay(MediaImpl*);
    ~AudioPlay();
public:
    void start();
    void stop();
private:
    QAudioFormat *format;

    QAudioOutput *audio;

    Generator  *m_generator;
};

#endif // AUDIOTASK_H
