/*
  ==============================================================================

    LineGenerator.cpp
    Created: 21 Feb 2024 10:58:08am
    Author:  mana

  ==============================================================================
*/

#include "LineGenerator.h"
#include "../utli.h"

namespace snd {
LineGenerator::LineGenerator(LinePoints& lp) 
    : line_points_(lp) {
    lp.add_listener(this);
}

LineGenerator::~LineGenerator() {
    line_points_.remove_listener(this);
}

void LineGenerator::SetBufferSize(size_t size) {
    render_buffer_.resize(size);
    RenderPoint(0, static_cast<int>(line_points_.get_points().size()));
}

void LineGenerator::point_changed(LinePoints& generator, int idx) {
    RenderPoint(idx - 1, idx + 1);
}

void LineGenerator::point_added_at(LinePoints& generator, int idx_at, LinePoints::Point point) {
    RenderPoint(idx_at - 1, idx_at + 1);
}

void LineGenerator::point_removed(LinePoints& generator, int idx) {
    RenderPoint(idx - 1, idx);
}

void LineGenerator::reloaded(LinePoints& generator) {
    RenderPoint(0, static_cast<int>(line_points_.get_points().size()));
}

void LineGenerator::RenderPoint(int begin_point_idx, int end_point_idx) {
    begin_point_idx = std::max(begin_point_idx, 0);
    end_point_idx = std::min(end_point_idx, line_points_.GetNumPoints());
    const auto& points = line_points_.get_points();
   
    for (size_t i = begin_point_idx; i < end_point_idx; ++i) {
        if (i + 1 >= points.size()) {
            break;
        }

        const auto begin_idx = static_cast<size_t>(points[i].x * render_buffer_.size());
        const auto end_idx = static_cast<size_t>(points[i + 1].x * render_buffer_.size());
        const auto power = points[i].power;
        const auto begin_y = points[i].y;
        const auto end_y = points[i + 1].y;
        const auto length = end_idx - begin_idx;

        for (size_t j = 0; j < length; ++j) {
            const auto x = static_cast<float>(j) / static_cast<float>(length);
            render_buffer_[begin_idx + j] = utli::linear_interpole(begin_y, end_y, x, power);
        }
    }

    const auto begin_idx = static_cast<int>(points[begin_point_idx].x * render_buffer_.size());
    const auto end_idx = static_cast<int>(points[
        std::min(end_point_idx, line_points_.GetNumPoints() - 1)].x * render_buffer_.size());
    listeners_.call(&Listener::DataChanged, this, begin_idx, end_idx);
}
}