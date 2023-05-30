// RendererPiano.cpp
// -----------------
// Renderer for LFT results which draws a piano with pretty colors and highlights.

#include "RendererPiano.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#pragma GCC diagnostic pop

#include "../lft/api/transform_results.hpp"

RendererPiano::RendererPiano(QWidget &widget) :
    _widget(widget),
    _piano()
{
}

void RendererPiano::render(const transform_results<double> &data, const log_transform_stats *stats) const
{
    _piano.unstrikeKeys();

    if (stats != nullptr) {
        for (auto &result : data.get_results())
            _piano.strikeKeysByFreq(result.first, result.second);
    }

    QPainter *painter = getPainter();
    if (painter != nullptr)
        _piano.draw(*painter);
}

void RendererPiano::onSetStats(const log_transform_stats &stats)
    { _piano.buildFreqToRangeMap(stats); }
