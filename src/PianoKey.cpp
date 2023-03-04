// PianoKey.cpp
// ------------
// An individual key used in the Piano class.

#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QPainter>
#include <QRect>
#pragma GCC diagnostic pop

#include "Piano.hpp"
#include "PianoKeySection.hpp"
#include "Utils.hpp"

#include "PianoKey.hpp"

PianoPitch &PianoKey::getPitch()
    { return _piano.getPitch(_pitchIndex); }
const PianoPitch &PianoKey::getPitch() const
    { return _piano.getPitch(_pitchIndex); }
PianoKey &PianoKey::getPrevKey()
    { return _piano.getKey(_keyIndex - 1); }
const PianoKey &PianoKey::getPrevKey() const
    { return _piano.getKey(_keyIndex - 1); }
PianoKey &PianoKey::getNextKey()
    { return _piano.getKey(_keyIndex + 1); }
const PianoKey &PianoKey::getNextKey() const
    { return _piano.getKey(_keyIndex + 1); }

void PianoKey::clearSections() const
    { _sectionMap.clear(); }

void PianoKey::unstrike() const
{
    for (auto &s : _sectionMap)
        s.second.unstrike();
}

void PianoKey::strike(const sectionRange_t &range, double volume) const
    { convertRangeToSection(range).strike(volume); }

void PianoKey::draw(QPainter &painter) const
{
    const PianoPitchClass &pitchClass = getPitchClass();
    const QColor bgColor = _piano.getBackgroundColor();
    double bgColorIntensity = pow(_piano.getBackgroundColorIntensity(), 0.50);

    // Get the loudest color of the key
    const PianoKeySection *loudestSection = nullptr;
    double loudestVolume = 0.00;
    for (const auto &section_i : _sectionMap) {
        const PianoKeySection &section = section_i.second;
        double volume = section.getVolume();
        if (loudestSection == nullptr || volume > loudestVolume) {
            loudestSection = &section;
            loudestVolume = volume;
        }
    }
    QColor loudestColor = loudestSection ? loudestSection->getDrawColor() : Qt::black;

    // Draw black outline
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setBrush(QBrush { Qt::black, Qt::BrushStyle::SolidPattern });
    painter.drawRect(
        _x + KEYBOARD_BORDER - KEY_GAP, KEYBOARD_BORDER - KEY_GAP,
        pitchClass.getWidth() + KEY_GAP * 2, pitchClass.getHeight() + KEY_GAP + 2
    );

    // Draw all subsections
    for (const auto &section_i : _sectionMap) {
        const PianoKeySection &section = section_i.second;

        // Draw the physical part of the key
        QColor drawColor = section.getDrawColor();
        painter.setBrush(QBrush { mixColors(drawColor, loudestColor, 0.50), Qt::BrushStyle::SolidPattern });
        QRect box = section.getDrawBox();
        painter.drawRect(box);

        // Determine the color of the plume
        QColor realColor = section.getColor();
        double volume = section.getVolume();

        QColor plumeColorInner = mixColors(Qt::black, mixColorsPolar(realColor, bgColor, std::min(1.00, bgColorIntensity / 16.00)), volume);
        QColor plumeColorMiddle = mixColors(Qt::black, mixColorsPolar(realColor, bgColor, std::min(1.00, bgColorIntensity)), volume);
        plumeColorMiddle.setAlpha(192);
        QColor plumeColorOuter = mixColors(Qt::black, bgColor, volume);
        plumeColorOuter.setAlpha(0);

        // Additional precalculations for plumes
        int rectHeight = volume * KEY_LOWER_HEIGHT * 1.50;
        int boxLeft    = box.left();
        int boxRight   = box.right();
        int boxTop     = box.top() - KEYBOARD_BORDER * 2;
        int boxWidth   = box.width();
        int boxBottom  = box.top() + KEY_LOWER_HEIGHT + KEYBOARD_BORDER * 2;
        int boxCenterX = (boxLeft + boxRight) / 2;

        // Draw the top plume...
        {
            QRect rect = QRect(boxLeft, boxTop - rectHeight, boxWidth, rectHeight);
            QLinearGradient gradient(boxCenterX, rect.top(), boxCenterX, rect.bottom());
            gradient.setColorAt(0.00, plumeColorOuter);
            gradient.setColorAt(0.75, plumeColorMiddle);
            gradient.setColorAt(1.00, plumeColorInner);
            painter.fillRect(rect, gradient);
        }

        // ...and the bottom plume.
        {
            QRect rect = QRect(boxLeft, boxBottom, boxWidth, rectHeight);
            QLinearGradient gradient(boxCenterX, rect.top(), boxCenterX, rect.bottom());
            gradient.setColorAt(0.00, plumeColorInner);
            gradient.setColorAt(0.25, plumeColorMiddle);
            gradient.setColorAt(1.00, plumeColorOuter);
            painter.fillRect(rect, gradient);
        }
    }
}

QColor PianoKey::createColor() const
{
    const PianoPitch &pitch = getPitch();
    const PianoPitchClass &pitchClass = pitch.getPitchClass();

    // Middle C has a special color
    if (pitch.getMidiPitch() == 60) {
        QColor color = getPitchClass().getColor();
        float r, g, b;
        color.getRgbF(&r, &g, &b);
        color.setRgbF(r * 1.00, g * 0.90, b * 0.80);
        return color;
    }

    // No special color - return the default
    return pitchClass.getColor();
}

PianoKeySection &PianoKey::convertRangeToSection(const sectionRange_t &range) const
{
    // Add the index to our freq-to-key map if it doesn't already exist
    auto iter = _sectionMap.find(range);
    if (iter != _sectionMap.end())
        return iter->second;

    _sectionMap.emplace(std::pair<sectionRange_t, PianoKeySection>(range, PianoKeySection { const_cast<PianoKey &>(*this), range }));
    return _sectionMap.at(range);
}

PianoKeySection &PianoKey::getSectionByRange(const sectionRange_t &range)
    { return convertRangeToSection(range); }

const PianoKeySection &PianoKey::getSectionByRange(const sectionRange_t &range) const
    { return convertRangeToSection(range); }

QRect PianoKey::getDrawBox() const
{
    const PianoPitchClass &pitchClass = getPitch().getPitchClass();
    return QRect(
         _x + KEYBOARD_BORDER, KEYBOARD_BORDER,
        pitchClass.getWidth(), pitchClass.getHeight()
    );
}
