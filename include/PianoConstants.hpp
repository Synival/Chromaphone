#ifndef PITCH_QT_PIANOCONSTANTS_HPP_INCLUDED
#define PITCH_QT_PIANOCONSTANTS_HPP_INCLUDED

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QColor>
#pragma GCC diagnostic pop

// Constant settings - maybe configurable in the future?
constexpr int MAX_PITCHES = 96;
constexpr int MAX_PITCH_CLASSES = 12;

constexpr int LOW_PITCH = 21;
constexpr double LOW_PITCH_FREQ = 27.50;

constexpr int KEY_LOWER_WIDTH = 20;
constexpr int KEY_LOWER_HEIGHT = 128;
const QColor KEY_LOWER_COLOR = QColor(64, 64, 64);

constexpr int KEY_UPPER_WIDTH = 12;
constexpr int KEY_UPPER_HEIGHT = 80;
const QColor KEY_UPPER_COLOR = QColor(16, 16, 16);

constexpr int KEY_GAP = 2;
constexpr int KEYBOARD_BORDER = 1;

using sectionRange_t = std::pair<double /*low*/, double /*high*/>;

#endif // PITCH_QT_PIANOCONSTANTS_HPP_INCLUDED
