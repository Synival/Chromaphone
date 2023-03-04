#ifndef PITCH_QT_PIANO_HPP_INCLUDED
#define PITCH_QT_PIANO_HPP_INCLUDED

#include <chrono>
#include <map>
#include <vector>

#include "PianoConstants.hpp"

#include "PianoPitchClass.hpp"
#include "PianoPitch.hpp"
#include "PianoKey.hpp"

class QPainter;
class log_transform_stats;

class Piano {
public:
    Piano();

    inline PianoPitchClass &getPitchClass(int index)
        { return _pitchClasses[index]; }
    inline const PianoPitchClass &getPitchClass(int index) const
        { return _pitchClasses[index]; }
    inline PianoPitch &getPitch(int index)
        { return _pitches[index]; }
    inline const PianoPitch &getPitch(int index) const
        { return _pitches[index]; }
    inline PianoKey &getKey(int index)
        { return _keys[index]; }
    inline const PianoKey &getKey(int index) const
        { return _keys[index]; }
    inline PianoKey &getLastKey()
        { return getKey(MAX_PITCHES - 1); }
    inline const PianoKey &getLastKey() const
        { return getKey(MAX_PITCHES - 1); }
    inline QColor getBackgroundColor() const
        { return _bgColor; }
    inline QColor getScaledBackgroundColor() const
        { return _bgScaledColor; }
    inline double getBackgroundColorIntensity() const
        { return _bgVolume; }

    void unstrikeKeys() const;
    void strikeKeysByFreq(double freq, double volume) const;
    void draw(QPainter &painter) const;
    void buildFreqToRangeMap(const log_transform_stats &stats) const;

private:
    std::vector<PianoPitchClass> makePitchClasses();
    std::vector<PianoPitch> makePitches();
    std::vector<PianoKey> makeKeys();
    void updateDimensions();
    void updateBackgroundColor() const;

    double getKeyIndexByFreq(double freq) const;

    inline sectionRange_t &getKeyRangeByFreq(double freq) const
        { return _freqToRangeMap.at(freq); }

    std::vector<PianoKey *> &convertRangeToKeys(const sectionRange_t &range) const;
    std::vector<PianoKey *> &getKeysByRange(const sectionRange_t &range);
    const std::vector<PianoKey *> &getKeysByRange(const sectionRange_t &range) const;

    std::vector<PianoPitchClass> _pitchClasses;
    std::vector<PianoPitch> _pitches;
    std::vector<PianoKey> _keys;

    int _width = 0;
    int _height = 0;
    mutable QColor _bgColor = Qt::black;
    mutable QColor _bgScaledColor = _bgColor;
    mutable double _bgVolume = 0.00;

    mutable std::map<double, double> _freqToKeyIndexMap {};
    mutable std::map<double, sectionRange_t> _freqToRangeMap {};
    mutable std::map<sectionRange_t, std::vector<PianoKey *>> _rangeToKeysMap {};
};

#endif // PITCH_QT_PIANO_HPP_INCLUDED