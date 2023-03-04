#ifndef PITCH_QT_LFT_CONSTANTS_HPP_INCLUDED
#define PITCH_QT_LFT_CONSTANTS_HPP_INCLUDED

#include <cstddef>
#include <vector>
#include <string>

constexpr int    DEFAULT_BITRATE          = 48000;
constexpr double DEFAULT_DATA_BUF_LEN_SEC = 4.00;
constexpr double DEFAULT_LOWEST_FREQ      = 27.50;
constexpr double DEFAULT_SIN_COUNT        = 16;
constexpr size_t DEFAULT_MIN_SIZE         = DEFAULT_BITRATE / 60; // use at least 60th of a second of data
#ifdef ANDROID
constexpr int    DEFAULT_OCTAVE_DIVS      = 48;
#else
constexpr int    DEFAULT_OCTAVE_DIVS      = 96;
#endif
constexpr int    DEFAULT_OCTAVES          = 8;

constexpr const char *DEFAULT_TRANSFORMER = "variable_fourier";
const char *DEFAULT_FILTERS[] = {
    "hypot",
    //"hypot",
    //"linearize",
#ifndef ANDROID
    "loudness", // loudnesser!
#endif
    //"loudness", // even loudnesser!!!
    "partial_normalize",
    //"normalize",
    nullptr
};

#endif // PITCH_QT_LFT_CONSTANTS_HPP_INCLUDED
