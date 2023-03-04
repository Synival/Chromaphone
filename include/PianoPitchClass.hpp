// PianoPitchClass.hpp
// -------------------
// Data for a class of pitch (e.g, C, C#, D, etc.) for a piano.

#ifndef PITCH_QT_PIANOPITCHCLASS_HPP_INCLUDED
#define PITCH_QT_PIANOPITCHCLASS_HPP_INCLUDED

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QColor>
#pragma GCC diagnostic pop

#include "PianoConstants.hpp"

// Forward declarations
class Piano;

enum class PianoPitchClassPosition {
    Upper = 0,
    Lower = 1
};

// Data for a class of pitch (e.g, C, C#, D, etc.) for a piano.
class PianoPitchClass {
public:
    PianoPitchClass(
        Piano &piano, int pitchClassIndex, PianoPitchClassPosition pos, int width, QColor color, int offset = 0
    ) :
        _piano(piano),
        _pitchClassIndex(pitchClassIndex),
        _pos(pos),
        _width(width),
        _height((pos == PianoPitchClassPosition::Lower) ? KEY_LOWER_HEIGHT : KEY_UPPER_HEIGHT),
        _color(color),
        _offset(offset)
    {}

    inline int getPitchClassIndex() const
        { return _pitchClassIndex; }
    inline PianoPitchClassPosition getPosition() const
        { return _pos; }
    inline int getWidth() const
        { return _width; }
    inline int getHeight() const
        { return _height; }
    inline QColor getColor() const
        { return _color; }
    inline int getOffset() const
        { return _offset; }

    inline bool isLower() const
        { return _pos == PianoPitchClassPosition::Lower; }

private:
    Piano &_piano;
    const int _pitchClassIndex;
    const PianoPitchClassPosition _pos;
    const int _width;
    const int _height;
    const QColor _color;
    const int _offset;
};

#endif // PITCH_QT_PIANOPITCHCLASS_HPP_INCLUDED
