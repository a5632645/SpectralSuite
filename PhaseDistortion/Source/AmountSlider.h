/*
  ==============================================================================

    AmountSlider.h
    Created: 18 Jan 2024 3:12:27pm
    Author:  mana

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/PluginParameters.h"

class AmountSlider : public juce::Component {
public:
    AmountSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~AmountSlider() override = default;

    void paint(Graphics&) override;
    void resized() override;
private:
    using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;

    Label amount_label;
    Slider amount_slider;
    std::unique_ptr<SliderAttachment> freqAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmountSlider)
};