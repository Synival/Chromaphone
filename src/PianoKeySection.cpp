// PianoKeySection.cpp
// -------------------
// A physical portion of a PianoKey which corresponds to a specific frequency range.

#include "PianoKeySection.hpp"

#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QPainter>
#include <QRect>
#pragma GCC diagnostic pop

#include "PianoKey.hpp"
#include "Utils.hpp"

PianoKeySection::PianoKeySection(PianoKey &key, const sectionRange_t &range) :
    _key(key),
    _range(range),
    _midiPitch((range.first + range.second) * 0.50 - (double) _key.getKeyIndex() + (double) _key.getPitch().getMidiPitch()),
    _color(createColor()),
    _drawBox(createDrawBox())
{}

void PianoKeySection::unstrike() const
    { _volume = 0.00; }

void PianoKeySection::strike(double volume) const
{
    if (volume > _volume)
        _volume = volume;
}

QColor PianoKeySection::getDrawColor() const
{
    QColor keyColor = _key.getColor();
    if (_volume == 0.00)
        return keyColor;

    double vol = _volume * 2.00;
    if (vol > 1.00) {
        vol = (vol - 1.00) * 4.00;
        vol = (vol / (vol + 1)) + 1.00;
    }
    return mixColors(keyColor, _color, vol);
}

QColor PianoKeySection::createColor() const
{
    QColor color;

#if PIANO_KEY_COLOR_MODE == 0
    // Colors, arranged in a rainbow order in the circle of fifths
    double hue = usefulFMod((double) _midiPitch / (double) MAX_PITCH_CLASSES, 1.00);
    if (_key.getPitch().getMidiPitch() % 2 == 1)
        hue = usefulFMod(hue + 0.50, 1.00);

    // Desaturate colors that are closer to the edge (i.e, less in-tune)
#elif PIANO_KEY_COLOR_MODE == 1
    // Colors, arranged in a rainbow order chromatically.
    double hue = usefulFMod((double) _midiPitch / (double) MAX_PITCH_CLASSES, 1.00);
#elif PIANO_KEY_COLOR_MODE == 2
    // Pretty blue!
    static constexpr double hue = 210.00 / 360.00;
#endif

#ifdef PIANO_KEY_DESATURATE_NON_EQUAL_TEMPERMENT_TONES
    double precision = 1.00 - (std::abs(usefulFMod(_midiPitch + 0.50, 1.00) - 0.50)) * 2.00;
    double saturation = precision;
    double luminosity = 0.25 * precision + 0.25;
#else
    static constexpr double saturation = 1.00;
    static constexpr double luminosity = 0.50;
#endif

    color.setHslF(hue, saturation, luminosity);
    return color;
}

QRect PianoKeySection::createDrawBox() const
{
    QRect keyBox = _key.getDrawBox();
    double keyIndex = _key.getKeyIndex();

    double leftPercent  = std::clamp(_range.first  - keyIndex + 0.50, 0.00, 1.00);
    double rightPercent = std::clamp(_range.second - keyIndex + 0.55, 0.00, 1.00);

    double keyLeft = keyBox.left();
    double keyWidth = keyBox.width();
    double left  = leftPercent  * keyWidth + keyLeft;
    double right = rightPercent * keyWidth + keyLeft;
    double width = right - left;

    return QRect(left, keyBox.top(), width, keyBox.height());
}
