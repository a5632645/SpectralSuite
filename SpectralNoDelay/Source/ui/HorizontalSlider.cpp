/*
  ==============================================================================

    HorizontalSlider.cpp
    Created: 21 Feb 2024 4:52:14pm
    Author:  mana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HorizontalSlider.h"
#include "CustomLookAndFeel.h"

namespace snd {
HorizontalSlider::HorizontalSlider(const juce::String& id, PluginParameters& param) {
    const auto* p = param.getParameterChoice(id);

    label_.setText(p->name, juce::dontSendNotification);
    addAndMakeVisible(label_);

    slider_.setLookAndFeel(&sCustomLookAndFeel);
    addAndMakeVisible(slider_);

    attachment_.reset(param.createSliderAttachment(id, slider_));
}

HorizontalSlider::~HorizontalSlider() {
    slider_.setLookAndFeel(nullptr);
    attachment_ = nullptr;
}

void HorizontalSlider::resized() {
    label_.setBoundsRelative(0.0f, 0.0f, 0.3f, 1.0f);
    slider_.setBoundsRelative(0.3f, 0.0f, 0.7f, 1.0f);
}
}