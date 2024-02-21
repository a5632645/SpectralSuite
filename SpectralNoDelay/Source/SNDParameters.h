#pragma once

#include "../../shared/PluginParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "data/LinePoints.h"

namespace snd {
juce::StringArray GetDelayModeChoiceStr();
}

namespace snd {
class PDParameters : public PluginParameters {
public:
    explicit PDParameters(juce::AudioProcessor* processor);

    LinePoints& GetLinePoints() { return line_points_; }
    LinePoints* GetLinePointsPtr() { return &line_points_; }

    juce::AudioParameterChoice* GetDelayModeParam();
    juce::AudioParameterBool* GetPitchModeParam();
    juce::AudioParameterFloat* GetDelayTimeParam();
    juce::AudioParameterFloat* GetGDBeginAngleParam();
    juce::AudioParameterBool* GetTimeMirrorParam();
    juce::AudioParameterBool* GetTimeDiv10Param();

    void replaceState(const ValueTree& newState) override;
    ValueTree copyState() override;
private:
    LinePoints line_points_;
};
}