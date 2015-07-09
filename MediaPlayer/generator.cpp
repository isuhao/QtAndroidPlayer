#include "generator.h"
#include <qmath.h>
#include <QFile>
#include <qendian.h>
#include "mediaimpl.h"
Generator::Generator(MediaImpl *Impl)
 :  m_pos(0),
 m_oMediaImpl(Impl)
{

}

Generator::~Generator()
{
}

void Generator::start()
{
    open(QIODevice::ReadOnly);
}

void Generator::stop()
{
    m_pos = 0;
    close();
}

qint64 Generator::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

qint64 Generator::readData(char *data, qint64 len)
{
    if(m_oMediaImpl)
        return m_oMediaImpl->read_media_audio_data(data,(unsigned int)len);
    return 0;
}

qint64 Generator::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

