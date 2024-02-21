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
class CurveEditor;
}

class CurveComponent
    : public juce::Component
    , private LinePoints::Listener
    , private LineGenerator::Listener {
public:
    explicit CurveComponent(LinePoints& generator);
    ~CurveComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void remove_point_component(size_t idx);
    void xy_point_moved(size_t idx);
    void power_point_moved(size_t idx);
    void power_point_reset(size_t idx);

    void mouseDoubleClick(const juce::MouseEvent& e) override;
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
    LinePoints& line_points_;
    LineGenerator line_generator_;
    juce::OwnedArray<detail::PointComponent> m_points;
    juce::OwnedArray<detail::PowerPointComponent> m_power_points;

    std::unique_ptr<juce::MouseListener> m_point_processor;
    std::unique_ptr<juce::MouseListener> m_power_point_processor;

    // ========================================================================
    void DataChanged(LineGenerator* ptr_generator, int begin, int end) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CurveComponent)
};
}
