/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 21 Feb 2024 4:52:59pm
    Author:  mana

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

namespace snd {
void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    Slider::SliderStyle, Slider& s) {
    g.setColour(s.findColour(juce::Slider::ColourIds::trackColourId));
    g.drawHorizontalLine(y + height / 2, x, x + width);

    g.setColour(s.findColour(juce::Slider::ColourIds::thumbColourId));
    g.fillRect(sliderPos, y + height * 0.25f, 0.1f * width, height * 0.5f);

    g.setColour(s.findColour(juce::Slider::ColourIds::trackColourId));
    g.drawRect(sliderPos, y + height * 0.25f, 0.1f * width, height * 0.5f);
}
}