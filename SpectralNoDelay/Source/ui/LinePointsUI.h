/*
  ==============================================================================

    LinePointsUI.h
    Created: 6 Feb 2024 1:27:36pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../data/LinePoints.h"
#include "../data/LineGenerator.h"

namespace snd {

namespace detail {
class PointComponent;
class PowerPointComponent;
}

class LineGraphEditor
    : public juce::Component
    , private LinePoints::Listener
    , private LineGenerator::Listener {
public:
    explicit LineGraphEditor(LinePoints& generator);
    ~LineGraphEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void remove_point_component(size_t idx);
    void xy_point_moved(size_t idx);
    void power_point_moved(size_t idx);
    void power_point_reset(size_t idx);

    void mouseDoubleClick(const juce::MouseEvent& e) override;

    void EnableSnapGrid(bool b) { snap_ = b; }
    void ShowGrid(bool b) { show_grid_ = b; repaint(); }
    void SetXGrid(int x) { x_grid_ = std::max(x, 1); repaint(); }
    void SetYGrid(int y) { y_grid_ = std::max(y, 1); repaint(); }
private:
    // ========================================================================
    void rebuild_interface();
    void adjust_point_position(size_t idx) const;
    void adjust_power_point_position(size_t idx) const;

    std::unique_ptr<detail::PointComponent> create_point_component();
    std::unique_ptr<detail::PowerPointComponent> create_power_point_component();

    // ========================================================================
    void point_changed(LinePoints& generator, int idx) override;
    void point_added_at(LinePoints& generator, int idx_behind, LinePoints::Point point) override;
    void point_removed(LinePoints& generator, int idx) override;
    void reloaded(LinePoints& generator) override;

    // ========================================================================
    void DataChanged(LineGenerator* ptr_generator, int begin, int end) override;

    // ========================================================================
    LinePoints& line_points_;
    LineGenerator line_generator_;
    juce::OwnedArray<detail::PointComponent> m_points;
    juce::OwnedArray<detail::PowerPointComponent> m_power_points;

    std::unique_ptr<juce::MouseListener> m_point_processor;
    std::unique_ptr<juce::MouseListener> m_power_point_processor;

    // ========================================================================
    // snap and grid
    // ========================================================================
    static constexpr float kMinPixelDistance = 4.0f;
    bool show_grid_{ true };
    bool snap_{ true };
    int x_grid_{4};
    int y_grid_{4};

    template<typename T>
    T SnapX(T x) const {
        if (!snap_) {
            return x;
        }

        float x_grid_size = static_cast<float>(getWidth()) / static_cast<float>(x_grid_);
        float left_x = std::floor(x / x_grid_size) * x_grid_size;
        float right_x = left_x + x_grid_size;

        if (std::abs(x - left_x) < kMinPixelDistance) {
            return static_cast<T>(left_x);
        }
        else if (std::abs(x - right_x) < kMinPixelDistance) {
            return static_cast<T>(right_x);
        }
        else {
            return x;
        }
    }

    template<typename T>
    T SnapY(T y) const {
        if (!snap_) {
            return y;
        }

        float y_grid_size = static_cast<float>(getHeight()) / static_cast<float>(y_grid_);
        float left_y = std::floor(y / y_grid_size) * y_grid_size;
        float right_y = left_y + y_grid_size;

        if (std::abs(y - left_y) < kMinPixelDistance) {
            return static_cast<T>(left_y);
        }
        else if (std::abs(y - right_y) < kMinPixelDistance) {
            return static_cast<T>(right_y);
        }
        else {
            return y;
        }
    }

    template<typename T>
    juce::Point<T> SnapPoint(juce::Point<T> point) {
        return juce::Point{ SnapX(point.x),SnapY(point.y) };
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LineGraphEditor)
};
}
