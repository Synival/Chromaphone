#ifndef PITCH_QT_PIANOPITCH_HPP_INCLUDED
#define PITCH_QT_PIANOPITCH_HPP_INCLUDED

#include "PianoConstants.hpp"

class Piano;
class PianoPitchClass;

class PianoPitch {
public:
    PianoPitch(Piano &piano, int pitchIndex) :
        _piano(piano),
        _pitchIndex(pitchIndex),
        _midiPitch(pitchIndex + LOW_PITCH),
        _pitchClassIndex(_midiPitch % MAX_PITCH_CLASSES),
        _freq(std::pow(2.00, (double) _pitchIndex / (double) MAX_PITCH_CLASSES) * LOW_PITCH_FREQ),
        _dim(((double) _midiPitch / (double) MAX_PITCHES) * 0.125 + 0.125)
    {}

    inline int getMidiPitch() const
        { return _midiPitch; }
    inline double getFreq() const
        { return _freq; }

    PianoPitchClass &getPitchClass();
    const PianoPitchClass &getPitchClass() const;

private:
    Piano &_piano;
    [[maybe_unused]] const int _pitchIndex;
    const int _midiPitch;
    const int _pitchClassIndex;
    const double _freq;
    [[maybe_unused]] const double _dim;
};

#endif // PITCH_QT_PIANOPITCH_HPP_INCLUDED
