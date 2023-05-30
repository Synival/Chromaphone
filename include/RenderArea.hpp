// RenderArea.hpp
// --------------
// A widget which renders LFT results using a Renderer of choice.

#ifndef PITCH_QT_RENDER_AREA_HPP_INCLUDED
#define PITCH_QT_RENDER_AREA_HPP_INCLUDED

#include <memory>
#include <chrono>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>
#pragma GCC diagnostic pop

#include "../lft/api/transformer_i.hpp"
#include "../lft/api/transform_results.hpp"
#include "../lft/api/log_transform_stats.hpp"

// Forward declarations
class Renderer;

namespace Ui {
    class RenderArea;
}

// A widget which renders LFT results using a Renderer of choice.
class RenderArea : public QWidget /*QOpenGLWidget*/ {
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = nullptr);
    ~RenderArea();

    void setResults(const transform_results<double> &results);
    void setResults(transform_results<double> &&results);
    void setStats(const log_transform_stats &stats);

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<transform_results<double>> _results;
};

#endif // PITCH_QT_RENDER_AREA_HPP_INCLUDED
