/*
  ==============================================================================

    LineTextEditor.cpp
    Created: 6 Feb 2024 9:42:09pm
    Author:  mana

  ==============================================================================
*/

#include <JuceHeader.h>
#include <charconv>
#include "LineTextEditor.h"

namespace snd {

CurveEditor::CurveEditor(LinePoints& generator)
    : m_generator(generator)
    , m_index_label({}, "Point Index")
    , m_x_label({}, "x: 0~1")
    , m_y_label({}, "y: 0~1")
    , m_power_label({}, "power: +-0.99")
    , m_save_button("save", "save changes to the curve")
    , m_default_button("reset", "reset curve to default") {

    addAndMakeVisible(m_index_label);
    addAndMakeVisible(m_x_label);
    addAndMakeVisible(m_y_label);
    addAndMakeVisible(m_power_label);
    addAndMakeVisible(m_index_editor);
    addAndMakeVisible(m_x_editor);
    addAndMakeVisible(m_y_editor);
    addAndMakeVisible(m_power_editor);
    addAndMakeVisible(m_save_button);
    addAndMakeVisible(m_default_button);

    generator.add_listener(this);

    m_save_button.onClick = std::bind_front(&CurveEditor::save_point, this);
    m_default_button.onClick = std::bind_front(&CurveEditor::reset_curve, this);
    m_index_editor.setInputRestrictions(0, "0123456789");
    m_x_editor.setInputRestrictions(0, "-.0123456789");
    m_y_editor.setInputRestrictions(0, "-.0123456789");
    m_power_editor.setInputRestrictions(0, "-.0123456789");

    load_point(0);
}

CurveEditor::~CurveEditor() {
    m_generator.remove_listener(this);
}

void CurveEditor::clear_point_text() {
    m_current_display_idx = k_invalid_idx;

    m_index_editor.clear();
    m_x_editor.clear();
    m_y_editor.clear();
    m_power_editor.clear();
}

void CurveEditor::load_point(size_t idx) {
    m_current_display_idx = idx;
    const auto point = m_generator.get_point(idx);

    m_index_editor.setText(juce::String(idx));
    m_x_editor.setText(juce::String(point.x));
    m_y_editor.setText(juce::String(point.y));
    m_power_editor.setText(juce::String(point.power));
}

void CurveEditor::resized() {
    static constexpr auto label_ratio = 0.5f;
    static constexpr auto editor_ratio = 1.0f - label_ratio;

    m_index_label.setBoundsRelative(0.0f, 0.0f, label_ratio, 0.2f);
    m_x_label.setBoundsRelative(0.0f, 0.2f, label_ratio, 0.2f);
    m_y_label.setBoundsRelative(0.0f, 0.4f, label_ratio, 0.2f);
    m_power_label.setBoundsRelative(0.0f, 0.6f, label_ratio, 0.2f);
    m_index_editor.setBoundsRelative(label_ratio, 0.0f, editor_ratio, 0.2f);
    m_x_editor.setBoundsRelative(label_ratio, 0.2f, editor_ratio, 0.2f);
    m_y_editor.setBoundsRelative(label_ratio, 0.4f, editor_ratio, 0.2f);
    m_power_editor.setBoundsRelative(label_ratio, 0.6f, editor_ratio, 0.2f);
    m_default_button.setBoundsRelative(0.05f, 0.8f, 0.2f, 0.2f);
    m_save_button.setBoundsRelative(0.65f, 0.8f, 0.2f, 0.2f);
}

void CurveEditor::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
}

void CurveEditor::point_changed(LinePoints& generator, int idx) {
    load_point(idx);
}

void CurveEditor::point_added_at(LinePoints& generator, int idx_at, LinePoints::Point point) {
    load_point(idx_at);
}

void CurveEditor::point_removed(LinePoints& generator, int idx) {
    if (m_current_display_idx == idx) {
        clear_point_text();
    }
}

void CurveEditor::reloaded(LinePoints& generator) {
    clear_point_text();
}

void CurveEditor::save_point() {
    static constexpr auto invalid_param = std::numeric_limits<float>::quiet_NaN();
    static constexpr auto juce_editor_to_number = [] <typename type> (const juce::TextEditor& te, type error_if) -> type {
        type res;
        const auto str = te.getText();
        auto [ptr, error_code] = std::from_chars(str.begin(), str.end(), res);

        if (error_code == std::errc()) {
            return res;
        }
        else {
            return error_if;
        }
    };

    LinePoints::Point point;
    const size_t idx = juce_editor_to_number(m_index_editor, k_invalid_idx);
    point.x = juce_editor_to_number(m_x_editor, invalid_param);
    point.y = juce_editor_to_number(m_y_editor, invalid_param);
    point.power = juce_editor_to_number(m_power_editor, invalid_param);

    if (idx == k_invalid_idx
        || std::isnan(point.x)
        || std::isnan(point.y)
        || std::isnan(point.power)) {
        DBG("[CurveEditor]: nan value get!");
        return;
    }

    m_generator.change(idx, point);
}

void CurveEditor::reset_curve() {
    m_generator.fill_default();
}
}