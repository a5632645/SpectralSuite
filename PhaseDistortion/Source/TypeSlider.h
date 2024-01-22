/*
  ==============================================================================

    TypeSlider.h
    Created: 18 Jan 2024 3:12:11pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/PluginParameters.h"

class TypeSlider : public juce::Component {
public:
    TypeSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~TypeSlider() override = default;

    void paint(Graphics&) override;
    void resized() override;
private:
    using SliderAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

    Label type_label;
    ComboBox type_combo_box;
    std::unique_ptr<SliderAttachment> type_attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TypeSlider)
};

