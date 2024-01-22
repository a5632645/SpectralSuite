#include "PDParameters.h"

PDParameters::PDParameters(AudioProcessor * processor) :
    PluginParameters(processor) {
    createAndAddParameter(
                          std::make_unique<AudioParameterFloat>(
                                                                ParameterID("amount", 1),
                                                                "distortion_amount",
                                                                juce::NormalisableRange<float>(0.0F,1.0F,0.001F),
                                                                0.0F
                          )
    );
    createAndAddParameter(std::make_unique<AudioParameterChoice>(
        ParameterID("type", 1),
        "Distortion type",
        juce::StringArray{ "same","random","distortion1","dispersion" },
        0
    ));
    createAndAddParameter(std::make_unique<AudioParameterBool>(ParameterID("dynamic", 1),
                                                               "dynamic",
                                                               false
    ));
}
