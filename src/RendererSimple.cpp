#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#pragma GCC diagnostic pop

#include "../lft/api/transform_results.hpp"

#include "Utils.hpp"

#include "RendererSimple.hpp"

RendererSimple::RendererSimple(QWidget &widget) :
    _widget(widget)
{}

void RendererSimple::render(const transform_results<double> &data, const log_transform_stats */*stats*/) const
{
    QPainter *painterPtr = getPainter();
    if (painterPtr == nullptr)
        return;
    QPainter &painter = *painterPtr;

    std::map<double, double> results = data.get_results();

    painter.setPen(Qt::black);

    int width  = _widget.width();
    int height = _widget.height();

    int index = 0;
    int max_index = results.size();

    int last_x = 0;
    int last_y = height - 1;

    double max_val = 0.00;
    for (auto &r : results)
        if (r.second > max_val)
            max_val = r.second;
    max_val = std::clamp(max_val, 0.00, 1.00);

    for (auto &r : results) {
        double freq_note = usefulFMod(std::log2(r.first / 440.00) - 0.25, 1.00);
        int hue = (int) (freq_note * 359.99);

/*
        // Offset quarter-tones by 180 degrees
        if ((int) (freq_note * 24.00 + 0.50) % 2)
            hue = (hue + 180) % 360;
*/

        double pos_percent = (double) index / (double) max_index;
        double val = std::min(r.second, 1.00);

        int new_x = (int) (pos_percent * (double) width);
        int new_y = height - 1 - (int) (val * (double) height);

        QPolygon poly;
        poly << QPoint { last_x, last_y }
             << QPoint { new_x,  new_y  }
             << QPoint { new_x,  height - 1 }
             << QPoint { last_x, height - 1 };

        // Fill polygon
        QPainterPath path;
        path.addPolygon(poly);

        // Brush
        QBrush brush;
        QColor color;
        int intensity = (int) std::min(255.00, std::pow(val / max_val, 0.50) * 255.00);

        color.setHsv(hue, 255, intensity, (intensity >= 127) ? 255 : intensity * 2);
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);

        // Draw polygon
        painter.fillPath(path, brush);

        last_x = new_x;
        last_y = new_y;
        ++index;
    }
}
