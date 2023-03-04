// Utils.hpp
// ---------
// Handy utility functions that can be used anywhere.

#ifndef PITCH_QT_UTILS_HPP_INCLUDED
#define PITCH_QT_UTILS_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QDebug>
#pragma GCC diagnostic pop

// Increments a counter when created, decrements that counter when destroyed.
class IncreaseCounter
{
public:
    inline IncreaseCounter(int &ref) : _ref(ref)
        { ++_ref; }
    inline ~IncreaseCounter()
        { --_ref; }

private:
    int &_ref;
};

// Shorthand for std::find_if(), without the .begin() and .end()
#define FIND_IF(_container, _predicate) \
    (std::find_if((_container).begin(), (_container).end(), (_predicate)))

// Floating-point modulo (not remainder) function that works with negative numbers.
inline double usefulFMod(double num, double div)
    { return std::fmod(std::fmod(num, div) + div, div); }

// Mixes colors by their RGB values.
QColor mixColors(const QColor &color1, const QColor &color2, double mix);

// Mixes colors using their "XYV" coordinates.
QColor mixColorsPolar(const QColor &color1, const QColor &color2, double mix);

// Gets XYV values for a QColor input.
void qColorGetXyvF(const QColor &color, float *x, float *y, float *v, float *a = nullptr);

// Sets a QColor's RGB values using XYV input.
void qColorSetXyvF(QColor &color, float x, float y, float v, float a = 1.00);

// Finds the maximum value in a map.
template<typename TKey, typename TValue>
inline const TValue &mapMaxValue(const std::map<TKey, TValue> &map)
{
    return std::max_element(
        map.begin(),
        map.end(),
        [](const std::pair<TKey, TValue> p1, const std::pair<TKey, TValue> p2)
            { return p2 > p1; }
    );
}

// Converts a null-terminated char * list into a std::vector<std::string>.
std::vector<std::string> cstrArrayToVector(const char **array);

#endif // PITCH_QT_UTILS_HPP_INCLUDED
