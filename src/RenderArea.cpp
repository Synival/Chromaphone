// RenderArea.cpp
// --------------
// A widget which renders LFT results using a Renderer of choice.

#include "RenderArea.hpp"

#include <iostream>
#include <chrono>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QPainter>
#include <QPaintEvent>
#ifdef ANDROID
    #include <QJniObject>
    #include <QJniEnvironment>
    #include <QCoreApplication>
#endif
#pragma GCC diagnostic pop

// TODO: this shouldn't be hard-coded :(
// #define USE_RENDER_SIMPLE

#ifdef USE_RENDER_SIMPLE
    #include "RendererSimple.hpp"
    using DEFAULT_RENDERER = RendererSimple;
#else
    #include "RendererPiano.hpp"
    using DEFAULT_RENDERER = RendererPiano;
#endif

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    _renderer(std::make_unique<DEFAULT_RENDERER>(*this))
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

RenderArea::~RenderArea()
{}

void RenderArea::setResults(const transform_results<double> &results)
    { _results = std::make_unique<transform_results<double>>(results); }

void RenderArea::setResults(transform_results<double> &&results)
    { _results = std::make_unique<transform_results<double>>(std::move(results)); }

void RenderArea::setStats(const log_transform_stats &stats)
    { _renderer->setStats(stats); }

void RenderArea::paintEvent(QPaintEvent * /*event*/)
{
    if (!_results)
        return;

    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    _renderer->setPainter(&painter);
    _renderer->render(*_results, _renderer->getStats());
    _renderer->setPainter(nullptr);
    painter.end();
}

void RenderArea::mousePressEvent(QMouseEvent */*event*/)
{
    emit clicked();
}
