/*
  ==============================================================================

    TitledCheckBox.h
    Created: 21 Feb 2024 4:52:32pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../shared/PluginParameters.h"

namespace snd {
class TitledCheckBox : public juce::Component {
public:
    using Attachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    TitledCheckBox(const juce::String& id, PluginParameters& param);
    TitledCheckBox(const juce::String& pure_text);
    ~TitledCheckBox() override;

    void resized() override;

    juce::ToggleButton& GetCheckBox() { return checkbox_; }
private:
    std::unique_ptr<Attachment> attachment_;
    juce::Label label_;
    juce::ToggleButton checkbox_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitledCheckBox)
};
}