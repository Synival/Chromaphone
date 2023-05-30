// LftWorker.cpp
// -------------
// Worker thread for processing audio via LFT and queueing it for rendering.

#include "LftWorker.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QThread>
#pragma GCC diagnostic pop

#include "SharedAudioData.hpp"

LftWorker::LftWorker(SharedAudioData &audioData, QObject *parent) :
    QObject(parent),
    _audioData(audioData)
{
}

void LftWorker::onAudioRead()
{
    if (_hasNewAudio)
        return;
    _hasNewAudio = true;
    updateLftResultsIfNecessary();
}

void LftWorker::onReadyForNewLftResults()
{
    if (_newLftRequested)
        return;
    _newLftRequested = true;
    updateLftResultsIfNecessary();
}

bool LftWorker::updateLftResultsIfNecessary()
{
    // Only perform an update when
    //   1) we have new audio data, and
    //   2) new LFT results have been requested.
    if (!(_hasNewAudio && _newLftRequested))
        return false;

    _hasNewAudio = false;
    _newLftRequested = false;

    _audioData.updateLftResults();
    emit lftResultsUpdated();

    return true;
}