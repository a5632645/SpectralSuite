/*
  ==============================================================================

    HorizontalSlider.h
    Created: 21 Feb 2024 4:52:14pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../shared/PluginParameters.h"

namespace snd {
class HorizontalSlider : public juce::Component {
public:
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    HorizontalSlider(const juce::String& id, PluginParameters& param);
    ~HorizontalSlider() override;

    void resized() override;

    juce::Slider& GetSlider() { return slider_; }
private:
    std::unique_ptr<Attachment> attachment_;
    juce::Label label_;
    juce::Slider slider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HorizontalSlider)
};
}