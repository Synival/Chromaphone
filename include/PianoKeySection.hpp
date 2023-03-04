#ifndef PITCH_QT_PIANOKEYSECTION_HPP_INCLUDED
#define PITCH_QT_PIANOKEYSECTION_HPP_INCLUDED

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QColor>
#include <QRect>
#pragma GCC diagnostic pop

#include "PianoConstants.hpp"

class PianoKey;

class PianoKeySection {
public:
    PianoKeySection(PianoKey &key, const sectionRange_t &range);

    inline double getVolume() const
        { return _volume; }
    inline QColor getColor() const
        { return _color; }
    inline QRect getDrawBox() const
        { return _drawBox; }

    void unstrike() const;
    void strike(double volume) const;
    QColor getDrawColor() const;

private:
    QColor createColor() const;
    QRect createDrawBox() const;

    PianoKey &_key;
    const sectionRange_t _range;
    const double _midiPitch;

    mutable QColor _color;
    mutable QRect _drawBox;

    mutable double _volume = 0;
};

#endif // PITCH_QT_PIANOKEYSECTION_HPP_INCLUDED
