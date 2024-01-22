#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "PDParameters.h"

class PDInteractor : public SpectralAudioProcessorInteractor {
public:
    
    PDInteractor(int numOverlaps, std::shared_ptr<PDParameters> params) : SpectralAudioProcessorInteractor(numOverlaps), m_params(params) {
        bind_parameters(*params);
    }
    
    void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
    std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount) override;
            

private:
    void bind_parameters(PDParameters& param);

    std::shared_ptr<PDParameters> m_params;

    float* m_distortion_amount = nullptr;
    juce::AudioParameterChoice* m_distortion_type = nullptr;
    juce::AudioParameterBool* m_dynamic = nullptr;
};
