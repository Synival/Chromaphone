#ifndef PITCH_QT_RENDERER_SIMPLE_HPP_INCLUDED
#define PITCH_QT_RENDERER_SIMPLE_HPP_INCLUDED

#include "Renderer.hpp"

class QWidget;

class RendererSimple : public Renderer {
public:
    RendererSimple(QWidget &widget);

    virtual void render(const transform_results<double> &data, const log_transform_stats *stats) const override;

private:
    QWidget &_widget;
};

#endif // PITCH_QT_RENDERER_SIMPLE_HPP_INCLUDED