// SharedAudioData.hpp
// -------------------
// All audio data that is shared between threads, with thread-safe access.

#ifndef PITCH_QT_SHARED_AUDIO_DATA_HPP_INCLUDED
#define PITCH_QT_SHARED_AUDIO_DATA_HPP_INCLUDED

#include <atomic>
#include <memory>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QMutex>
#pragma GCC diagnostic pop

#include "../lft/api/transform_results.hpp"

#include "Lft.hpp"

class SharedAudioData {
public:
    void setMagnitude(double value)
        { _magnitude = value; }

    double getMagnitude()
        { return _magnitude; }
    transform_results<double> getLftResults();
    log_transform_stats getLftStats();

    void pushToLft(const double *data, size_t dataLen);
    void resetLftSignalData();
    void updateLftResults();
    QAudioFormat getLftAudioFormat(QAudioFormat::SampleFormat sampleFormat);

private:
    QMutex _lftLock {};
    Lft _lft {};

    QMutex _lftResultsLock {};
    std::unique_ptr<transform_results<double>> _lftResults {};

    std::atomic<double> _magnitude = 1.00;
};

#endif // PITCH_QT_SHARED_AUDIO_DATA_HPP_INCLUDED
