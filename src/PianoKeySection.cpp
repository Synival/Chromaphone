// PianoKeySection.cpp
// -------------------
// A physical portion of a PianoKey which corresponds to a specific frequency range.

#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QPainter>
#include <QRect>
#pragma GCC diagnostic pop

#include "PianoKey.hpp"
#include "Utils.hpp"

#include "PianoKeySection.hpp"

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

    // TODO: maybe some way to change this as a user???
#if 1
    // Colors, arranged in a rainbow order in the circle of fifths
    double hue = usefulFMod((double) _midiPitch / (double) MAX_PITCH_CLASSES, 1.00);
    if (_key.getPitch().getMidiPitch() % 2 == 1)
        hue = usefulFMod(hue + 0.50, 1.00);
#else
    // Pretty blue!
    double hue = 210.00 / 360.00;
#endif

    // Desaturate colors that are closer to the edge (i.e, less in-tune)
    double pitchOffset = usefulFMod(_midiPitch + 0.50, 1.00) - 0.50;
    double precision = 1.00 - std::abs(pitchOffset) * 2.00;

    color.setHslF(hue, 1.00 * precision, 0.25 * precision + 0.25);
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
