// PianoPitch.hpp
// --------------
// Frequency data for a pitch contained in a Piano. Used to construct PianoKey's.

#ifndef PITCH_QT_PIANOPITCH_HPP_INCLUDED
#define PITCH_QT_PIANOPITCH_HPP_INCLUDED

#include "PianoConstants.hpp"

// Forward declarations
class Piano;
class PianoPitchClass;

// Frequency and MIDI data which corresponds to an individual key on a piano.
class PianoPitch {
public:
    PianoPitch(Piano &piano, int pitchIndex) :
        _piano(piano),
        _pitchIndex(pitchIndex),
        _midiPitch(pitchIndex + LOW_PITCH),
        _pitchClassIndex(_midiPitch % MAX_PITCH_CLASSES),
        _freq(std::pow(2.00, (double) _pitchIndex / (double) MAX_PITCH_CLASSES) * LOW_PITCH_FREQ)
    {}

    inline int getMidiPitch() const
        { return _midiPitch; }
    inline double getFreq() const
        { return _freq; }

    PianoPitchClass &getPitchClass();
    const PianoPitchClass &getPitchClass() const;

private:
    Piano &_piano;
    const int _pitchIndex;
    const int _midiPitch;
    const int _pitchClassIndex;
    const double _freq;
};

#endif // PITCH_QT_PIANOPITCH_HPP_INCLUDED
