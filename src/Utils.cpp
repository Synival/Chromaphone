// Utils.hpp
// ---------
// Handy utility functions that can be used anywhere.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QColor>
#pragma GCC diagnostic pop

#include "Utils.hpp"

QColor mixColors(const QColor &color1, const QColor &color2, double mix)
{
    if (mix <= 0.00)
        return color1;

    if (mix >= 2.00)
        return Qt::white;

    if (mix >= 1.00) {
        float r, g, b, a;
        color2.getRgbF(&r, &g, &b, &a);

        mix -= 1.00;
        double invMix = 1.00 - mix;

        QColor rval;
        rval.setRgbF(
            (r * invMix) + mix,
            (g * invMix) + mix,
            (b * invMix) + mix,
            (a * invMix) + mix
        );
        return rval;
    }

    float r1, g1, b1, a1;
    float r2, g2, b2, a2;
    color1.getRgbF(&r1, &g1, &b1, &a1);
    color2.getRgbF(&r2, &g2, &b2, &a2);

    double invMix = 1.00 - mix;

    QColor rval;
    rval.setRgbF(
        (r1 * invMix) + (r2 * mix),
        (g1 * invMix) + (g2 * mix),
        (b1 * invMix) + (b2 * mix),
        (a1 * invMix) + (a2 * mix)
    );
    return rval;
}

static constexpr double M_PI2 = M_PI * 2.00;

void qColorGetXyvF(const QColor &color, float *x, float *y, float *v, float *a)
{
    float h, s;
    color.getHsvF(&h, &s, v, a);

    if (x)
        *x = cos(M_PI2 * h) * s;
    if (y)
        *y = sin(M_PI2 * h) * s;
}

void qColorSetXyvF(QColor &color, float x, float y, float v, float a)
{
    float h = std::atan2(y, x) / M_PI2;
    if (h < 0.00)
        h += 1.00;
    float s = std::hypot(y, x);
    if (s > 1.00)
        s = 1.00;

    color.setHsvF(h, s, v, a);
}

QColor mixColorsPolar(const QColor &color1, const QColor &color2, double mix)
{
    float x1, y1, v1, a1;
    qColorGetXyvF(color1, &x1, &y1, &v1, &a1);

    float x2, y2, v2, a2;
    qColorGetXyvF(color2, &x2, &y2, &v2, &a2);

    float invMix = 1.00 - mix;
    float xn = x1 * invMix + x2 * mix;
    float yn = y1 * invMix + y2 * mix;
    float vn = v1 * invMix + v2 * mix;
    float an = a1 * invMix + a2 * mix;

    QColor outColor;
    qColorSetXyvF(outColor, xn, yn, vn, an);
    return outColor;
}

std::vector<std::string> cstrArrayToVector(const char **array)
{
    std::vector<std::string> vecOut;

    for (const char **a = array; *a != nullptr; ++a) {
        const char *str = *a;
        vecOut.push_back(str);
    }

    return vecOut;
}
