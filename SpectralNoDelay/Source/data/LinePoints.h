/*
  ==============================================================================

    LinePoints.h
    Created: 6 Feb 2024 1:27:58pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>

namespace snd {
class LinePoints {
public:
    struct Point {
        float x;
        float y;
        float power;

        void Clamp() {
            x = std::clamp(x, 0.0f, 1.0f);
            y = std::clamp(y, 0.0f, 1.0f);
            power = std::clamp(power, -0.9999f, 0.9999f);
        }
    };

    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void point_changed(LinePoints& generator, int idx) = 0;
        virtual void point_added_at(LinePoints& generator, int idx_at, Point point) = 0;
        virtual void point_removed(LinePoints& generator, int idx) = 0;
        virtual void reloaded(LinePoints& generator) = 0;
    };

    LinePoints();

    void fill_default();

    void remove(size_t idx);

    void add_behind(size_t idx, Point point);

    void add_point(Point point);

    void change(size_t idx, Point point);

    template<typename T> requires std::is_same_v<std::vector<Point>, std::decay_t<T>>
    void fill_points(T&& point) {
        m_points = std::forward<T>(point);

        for (auto& p : m_points) {
            p.Clamp();
        }

        for (auto* l : m_listeners) {
            l->reloaded(*this);
        }
    }

    void add_listener(Listener* l);
    void remove_listener(Listener* l);

    std::vector<Point>& get_points() { return m_points; }
    const std::vector<Point>& get_points() const { return m_points; }
    Point get_point(size_t idx) const { return m_points[idx]; }
    int GetNumPoints() const { return static_cast<int>(m_points.size()); }

private:
    std::vector<Point> m_points;
    juce::Array<Listener*> m_listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinePoints);
};
}