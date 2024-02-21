/*
  ==============================================================================

    TitledCheckBox.cpp
    Created: 21 Feb 2024 4:52:32pm
    Author:  mana

  ==============================================================================
*/

#include "TitledCheckBox.h"

namespace snd {
TitledCheckBox::TitledCheckBox(const juce::String& id, PluginParameters& param) {
    const auto* p = param.getParameterChoice(id);

    label_.setText(p->name, juce::dontSendNotification);
    addAndMakeVisible(label_);

    addAndMakeVisible(checkbox_);

    attachment_.reset(param.createButtonAttachment(id, checkbox_));
}

TitledCheckBox::TitledCheckBox(const juce::String& pure_text) {
    label_.setText(pure_text, juce::dontSendNotification);
    addAndMakeVisible(label_);

    addAndMakeVisible(checkbox_);
}

TitledCheckBox::~TitledCheckBox() {
    attachment_ = nullptr;
}

void TitledCheckBox::resized() {
    auto b = getLocalBounds();
    checkbox_.setBounds(b.removeFromRight(getHeight()));
    label_.setBounds(b);
}
}