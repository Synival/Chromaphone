#include <cmath>
#include <algorithm>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QPainter>
//#include <QPainterPath>
//#include <QColor>
//#include <QBrush>
#pragma GCC diagnostic pop

#include "../lft/api/log_transform_stats.hpp"

#include "PianoPitchClass.hpp"
#include "PianoKey.hpp"
#include "Utils.hpp"

#include "Piano.hpp"

Piano::Piano() :
    _pitchClasses(makePitchClasses()),
    _pitches(makePitches()),
    _keys(makeKeys())
{
    updateDimensions();
}

std::vector<PianoPitchClass> Piano::makePitchClasses()
{
    constexpr PianoPitchClassPosition upper = PianoPitchClassPosition::Upper;
    constexpr PianoPitchClassPosition lower = PianoPitchClassPosition::Lower;

    // TODO: make this an option!!
#define PIANO_MODE 0
#if PIANO_MODE == 0
    // Standard piano keyboard
    const static PianoPitchClass _defaultPitches[MAX_PITCH_CLASSES] = {
        { *this,  0, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // C
        { *this,  1, upper, KEY_UPPER_WIDTH, KEY_UPPER_COLOR, -1 }, // C#
        { *this,  2, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // D
        { *this,  3, upper, KEY_UPPER_WIDTH, KEY_UPPER_COLOR, 1 },  // Eb
        { *this,  4, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // E
        { *this,  5, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // F
        { *this,  6, upper, KEY_UPPER_WIDTH, KEY_UPPER_COLOR, -1 }, // F#
        { *this,  7, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // G
        { *this,  8, upper, KEY_UPPER_WIDTH, KEY_UPPER_COLOR, 0 },  // Ab
        { *this,  9, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // A
        { *this, 10, upper, KEY_UPPER_WIDTH, KEY_UPPER_COLOR, 1 },  // Bb
        { *this, 11, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // B
    };
#elif PIANO_MODE == 1
    // Wholetone piano keyboard
    const static PianoPitchClass _defaultPitches[MAX_PITCH_CLASSES] = {
        { *this,  0, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // C
        { *this,  1, upper, KEY_UPPER_WIDTH, KEY_UPPER_COLOR, 0 },  // C#
        { *this,  2, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // D
        { *this,  3, upper, KEY_UPPER_WIDTH, KEY_UPPER_COLOR, 0 },  // Eb
        { *this,  4, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR },     // E
        { *this,  5, upper, KEY_UPPER_WIDTH, KEY_LOWER_COLOR, 0 },  // F
        { *this,  6, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR },     // F#
        { *this,  7, upper, KEY_UPPER_WIDTH, KEY_LOWER_COLOR, 0 },  // G
        { *this,  8, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR },     // Ab
        { *this,  9, upper, KEY_UPPER_WIDTH, KEY_LOWER_COLOR, 0 },  // A
        { *this, 10, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR },     // Bb
        { *this, 11, upper, KEY_UPPER_WIDTH, KEY_LOWER_COLOR, 0 },  // B
    };
#elif PIANO_MODE == 2
    // No upper keys
    const static PianoPitchClass _defaultPitches[MAX_PITCH_CLASSES] = {
        { *this,  0, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR }, // C
        { *this,  1, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR }, // C#
        { *this,  2, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR }, // D
        { *this,  3, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR }, // Eb
        { *this,  4, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR }, // E
        { *this,  5, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR }, // F
        { *this,  6, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR }, // F#
        { *this,  7, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR }, // G
        { *this,  8, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR }, // Ab
        { *this,  9, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR }, // A
        { *this, 10, lower, KEY_LOWER_WIDTH, KEY_UPPER_COLOR }, // Bb
        { *this, 11, lower, KEY_LOWER_WIDTH, KEY_LOWER_COLOR }, // B
    };
#endif

    // Return a copy of our default pitch classes as a vector.
    // TODO: user customization, maybe??
    return std::vector<PianoPitchClass> { std::begin(_defaultPitches), std::end(_defaultPitches) };
}

std::vector<PianoPitch> Piano::makePitches()
{
    std::vector<PianoPitch> pitches;
    pitches.reserve(MAX_PITCHES);
    for (int i = 0; i < MAX_PITCHES; ++i)
        pitches.emplace_back(*this, i);
    return pitches;
}

std::vector<PianoKey> Piano::makeKeys()
{
    std::vector<PianoKey> keys;
    keys.reserve(MAX_PITCHES);

    int nextX = 0;
    for (int i = 0; i < MAX_PITCHES; ++i) {
        const PianoPitchClass &pitchClass = _pitches[i].getPitchClass();
        keys.emplace_back(*this, i, nextX - (pitchClass.isLower() ? 0 : pitchClass.getWidth() / 2));

        if (pitchClass.isLower())
            nextX += pitchClass.getWidth() + KEY_GAP;
    }
    return keys;
}

void Piano::unstrikeKeys() const
{
    for (auto &key : _keys)
        key.unstrike();
}

void Piano::strikeKeysByFreq(double freq, double volume) const
{
    sectionRange_t range = getKeyRangeByFreq(freq);
    const std::vector<PianoKey *> &keys = getKeysByRange(range);

    for (const auto &key : keys)
        key->strike(range, volume);
}

void Piano::draw(QPainter &painter) const
{
    updateBackgroundColor();

    // Update the painter transformation matrix to draw our piano in the center,
    // stretches across the full width of its render area.
    QRect window = painter.window();
    int windowWidth = window.width();
    int windowHeight = window.height();
    double rotation, scale;

    // If the render area is taller than it is wide, rotate the camera 90 degrees.
    // Place the high range up top (seems intuitive!)
    if (windowHeight > windowWidth) {
        scale = (double) windowHeight / (double) _width;
        rotation = -90.00;
    }
    else {
        scale = (double) windowWidth / (double) _width;
        rotation = 0.00;
    }

    // Set up the transformation matrix.
    painter.resetTransform();

    painter.translate(windowWidth * 0.50, windowHeight * 0.50);
    painter.scale(scale, scale);

    if (rotation != 0.00)
        painter.rotate(rotation);

    painter.translate(_width * -0.50, _height * -0.50);

    // Draw lower pitches first...
    for (const auto &key : _keys)
        if (key.getPitch().getPitchClass().isLower())
            key.draw(painter);

    // ...then upper pitches on top.
    for (const auto &key : _keys)
        if (!key.getPitch().getPitchClass().isLower())
            key.draw(painter);

    // Render a pretty plume of the overall color
    QColor bgColor = getBackgroundColor();
    bgColor.setAlpha(std::min(160, (int) (_bgVolume * 160.00)));
    QColor bgColorHalfTransparent = bgColor;
    bgColorHalfTransparent.setAlpha(std::min(160, (int) (_bgVolume * 80.00)));
    QColor bgColorTransparent = bgColor;
    bgColorTransparent.setAlpha(0);
    int middleX = _width / 2;

    double outerPlumeHeight = _bgVolume * _height * 2.00;
    double innerPlumeHeight = _height / 4;

    // Outer plume top
    {
        QRect rect = QRect(0, -outerPlumeHeight, _width, outerPlumeHeight);
        QLinearGradient gradient(middleX, rect.top(), middleX, rect.bottom());
        gradient.setColorAt(0.00, bgColorTransparent);
        gradient.setColorAt(1.00, bgColor);
        painter.fillRect(rect, gradient);
    }

    // Outer plume bottom
    {
        QRect rect = QRect(0, _height, _width, outerPlumeHeight);
        QLinearGradient gradient(middleX, rect.top(), middleX, rect.bottom());
        gradient.setColorAt(0.00, bgColor);
        gradient.setColorAt(1.00, bgColorTransparent);
        painter.fillRect(rect, gradient);
    }

    // Inner plume top
    {
        QRect rect = QRect(0, 0, _width, innerPlumeHeight);
        QLinearGradient gradient(middleX, rect.top(), middleX, rect.bottom());
        gradient.setColorAt(0.00, bgColorHalfTransparent);
        gradient.setColorAt(1.00, bgColorTransparent);
        painter.fillRect(rect, gradient);
    }

    // Inner plume bottom
    {
        QRect rect = QRect(0, _height - innerPlumeHeight, _width, innerPlumeHeight);
        QLinearGradient gradient(middleX, rect.top(), middleX, rect.bottom());
        gradient.setColorAt(0.00, bgColorTransparent);
        gradient.setColorAt(1.00, bgColorHalfTransparent);
        painter.fillRect(rect, gradient);
    }
}

void Piano::updateDimensions()
{
    const PianoKey &lastKey = getLastKey();
    _width  = lastKey.getX() + lastKey.getPitchClass().getWidth() + KEYBOARD_BORDER * 2;
    _height = KEY_LOWER_HEIGHT + KEYBOARD_BORDER * 2;
}

void Piano::buildFreqToRangeMap(const log_transform_stats &stats) const
{
    for (auto &key : _keys)
        key.clearSections();

    _freqToRangeMap.clear();
    _rangeToKeysMap.clear();

    const auto &freqStats = stats.get_freq_stats();
    if (freqStats.size() < 2)
        return;

    bool hasFirstKey = false, firstKeyInserted = false;
    double lastFreq, lastKey, lastKeyRight;
    double keyDelta, keyDeltaHalf;

    for (const auto &fs : freqStats) {
        double freq = fs.first;
        double key = getKeyIndexByFreq(freq);

        if (!hasFirstKey) {
            lastFreq = freq;
            lastKey = key;

            hasFirstKey = true;
            continue;
        }

        if (!firstKeyInserted) {
            keyDelta = key - lastKey;
            keyDeltaHalf = keyDelta / 2.00;
            lastKeyRight = key + keyDeltaHalf;

            _freqToRangeMap.emplace(lastFreq, sectionRange_t(key - keyDeltaHalf, lastKeyRight));
            firstKeyInserted = true;
        }

        double keyRight = key + keyDeltaHalf;
        _freqToRangeMap.emplace(freq, sectionRange_t(lastKeyRight, keyRight));

        lastFreq = freq;
        lastKey = key;
        lastKeyRight = keyRight;
    }
}

double Piano::getKeyIndexByFreq(double freq) const
{
    auto freqToKeyIndex = _freqToKeyIndexMap.find(freq);
    if (freqToKeyIndex != _freqToKeyIndexMap.end())
        return freqToKeyIndex->second;

    double keyIndex = std::log2(freq / LOW_PITCH_FREQ) * (double) MAX_PITCH_CLASSES;
    _freqToKeyIndexMap.emplace(freq, keyIndex);
    return keyIndex;
}

std::vector<PianoKey *> &Piano::convertRangeToKeys(const sectionRange_t &range) const
{
    auto rangeToKeyIndex = _rangeToKeysMap.find(range);
    if (rangeToKeyIndex != _rangeToKeysMap.end())
        return rangeToKeyIndex->second;

    // Determine which keys belong to this range
    std::vector<PianoKey *> keys {};
    size_t min_key = (int) std::floor(range.first + 0.49);
    size_t max_key = (int) std::ceil(range.second + 0.49);

    min_key = std::clamp(min_key, (size_t) 0, _keys.size());
    max_key = std::clamp(max_key, (size_t) 0, _keys.size());
    keys.reserve(max_key - min_key);
    for (size_t i = min_key; i < max_key; ++i) {
        auto k = const_cast<PianoKey *>(&_keys[i]);
        keys.push_back(k);
    }

    _rangeToKeysMap.emplace(range, std::move(keys));
    return _rangeToKeysMap.at(range);
}

std::vector<PianoKey *> &Piano::getKeysByRange(const sectionRange_t &range)
    { return convertRangeToKeys(range); }

const std::vector<PianoKey *> &Piano::getKeysByRange(const sectionRange_t &range) const
    { return convertRangeToKeys(range); }

void Piano::updateBackgroundColor() const
{
    double totalVolume = 0.00, totalWeight = 0.00, totalWeightVolume = 0.00;
    double totalX = 0.00, totalY = 0.00;
    double maxVolume = 0.00;

    for (auto &key : _keys) {
        for (auto &sectionPair : key.getSectionMap()) {
            const PianoKeySection &section = sectionPair.second;

            double volume = section.getVolume();
            QColor color = section.getColor();
            float x, y, v, weight;
            qColorGetXyvF(color, &x, &y, &v);

            weight = std::hypotf(x, y) * v;
            double weightVolume = weight * volume;

            totalVolume += volume;
            totalWeight += weight;
            totalWeightVolume += weightVolume;
            totalX += x * weightVolume;
            totalY += y * weightVolume;

            if (volume > maxVolume)
                maxVolume = volume;
        }
    }

    float x = totalX / totalWeightVolume;
    float y = totalY / totalWeightVolume;

    qColorSetXyvF(_bgColor, x, y, 1.00);

    // Increase saturation
    float bgH, bgS, bgV;
    _bgColor.getHsvF(&bgH, &bgS, &bgV);
    _bgColor.setHsvF(bgH, std::min(1.00f, bgS * 2.00f), bgV);

    double avgVolume = totalVolume / totalWeight;
    _bgVolume = (maxVolume + avgVolume * 3.00) / 4.00;

    if (_bgVolume >= 1.00)
        _bgScaledColor = _bgColor;
    else {
        float r, g, b;
        _bgColor.getRgbF(&r, &g, &b);
        _bgScaledColor.setRgbF(r * _bgVolume, g * _bgVolume, b * _bgVolume);
    }
}