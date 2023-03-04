#include <QtDebug>

#include "../lft/api/signal_profile.hpp"
#include "../lft/api/factories.hpp"
#include "../lft/api/signal_data_doublebuf.hpp"
#include "../lft/api/log_transformer_filter_i.hpp"
#include "../lft/api/log_transformer_i.hpp"
#include "../lft/api/transform_results.hpp"
#include "../lft/api/log_transform_stats.hpp"

#include "LftConstants.hpp"
#include "Utils.hpp"

#include "Lft.hpp"

// Macro for debugging so we can use __func__
#define DEBUG() \
    (qDebug().nospace() << "Lft: " << __func__ << "(): ")

Lft::Lft() :
    _signalProfile(std::make_unique<signal_profile>(DEFAULT_BITRATE)),
    _signalData(
        std::make_unique<signal_data_doublebuf>(
            *_signalProfile, std::chrono::milliseconds((int) (DEFAULT_DATA_BUF_LEN_SEC * 1000.00))
        )
    ),
    _transformerFilters(createFilters(cstrArrayToVector(DEFAULT_FILTERS))),
    _transformer(
        factory_log_transformer {
            *_signalProfile,
            getLowestFreq(DEFAULT_LOWEST_FREQ, DEFAULT_OCTAVE_DIVS),
            DEFAULT_SIN_COUNT,
            DEFAULT_MIN_SIZE,
            DEFAULT_OCTAVE_DIVS,
            DEFAULT_OCTAVES
        }.create(DEFAULT_TRANSFORMER)
    ),
    _transformerStats(
        createTransformStats()
    )
{}

Lft::~Lft()
{}

QAudioFormat Lft::getAudioFormat(QAudioFormat::SampleFormat sampleFormat)
{
    QAudioFormat format {};
    format.setSampleRate(_signalProfile->get_bitrate());
    format.setChannelCount(1);
    format.setSampleFormat(sampleFormat);
    return format;
}

void Lft::resetSignalData()
    { _signalData->reset(); }

void Lft::push(const std::vector<double> &vals)
    { _signalData->push(vals); }

void Lft::push(const double *data, size_t len)
    { _signalData->push(data, len); }

std::unique_ptr<transform_results<double>> Lft::getResults()
{
    transform_results<double> results = _transformer->transform<double>(*_signalData);
    for (auto &filter : _transformerFilters)
        results = filter->filter_transform(results);
    return std::make_unique<transform_results<double>>(std::move(results));
};

log_transform_stats Lft::getStats()
{
    log_transform_stats stats = _transformer->get_stats();
    for (auto &filter : _transformerFilters)
        stats = filter->filter_stats(stats);
    return stats;
};

std::vector<std::unique_ptr<log_transformer_filter_i>> Lft::createFilters(const std::vector<std::string> filters)
{
    factory_log_transformer_filter factory {};
    std::vector<std::unique_ptr<log_transformer_filter_i>> newFilters {};

    size_t len = filters.size();
    newFilters.resize(len);
    for (size_t i = 0; i < len; ++i)
        newFilters[i] = factory.create(filters[i].c_str());

    return newFilters;
}

double Lft::getLowestFreq(double initialLowestFreq, int octaveDivs)
{
    double lowFreq = initialLowestFreq;
    for (auto &filter : _transformerFilters)
        lowFreq = filter->inverse_lowest_freq(lowFreq, octaveDivs);
    return lowFreq;
};

std::unique_ptr<log_transform_stats> Lft::createTransformStats()
{
    log_transform_stats stats = _transformer->get_stats();
    for (auto &filter : _transformerFilters)
        stats = filter->filter_stats(stats);
    return std::make_unique<log_transform_stats>(std::move(stats));
}