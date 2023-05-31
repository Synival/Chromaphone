// PianoConstants.hpp
// ------------------
// Default parameters for the Piano class and its child classes.

#ifndef PITCH_QT_PIANO_CONSTANTS_HPP_INCLUDED
#define PITCH_QT_PIANO_CONSTANTS_HPP_INCLUDED

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

// 0: Pretty rainbow along circle of fifths
// 1: Pretty rainbow chromatically
// 2: Pretty blue
#define PIANO_KEY_COLOR_MODE 0

// 0: Standard piano keyboard layout
// 1: Wholestone keyboard layout
// 2: Single-tier keyboard layout
#define PIANO_KEY_LAYOUT 0

// Enable to show "out-of-tune" notes as white
#define PIANO_KEY_DESATURATE_NON_EQUAL_TEMPERMENT_TONES

using sectionRange_t = std::pair<double /*low*/, double /*high*/>;

#endif // PITCH_QT_PIANO_CONSTANTS_HPP_INCLUDED
