/*
  ==============================================================================

    TitledCombBox.h
    Created: 21 Feb 2024 4:52:44pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../shared/PluginParameters.h"

namespace snd {
class TitledCombBox : public juce::Component {
public:
    using Attachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    TitledCombBox(const juce::String& id, PluginParameters& param);
    ~TitledCombBox() override;

    void resized() override;

    juce::ComboBox& GetComboBox() { return combobox_; }
private:
    std::unique_ptr<Attachment> attachment_;
    juce::Label label_;
    juce::ComboBox combobox_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitledCombBox)
};
}