/*
  ==============================================================================

    LineTextEditor.h
    Created: 6 Feb 2024 9:42:09pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../data/LinePoints.h"

namespace snd {
class CurveEditor
    : public juce::Component
    , private LinePoints::Listener {
public:
    explicit CurveEditor(LinePoints& generator);
    ~CurveEditor() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void clear_point_text();

    void load_point(size_t idx);

    size_t get_current_display_idx() const { return m_current_display_idx; }

private:
    void point_changed(LinePoints& generator, int idx) override;
    void point_added_at(LinePoints& generator, int idx_at, LinePoints::Point point) override;
    void point_removed(LinePoints& generator, int idx) override;
    void reloaded(LinePoints& generator) override;

    void save_point();
    void reset_curve();

    static constexpr auto k_invalid_idx = std::numeric_limits<size_t>::max();

    size_t m_current_display_idx{ k_invalid_idx };
    LinePoints& m_generator;
    juce::Label m_index_label;
    juce::Label m_x_label;
    juce::Label m_y_label;
    juce::Label m_power_label;
    juce::TextEditor m_index_editor;
    juce::TextEditor m_x_editor;
    juce::TextEditor m_y_editor;
    juce::TextEditor m_power_editor;
    juce::TextButton m_default_button;
    juce::TextButton m_save_button;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CurveEditor);
};
}