/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 21 Feb 2024 4:52:59pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace snd {
class CustomLookAndFeel
    : public juce::LookAndFeel_V4 {
public:
    void drawLinearSlider(Graphics&, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        Slider::SliderStyle, Slider&) override;
};

inline static CustomLookAndFeel sCustomLookAndFeel;
}