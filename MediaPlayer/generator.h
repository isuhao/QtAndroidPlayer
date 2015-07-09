#ifndef GENERATOR_H
#define GENERATOR_H
#include <QByteArray>
#include <QIODevice>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>
using namespace std;
class MediaImpl;
class Generator : public QIODevice
{
    Q_OBJECT
public:
    Generator(MediaImpl*);

    ~Generator();
public:

    void start();
    void stop();

    qint64 readData(char *data, qint64 maxlen);

    qint64 writeData(const char *data, qint64 len);

    qint64 bytesAvailable() const;

private:
    qint64 m_pos;
    QByteArray m_buffer;
    MediaImpl *m_oMediaImpl;
};

#endif // GENERATOR_H
