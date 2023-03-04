#ifndef PITCH_QT_UTILS_HPP_INCLUDED
#define PITCH_QT_UTILS_HPP_INCLUDED

#include <algorithm>
#include <vector>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QDebug>
#pragma GCC diagnostic pop

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

#define FIND_IF(_container, _predicate) \
    (std::find_if((_container).begin(), (_container).end(), (_predicate)))

inline double usefulFMod(double num, double div)
    { return std::fmod(std::fmod(num, div) + div, div); }

QColor mixColors(const QColor &color1, const QColor &color2, double mix);
QColor mixColorsPolar(const QColor &color1, const QColor &color2, double mix);
void qColorGetXyvF(const QColor &color, float *x, float *y, float *v, float *a = nullptr);
void qColorSetXyvF(QColor &color, float x, float y, float v, float a = 1.00);

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

std::vector<std::string> cstrArrayToVector(const char **array);

#endif // PITCH_QT_UTILS_HPP_INCLUDED