#include "SNDInteractor.h"
#include "../../shared/PhaseVocoder.h"
#include "../../shared/PhaseBuffer.h"

#include "../../shared/SpectralAudioPlugin.h"
#include "../../shared/utilities.h"

#include "SNDProcessor.h"
#include "pitchconv.h"
#include <ranges>

namespace snd {
PDInteractor::PDInteractor(int numOverlaps, std::shared_ptr<PDParameters> params)
    : SpectralAudioProcessorInteractor(numOverlaps)
    , params_(params)
    , line_generator_(params->GetLinePoints()) {
    BindParams(*params);
    line_generator_.AddListener(this);
}

PDInteractor::~PDInteractor() {
    line_generator_.RemoveListener(this);
}

std::unique_ptr<StandardFFTProcessor> PDInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount) {
    return std::make_unique<SNDProcessor>(fftSize, hopSize, hopSize * (index % numOverlaps), sampleRate, this->getPhaseBuffer());
}

void PDInteractor::onFftSizeChanged() {
    // Shit spectrum process only use half of fft size
    hz_phase_shift_buffer_.resize(getFftSize() / 2);
    pitch_phase_shift_buffer_.resize(getFftSize() / 2);
    GenerateF2Pmap();
    line_generator_.SetBufferSize(getFftSize() / 2);
    GeneratePitchBuffer(0, getFftSize() / 2);
}

void PDInteractor::BindParams(PDParameters& param) {
    ptr_pitch_mode_ = param.GetPitchModeParam();
    ptr_delay_mode_ = param.GetDelayModeParam();
    ptr_delay_time_ = param.GetDelayTimeParam();
    ptr_time_mirror_ = param.GetTimeMirrorParam();
    ptr_gp_begin_angle_ = param.GetGDBeginAngleParam();
    ptr_time_div_10_ = param.GetTimeDiv10Param();
}

void PDInteractor::GenerateF2Pmap() {
    const float nyquist_rate = static_cast<float>(getSampleRate()) / 2.0f;
    const float nyquist_pitch = pc::f2p(nyquist_rate);
    const int map_size = getFftSize() / 2;
    f2p_map_.resize(map_size);

    f2p_map_[0] = 0.0f;
    for (int i = 1; i < map_size; ++i) {
        const float frequency = static_cast<float>(i) / static_cast<float>(map_size) * nyquist_rate;
        const float pitch = pc::f2p(frequency);
        f2p_map_[i] = std::clamp(std::abs(pitch) / nyquist_pitch, 0.0f, 1.0f);
    }
}

void PDInteractor::GeneratePitchBuffer(int begin, int end) {
    const auto hz_length = hz_phase_shift_buffer_.size();
    for (int i = begin; i < end; ++i) {
        const float hz_lookup_position = f2p_map_[i] * (hz_length - 1.0f);
        const size_t lerp_1_pos = static_cast<size_t>(hz_lookup_position);
        const size_t lerp_2_pos = std::min(lerp_1_pos, hz_length - 1);
        const float lerp = hz_lookup_position - static_cast<float>(lerp_1_pos);
        pitch_phase_shift_buffer_[i] = std::lerp(hz_phase_shift_buffer_[lerp_1_pos],
            hz_phase_shift_buffer_[lerp_2_pos],
            lerp);
    }
}

void PDInteractor::DataChanged(LineGenerator* ptr_generator, int begin, int end) {
    using namespace std::ranges::views;
    std::ranges::copy(ptr_generator->GetRenderBuffer() | drop(begin) | take(end - begin),
        hz_phase_shift_buffer_.begin() + begin);
    GeneratePitchBuffer(begin, end);
}

void PDInteractor::prepareProcess(StandardFFTProcessor* spectralProcessor) {
    auto& processor = *((SNDProcessor*)spectralProcessor);

    if (ptr_pitch_mode_->get()) {
        processor.SetPhaseShiftBuffer(&pitch_phase_shift_buffer_);
    }
    else {
        processor.SetPhaseShiftBuffer(&hz_phase_shift_buffer_);
    }

    processor.SetDelayMode(static_cast<DelayMode>(ptr_delay_mode_->getIndex()));
    processor.SetDelayTime(ptr_delay_time_->get());
    processor.SetTimeMirror(ptr_time_mirror_->get());
    processor.SetGroupDelayBeginAngle(ptr_gp_begin_angle_->get());
    processor.SetTimeDiv10(ptr_time_div_10_->get());
}
}