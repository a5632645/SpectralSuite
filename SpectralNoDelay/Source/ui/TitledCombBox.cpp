/*
  ==============================================================================

    TitledCombBox.cpp
    Created: 21 Feb 2024 4:52:44pm
    Author:  mana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TitledCombBox.h"

namespace snd {
TitledCombBox::TitledCombBox(const juce::String& id, PluginParameters& param) {
    const auto* p = param.getParameterChoice(id);

    label_.setText(p->name, juce::dontSendNotification);
    addAndMakeVisible(label_);

    combobox_.addItemList(p->choices, 1);
    addAndMakeVisible(combobox_);

    attachment_.reset(param.createComboBoxAttachment(id, combobox_));
}

TitledCombBox::~TitledCombBox() {
    attachment_ = nullptr;
}

void TitledCombBox::resized() {
    label_.setBoundsRelative(0.0f, 0.0f, 0.3f, 1.0f);
    combobox_.setBoundsRelative(0.3f, 0.0f, 0.7f, 1.0f);
}
}