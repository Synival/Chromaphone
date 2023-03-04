#ifndef PITCH_QT_PIANOKEY_HPP_INCLUDED
#define PITCH_QT_PIANOKEY_HPP_INCLUDED

#include <map>

#include "PianoConstants.hpp"

#include "PianoPitch.hpp"
#include "PianoPitchClass.hpp"
#include "PianoKeySection.hpp"

class QPainter;
class QRect;
class Piano;
class PianoPitch;
class PianoPitchClass;
class PianoKeySection;

class PianoKey {
public:
    PianoKey(Piano &piano, int index, int x) :
        _piano(piano),
        _keyIndex(index),
        _pitchIndex(index),
        _x(x),
        _color(createColor())
    {}

    inline bool isFirstKey() const
        { return _keyIndex == 0; }
    inline bool isLastKey() const
        { return _keyIndex == MAX_PITCHES - 1; }

    inline QColor getColor() const
        { return _color; }
    inline int getKeyIndex() const
        { return _keyIndex; }
    inline int getPitchIndex() const
        { return _pitchIndex; }
    inline int getX() const
        { return _x; }
    inline std::map<sectionRange_t, PianoKeySection> &getSectionMap()
        { return _sectionMap; }
    inline const std::map<sectionRange_t, PianoKeySection> &getSectionMap() const
        { return _sectionMap; }

    inline PianoPitchClass &getPitchClass()
        { return getPitch().getPitchClass(); }
    inline const PianoPitchClass &getPitchClass() const
        { return getPitch().getPitchClass(); }
    inline PianoPitchClass &getPrevPitchClass()
        { return getPrevKey().getPitchClass(); }
    inline const PianoPitchClass &getPrevPitchClass() const
        { return getPrevKey().getPitchClass(); }
    inline PianoPitchClass &getNextPitchClass()
        { return getNextKey().getPitchClass(); }
    inline const PianoPitchClass &getNextPitchClass() const
        { return getNextKey().getPitchClass(); }

    PianoPitch &getPitch();
    const PianoPitch &getPitch() const;
    PianoKey &getPrevKey();
    const PianoKey &getPrevKey() const;
    PianoKey &getNextKey();
    const PianoKey &getNextKey() const;
    PianoKeySection &getSectionByRange(const sectionRange_t &range);
    const PianoKeySection &getSectionByRange(const sectionRange_t &range) const;
    QRect getDrawBox() const;

    void clearSections() const;
    void unstrike() const;
    void strike(const sectionRange_t &range, double volume) const;
    void draw(QPainter &painter) const;

private:
    PianoKeySection &convertRangeToSection(const sectionRange_t &range) const;
    QColor createColor() const;

    Piano &_piano;
    const int _keyIndex;
    const int _pitchIndex;
    const int _x;
    const QColor _color;

    mutable std::map<sectionRange_t, PianoKeySection> _sectionMap {};
};

#endif // PITCH_QT_PIANOKEY_HPP_INCLUDED
