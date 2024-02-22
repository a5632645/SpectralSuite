/*
  ==============================================================================

    PDProcessor.cpp
    Created: 18 Jan 2024 3:49:48pm
    Author:  mana

  ==============================================================================
*/

#include <cmath>
#include "SNDProcessor.h"
#include "JuceHeader.h"

namespace snd {
SNDProcessor::SNDProcessor(int fftSize, int hopSize, int offset, int sampleRate, std::shared_ptr<PhaseBuffer> phaseBuffer)
    : PhaseVocoder(fftSize, hopSize, offset, sampleRate, phaseBuffer) {
}

void SNDProcessor::spectral_process(const PolarVector& in, PolarVector& out, int bins) {
    out[0].m_mag = in[0].m_mag;
    out[0].m_phase = in[0].m_phase;
    delay_time_ms_.skip(getHopSize());

    switch (delay_mode_) {
    case DelayMode::PHASE_DELAY:
        PhaseDelayProcess(in, out, bins);
        break;
    case DelayMode::GROUP_DELAY:
        GroupDelayProcess(in, out, bins);
        break;
    default:
        jassertfalse;
        break;
    }
}

void SNDProcessor::PhaseDelayProcess(const PolarVector& in, PolarVector& out, int bins) {
    const float sample_rate_div_1000 = getSampleRate() / 1000.0f;
    const float delay_time_sv = delay_time_ms_.getNextValue();
    const float delay_time_ms = time_div_10_ ? delay_time_sv * 0.1f : delay_time_sv;
    const float max_delay_num_samples = delay_time_ms * sample_rate_div_1000;
    const float min_delay_num_samples = time_mirror_ ? -max_delay_num_samples : 0.0f;

    for (int i = 1; i < bins; ++i) {
        const float normalize_frequency = static_cast<float>(i) / static_cast<float>(bins) * std::numbers::pi_v<float>;
        const float sample_delay = -std::lerp(min_delay_num_samples, max_delay_num_samples, ptr_phase_shift_buffer_->at(i));

        out[i].m_mag = in[i].m_mag;
        out[i].m_phase = in[i].m_phase + sample_delay * normalize_frequency;
    }
}

void SNDProcessor::GroupDelayProcess(const PolarVector& in, PolarVector& out, int bins) {
    const float sample_rate_div_1000 = getSampleRate() / 1000.0f;
    const float delay_time_sv = delay_time_ms_.getNextValue();
    const float delay_time_ms = time_div_10_ ? delay_time_sv * 0.1f : delay_time_sv;
    const float max_delay_num_samples = delay_time_ms * sample_rate_div_1000;
    const float min_delay_num_samples = time_mirror_ ? -max_delay_num_samples : 0.0f;
    const float delta_frequency = std::numbers::pi_v<float> / static_cast<float>(bins);
    float current_phase_shift = group_delay_begin_phase_;

    for (int i = 1; i < bins; ++i) {
        out[i].m_mag = in[i].m_mag;
        out[i].m_phase = in[i].m_phase + current_phase_shift;

        const float sample_delay = -std::lerp(min_delay_num_samples, max_delay_num_samples, ptr_phase_shift_buffer_->at(i));
        current_phase_shift += sample_delay * delta_frequency;
    }
}
}