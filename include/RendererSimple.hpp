// RendererSimple.hpp
// ------------------
// Renderer for LFT results in a simple graph format.

#ifndef PITCH_QT_RENDERER_SIMPLE_HPP_INCLUDED
#define PITCH_QT_RENDERER_SIMPLE_HPP_INCLUDED

#include "Renderer.hpp"

// Forward declarations
class QWidget;

// Renderer for LFT results in a simple graph format.
class RendererSimple : public Renderer {
public:
    RendererSimple(QWidget &widget);

    virtual void render(const transform_results<double> &data, const log_transform_stats *stats) const override;

private:
    QWidget &_widget;
};

#endif // PITCH_QT_RENDERER_SIMPLE_HPP_INCLUDED
