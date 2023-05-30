// LftWorker.hpp
// -------------
// Worker thread for processing audio via LFT and queueing it for rendering.

#ifndef PITCH_QT_LFT_WORKER_HPP_INCLUDED
#define PITCH_QT_LFT_WORKER_HPP_INCLUDED

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QObject>
#pragma GCC diagnostic pop

class SharedAudioData;

class LftWorker : public QObject
{
    Q_OBJECT

public:
    explicit LftWorker(SharedAudioData &audioData, QObject *parent = nullptr);

public slots:
    void onAudioRead();
    void onReadyForNewLftResults();

signals:
    void lftResultsUpdated();

private:
    bool updateLftResultsIfNecessary();

    SharedAudioData &_audioData;
    bool _hasNewAudio = false;
    bool _newLftRequested = false;
};

#endif // PITCH_QT_LFT_WORKER_HPP_INCLUDED
