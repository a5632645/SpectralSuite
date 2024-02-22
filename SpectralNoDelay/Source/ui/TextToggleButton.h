/*
  ==============================================================================

    TextToggleButton.h
    Created: 22 Feb 2024 8:07:06pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace snd {
class TextToggleButton : public juce::ToggleButton {
public:
    TextToggleButton() = default;

    void paint(juce::Graphics& g) override {
        if (getToggleState()) {
            g.fillAll(juce::Colours::darkorange);
        }
        else {
            g.fillAll(juce::Colours::grey);
        }

        g.setColour(juce::Colours::white);
        g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

        g.setColour(juce::Colours::black);
        g.setFont(14.0f);
        g.drawText(getButtonText(), getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextToggleButton)
};
}