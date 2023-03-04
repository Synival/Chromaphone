// RendererPiano.hpp
// -----------------
// Renderer for LFT results which draws a piano with pretty colors and highlights.

#ifndef PITCH_QT_RENDERER_PIANO_HPP_INCLUDED
#define PITCH_QT_RENDERER_PIANO_HPP_INCLUDED

#include <memory>

#include "Renderer.hpp"

#include "Piano.hpp"

// Forward declarations
class QWidget;
class QPainter;

// Renderer for LFT results which draws a piano with pretty colors and highlights.
class RendererPiano : public Renderer {
private:

public:
    RendererPiano(QWidget &widget);

    virtual void render(const transform_results<double> &data, const log_transform_stats *stats) const override;

private:
    virtual void onSetStats(const log_transform_stats &stats) override;

    QWidget &_widget;
    Piano _piano;
};

#endif // PITCH_QT_RENDERER_PIANO_HPP_INCLUDED
