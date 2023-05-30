// AudioWorker.cpp
// ---------------
// Worker thread for reading audio and queueing it for processing.

#include "AudioWorker.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QThread>
#include <QMutexLocker>
#include <QDebug>
#pragma GCC diagnostic pop

#include "SharedAudioData.hpp"

constexpr QAudioFormat::SampleFormat SAMPLE_FORMAT = QAudioFormat::SampleFormat::Int16;
using SAMPLE_TYPE = int16_t;
constexpr size_t SAMPLE_TYPE_SIZE = sizeof(SAMPLE_TYPE);
constexpr double SAMPLE_DIVISOR = (1 << ((8 * SAMPLE_TYPE_SIZE) - 1));

// Macro for debugging so we can use __func__
#define DEBUG() \
    (qDebug().nospace() << "AudioWorker: " << __func__ << "(): ")

AudioWorker::AudioWorker(SharedAudioData &audioData, QObject *parent) :
    QObject(parent),
    _audioData(audioData)
{
}

void AudioWorker::onInputDeviceChanged(QAudioDevice device)
{
    // Exclusive access to _lftMutexData.
    DEBUG() << "audio input device updated";

    // Stop previous audio stream and sources and delete instances
    if (_audioInputStream) {
        DEBUG() << "closing previous audio input stream";
        _audioInputStream->close();
        _audioInputStream = nullptr;
    }
    if (_audioSource) {
        DEBUG() << "closing previous audio source";
        _audioSource->stop();
        _audioSource.reset();
    }

    // Reset data and clear the LFT display
    DEBUG() << "resetting lft signal data";
    _audioData.resetLftSignalData();
    emit audioRead();

    // If there's no device, we're done.
    if (device.isNull()) {
        DEBUG() << "no audio source";
        return;
    }

    DEBUG() << "audio source: " << device.description() << ", " << device.id();

    // Figure out which format to use
    QAudioFormat format = _audioData.getLftAudioFormat(SAMPLE_FORMAT);

    if (!device.isFormatSupported(format)) {
        DEBUG() << "raw audio format not supported by device";
        return;
    }

    // Create our audio source and start the input stream
    _audioSource = std::make_unique<QAudioSource>(device, format);

    // Use 1/20th of a second worth of data
    _audioSource->setBufferSize(format.sampleRate() / 20 * SAMPLE_TYPE_SIZE);
    _audioInputStream = _audioSource->start();

    // Listen!
    connect(_audioInputStream, &QIODevice::readyRead, this, &AudioWorker::onAudioInputStreamReadyRead);
}

void AudioWorker::onAudioInputStreamReadyRead()
{
    const size_t sampleBufSize = _audioSource->bufferSize();
    const double magnitude = _audioData.getMagnitude() / SAMPLE_DIVISOR;

    std::vector<char> bufVector;
    bufVector.resize(sampleBufSize);
    size_t totalBytesRead = 0, bytesRead;

    do {
        // Attempt to read the entire buffer available.
        char *bufData = bufVector.data();
        bytesRead = _audioInputStream->read(bufData, sampleBufSize);
        totalBytesRead += bytesRead;
        if (bytesRead == 0)
            break;

        // Convert from type 'SAMPLE_TYPE' to normalized 'double'.
        size_t dataLen = bytesRead / SAMPLE_TYPE_SIZE;

        const auto dataInt = reinterpret_cast<SAMPLE_TYPE *>(bufData);
        double dataFloat[dataLen];
        for (size_t i = 0; i < dataLen; ++i)
            dataFloat[i] = (double) dataInt[i] * magnitude;
        _audioData.pushToLft(dataFloat, dataLen);

        // Keep reading until we've read less than the full buffer.
    } while (bytesRead == sampleBufSize);

    if (totalBytesRead > 0)
        emit audioRead();
}