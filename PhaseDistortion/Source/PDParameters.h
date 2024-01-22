#pragma once


#include "../../shared/PluginParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"

class PDParameters : public PluginParameters {
public:
    explicit PDParameters(AudioProcessor* processor);
    
    float* getAmountParameter() const {
        return getRawParameterValue("amount");
    }
    
    juce::AudioParameterChoice* getTypeParameter() const {
        return getParameterChoice("type");
    }

    juce::AudioParameterBool* getDynamicParameter() const {
        return static_cast<juce::AudioParameterBool*>(getParameter("dynamic"));
    }
};
