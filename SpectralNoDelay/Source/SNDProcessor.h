/*
  ==============================================================================

    PDProcessor.h
    Created: 18 Jan 2024 3:49:48pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include "../../shared/PhaseVocoder.h"
#include <vector>
#include <numbers>
#include <random>

namespace snd {
enum class DelayMode {
    PHASE_DELAY = 0,
    GROUP_DELAY
};

constexpr float kDefaultDelayTime = 10.0f;
constexpr float kBeginPhase = 0.0f;

class SNDProcessor : public PhaseVocoder {
public:
    SNDProcessor(int fftSize, int hopSize, int offset, int sampleRate, std::shared_ptr<PhaseBuffer> phaseBuffer);
    ~SNDProcessor() override = default;

    void spectral_process(const PolarVector& in, PolarVector& out, int bins) override;

    void SetPhaseShiftBuffer(std::vector<float>* ptr) { ptr_phase_shift_buffer_ = ptr; }
    void SetDelayMode(DelayMode mode) { delay_mode_ = mode; }
    void SetDelayTime(float ms) { delay_time_ms_ = ms; }
    void SetTimeMirror(bool mirror) { time_mirror_ = mirror; }
    void SetTimeDiv10(bool scale_down) { time_div_10_ = scale_down; }
    void SetGroupDelayBeginAngle(float angle_phase) { group_delay_begin_phase_ = angle_phase * std::numbers::pi_v<float> / 180.0f; }
private:
    void PhaseDelayProcess(const PolarVector& in, PolarVector& out, int bins);
    void GroupDelayProcess(const PolarVector& in, PolarVector& out, int bins);

    std::vector<float>* ptr_phase_shift_buffer_{};
    DelayMode delay_mode_{ DelayMode::PHASE_DELAY };
    float delay_time_ms_{ kDefaultDelayTime };
    float group_delay_begin_phase_{ kBeginPhase };
    bool time_mirror_{ false };
    bool time_div_10_{ false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SNDProcessor);
};
}