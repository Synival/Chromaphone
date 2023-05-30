// AudioWorker.hpp
// ---------------
// Worker thread for reading audio and queueing it for processing.

#ifndef PITCH_QT_AUDIO_WORKER_HPP_INCLUDED
#define PITCH_QT_AUDIO_WORKER_HPP_INCLUDED

#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QObject>
#include <QAudioSource>
#pragma GCC diagnostic pop

class SharedAudioData;
class QMutex;
class Lft;

class AudioWorker : public QObject
{
    Q_OBJECT

public:
    explicit AudioWorker(SharedAudioData &audioData, QObject *parent = nullptr);

public slots:
    void onAudioInputStreamReadyRead();
    void onInputDeviceChanged(QAudioDevice device);

signals:
    void audioRead();

private:
    SharedAudioData &_audioData;
    std::unique_ptr<QAudioSource> _audioSource {};
    QIODevice *_audioInputStream = nullptr;
};

#endif // PITCH_QT_AUDIO_WORKER_HPP_INCLUDED
