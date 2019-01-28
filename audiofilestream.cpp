#include "audiofilestream.h"

#include <QtMath>
#include <qendian.h>

AudioFileStream::AudioFileStream() :
    m_input(&m_data),
    m_output(&m_data),
    m_state(State::Stopped)
{
    setOpenMode(QIODevice::ReadOnly);

    isInited = false;
    isDecodingFinished = false;
}

// format - it is audio format to which we whant decode audio data
bool AudioFileStream::init(const QAudioFormat& format)
{
    m_format = format;

    switch (m_format.sampleSize()) {
    case 8:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;

    case 32:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            m_maxAmplitude = 0x7fffffff; // Kind of
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    m_decoder.setAudioFormat(m_format);

    connect(&m_decoder, SIGNAL(bufferReady()), this, SLOT(bufferReady()));
    connect(&m_decoder, SIGNAL(finished()), this, SLOT(finished()));

    // Initialize buffers
    if (!m_output.open(QIODevice::ReadOnly) || !m_input.open(QIODevice::WriteOnly))
    {
        return false;
    }

    isInited = true;

    return true;
}

// AudioOutput device (like speaker) call this function for get new audio data
qint64 AudioFileStream::readData(char* data, qint64 len)
{
    memset(data, 0, static_cast<size_t>(len));

    if (m_state == State::Playing)
    {
        m_output.read(data, len);

        // There is we send readed audio data via signal, for ability get audio signal for the who listen this signal.
        // Other word this emulate QAudioProbe behaviour for retrieve audio data which of sent to output device (speaker).
        if (len > 0)
        {
            QByteArray buff(data, static_cast<int>(len));
            emit newData(buff);
        }

        // Is finish of file
        if (atEnd())
        {
            stop();
        }
    }

    // TODO only do this stuff if the program wants the current output to be displayed
    if (m_maxAmplitude) {
        Q_ASSERT(m_format.sampleSize() % 8 == 0);
        const int channelBytes = m_format.sampleSize() / 8;
        const int sampleBytes = m_format.channelCount() * channelBytes;
        Q_ASSERT(len % sampleBytes == 0);
        const int numSamples = static_cast<int>(len / sampleBytes);

        quint32 maxValue = 0;
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

        for (int i = 0; i < numSamples; ++i) {
            for (int j = 0; j < m_format.channelCount(); ++j) {
                quint32 value = 0;

                if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    value = *reinterpret_cast<const quint8*>(ptr);
                } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    value = static_cast<quint32>(qAbs(*reinterpret_cast<const qint8*>(ptr)));
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint16>(ptr);
                    else
                        value = qFromBigEndian<quint16>(ptr);
                } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = static_cast<quint32>(qAbs(qFromLittleEndian<qint16>(ptr)));
                    else
                        value = static_cast<quint32>(qAbs(qFromBigEndian<qint16>(ptr)));
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = qFromLittleEndian<quint32>(ptr);
                    else
                        value = qFromBigEndian<quint32>(ptr);
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::SignedInt) {
                    if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                        value = static_cast<quint32>(qAbs(qFromLittleEndian<qint32>(ptr)));
                    else
                        value = static_cast<quint32>(qAbs(qFromBigEndian<qint32>(ptr)));
                } else if (m_format.sampleSize() == 32 && m_format.sampleType() == QAudioFormat::Float) {
                    value = static_cast<quint32>(qAbs(*reinterpret_cast<const float*>(ptr) * 0x7fffffff)); // assumes 0-1.0
                }

                maxValue = qMax(value, maxValue);
                ptr += channelBytes;
            }
        }

        maxValue = qMin(maxValue, m_maxAmplitude);
        m_level = qreal(maxValue) / m_maxAmplitude;

    }
    emit update();

    return len;
}

qint64 AudioFileStream::writeData(const char* data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

// Start play audio file
void AudioFileStream::play(QFile &file) {

    m_decoder.setSourceDevice(&file);
    m_decoder.start();

    m_state = State::Playing;
    emit stateChanged(m_state);
}

// Stop play audio file
void AudioFileStream::stop()
{
    clear();
    m_state = State::Stopped;
    emit stateChanged(m_state);
}

void AudioFileStream::clear()
{
    m_decoder.stop();
    //m_input.reset();
    if (isInited) m_input.seek(0);
    //m_output.reset();
    if (isInited) m_output.seek(0);
    m_data.clear();
    isDecodingFinished = false;
}

// Is finish of file
bool AudioFileStream::atEnd() const
{
    return m_output.size()
        && m_output.atEnd()
        && isDecodingFinished;
}

/////////////////////////////////////////////////////////////////////
// QAudioDecoder logic this methods responsible for decode audio file and put audio data to stream buffer

// Run when decode decoded some audio data
void AudioFileStream::bufferReady() // SLOT
{
    const QAudioBuffer &buffer = m_decoder.read();

    const int length = buffer.byteCount();
    const char *data = buffer.constData<char>();

    m_input.write(data, length);
}

// Run when decode finished decoding
void AudioFileStream::finished() // SLOT
{
    isDecodingFinished = true;
}
