#include "audioplay.h"
#include "generator.h"
#include "mediaimpl.h"

AudioPlay::AudioPlay(MediaImpl *Impl)
{
    m_generator = new Generator(Impl);

    m_generator->start();

    format      = new QAudioFormat;

    QString strD;
    strD.sprintf("setSampleRate  : %d  , %d ",Impl->get_sample_rate(),Impl->get_channel_count());
    qDebug(strD.toLocal8Bit().data());

    format->setSampleRate(Impl->get_sample_rate());
    format->setChannelCount(Impl->get_channel_count());
    format->setSampleSize(16);
    //pAVStream->codec->bits_per_coded_sample 这个设置有疑问 应该是16
    format->setCodec("audio/pcm");
    format->setByteOrder(QAudioFormat::LittleEndian);
    format->setSampleType(QAudioFormat::UnSignedInt);

    audio = new QAudioOutput( *format, 0);
    audio->start(m_generator);
}

AudioPlay::~AudioPlay()
{
    audio->stop();

    m_generator->stop();

    delete m_generator;

    delete format;

    delete audio;

}

void AudioPlay::start()
{

}

void AudioPlay::stop()
{

}
