/*
  ==============================================================================

    LinePoints.cpp
    Created: 6 Feb 2024 1:27:58pm
    Author:  mana

  ==============================================================================
*/

#include "LinePoints.h"

constexpr size_t kInitPointCapacity = 128;

namespace snd {
LinePoints::LinePoints() {
    m_points.resize(kInitPointCapacity);
    fill_default();
}

void LinePoints::fill_default() {
    m_points.clear();
    m_points.emplace_back(Point{ 0.0f, 0.0f, 0.0f });
    m_points.emplace_back(Point{ 1.0f, 1.0f, 0.0f });

    for (auto* l : m_listeners) {
        l->reloaded(*this);
    }
}

void LinePoints::remove(size_t idx) {
    m_points.erase(m_points.begin() + idx);

    for (auto* l : m_listeners) {
        l->point_removed(*this, idx);
    }
}

void LinePoints::add_point(Point point) {
    point.Clamp();

    const size_t num_loop = m_points.size() - 1;
    for (size_t i = 0; i < num_loop; ++i) {
        if (point.x >= m_points[i].x && point.x <= m_points[i + 1].x) {
            add_behind(i, point);
            return;
        }
    }

    add_behind(m_points.size() - 1, point);
}

void LinePoints::add_behind(size_t idx, Point point) {
    point.Clamp();

    m_points.emplace(m_points.begin() + idx + 1, point);

    for (auto* l : m_listeners) {
        l->point_added_at(*this, idx + 1, point);
    }
}

void LinePoints::change(size_t idx, Point point) {
    if (idx == 0) {
        point.x = 0.0f;
    }
    else if (idx == m_points.size() - 1) {
        point.x = 1.0f;
    }
    point.Clamp();

    m_points[idx] = point;

    for (auto* l : m_listeners) {
        l->point_changed(*this, idx);
    }
}

void LinePoints::add_listener(Listener* l) {
    m_listeners.addIfNotAlreadyThere(l);
}

void LinePoints::remove_listener(Listener* l) {
    m_listeners.removeAllInstancesOf(l);
}
}