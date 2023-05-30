// SharedAudioData.cpp
// -------------------
// All audio data that is shared between threads, with thread-safe access.

#include "SharedAudioData.hpp"

#include "../lft/api/transform_results.hpp"
#include "../lft/api/log_transform_stats.hpp"

transform_results<double> SharedAudioData::getLftResults()
{
    QMutexLocker lock { &_lftResultsLock };
    return *_lftResults.get();
}

log_transform_stats SharedAudioData::getLftStats()
{
    QMutexLocker lock { &_lftLock };
    return _lft.getStats();
}

void SharedAudioData::pushToLft(const double *data, size_t dataLen)
{
    QMutexLocker lock { &_lftLock };
    _lft.push(data, dataLen);
}

void SharedAudioData::resetLftSignalData()
{
    QMutexLocker lock { &_lftLock };
    _lft.resetSignalData();
}

void SharedAudioData::updateLftResults()
{
    std::unique_ptr<transform_results<double>> results;
    {
        QMutexLocker lftLock { &_lftLock };
        results = _lft.getResults();
    }

    {
        QMutexLocker lftResultsLock { &_lftResultsLock };
        _lftResults = std::move(results);
    }
}

QAudioFormat SharedAudioData::getLftAudioFormat(QAudioFormat::SampleFormat sampleFormat)
{
    QMutexLocker lock { &_lftLock };
    return _lft.getAudioFormat(sampleFormat);
}