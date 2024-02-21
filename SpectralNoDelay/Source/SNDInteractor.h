#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "SNDParameters.h"
#include "data/LineGenerator.h"

namespace snd {
class PDInteractor
    : public SpectralAudioProcessorInteractor
    , private LineGenerator::Listener {
public:
    PDInteractor(int numOverlaps, std::shared_ptr<PDParameters> params);
    ~PDInteractor() override;

    void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
    std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount) override;

    void onFftSizeChanged() override;
private:
    // implement for LineGenerator::Listener
    void DataChanged(LineGenerator* ptr_generator, int begin, int end) override;

    void BindParams(PDParameters& param);
    void GenerateF2Pmap();
    void GeneratePitchBuffer(int begin, int end);

    std::shared_ptr<PDParameters> params_;

    LineGenerator line_generator_;
    std::vector<float> hz_phase_shift_buffer_;
    std::vector<float> pitch_phase_shift_buffer_;
    std::vector<float> f2p_map_;

    juce::AudioParameterBool* ptr_pitch_mode_{};
    juce::AudioParameterBool* ptr_time_mirror_{};
    juce::AudioParameterChoice* ptr_delay_mode_{};
    juce::AudioParameterFloat* ptr_delay_time_{};
    juce::AudioParameterFloat* ptr_gp_begin_angle_{};
    juce::AudioParameterBool* ptr_time_div_10_{};
};
}