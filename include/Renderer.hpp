#ifndef PITCH_QT_RENDERER_HPP_INCLUDED
#define PITCH_QT_RENDERER_HPP_INCLUDED

#include <memory>

#include "../lft/api/log_transform_stats.hpp"

#include "../lft/api/log_transformer_renderer_i.hpp"

class QPainter;

class Renderer : public log_transformer_renderer_i
{
public:
    log_transform_stats *getStats() const
        { return _stats.get(); }
    QPainter *getPainter() const
        { return _painter; }

    void setStats(const log_transform_stats &stats) {
        _stats = std::make_unique<log_transform_stats>(std::move(stats));
        onSetStats(stats);
    }
    void setPainter(QPainter *painter)
        { _painter = painter; }

private:
    virtual void onSetStats(const log_transform_stats &/*stats*/) {};

    std::unique_ptr<log_transform_stats> _stats;
    QPainter *_painter;
};

#endif // PITCH_QT_RENDERER_HPP_INCLUDED
