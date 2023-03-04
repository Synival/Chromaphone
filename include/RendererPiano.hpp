#ifndef PITCH_QT_RENDERER_PIANO_HPP_INCLUDED
#define PITCH_QT_RENDERER_PIANO_HPP_INCLUDED

#include <memory>

#include "Renderer.hpp"

#include "Piano.hpp"

class QWidget;
class QPainter;

class RendererPiano : public Renderer {
private:

public:
    RendererPiano(QWidget &widget);

    virtual void render(const transform_results<double> &data, const log_transform_stats *stats) const override;

private:
    virtual void onSetStats(const log_transform_stats &stats) override;

    [[maybe_unused]] QWidget &_widget;
    Piano _piano;
};

#endif // PITCH_QT_RENDERER_PIANO_HPP_INCLUDED
