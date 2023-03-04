#ifndef PITCH_QT_LFT_HPP_INCLUDED
#define PITCH_QT_LFT_HPP_INCLUDED

#include <memory>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QAudioFormat>
#pragma GCC diagnostic pop

class signal_profile;
class signal_data_i;
class log_transformer_filter_i;
class log_transformer_i;
class log_transform_stats;

template<typename T>
class transform_results;

class Lft
{
public:
    Lft();
    ~Lft();

    std::unique_ptr<transform_results<double>> getResults();
    log_transform_stats getStats();
    QAudioFormat getAudioFormat(QAudioFormat::SampleFormat sampleFormat);

    void resetSignalData();
    void push(const std::vector<double> &vals);
    void push(const double *data, size_t len);

private:
    std::vector<std::unique_ptr<log_transformer_filter_i>> createFilters(const std::vector<std::string> filters);

    double getLowestFreq(double initialLowestFreq, int octaveDivs);
    std::unique_ptr<log_transform_stats> createTransformStats();

    std::unique_ptr<signal_profile> _signalProfile;
    std::unique_ptr<signal_data_i> _signalData;
    std::vector<std::unique_ptr<log_transformer_filter_i>> _transformerFilters;
    std::unique_ptr<log_transformer_i> _transformer;
    std::unique_ptr<log_transform_stats> _transformerStats;
};

#endif // PITCH_QT_LFT_HPP_INCLUDED