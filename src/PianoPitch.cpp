// PianoPitch.hpp
// --------------
// Frequency data for a pitch contained in a Piano. Used to construct PianoKey's.

#include "PianoPitch.hpp"

#include "Piano.hpp"

PianoPitchClass &PianoPitch::getPitchClass()
    { return _piano.getPitchClass(_pitchClassIndex); }
const PianoPitchClass &PianoPitch::getPitchClass() const
    { return _piano.getPitchClass(_pitchClassIndex); }
