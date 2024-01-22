/*
  ==============================================================================

    PDProcessor.cpp
    Created: 18 Jan 2024 3:49:48pm
    Author:  mana

  ==============================================================================
*/

#include "PDProcessor.h"

#include "JuceHeader.h"
#include <cmath>
#include <numbers>

template<typename T>
constexpr auto two_pi = static_cast<T>(TWOPI);

static void spectrum_same_distortion(PolarVector const& in, PolarVector& out, int bins, float amount);
static void spectrum_distortion_1(PolarVector const& in, PolarVector& out, int bins, float amount, float sample_rate);
static void spectrum_dispersion(PolarVector const& in, PolarVector& out, int bins, float amount);

static constexpr size_t RANDOM_TABLE_SIZE = 1024;

PhaseDistortionProcess::PhaseDistortionProcess(int fftSize, int hopSize, int offset, int sampleRate, std::shared_ptr<PhaseBuffer> phaseBuffer)
    :PhaseVocoder(fftSize, hopSize, offset, sampleRate, phaseBuffer) {
    m_random_table.resize(RANDOM_TABLE_SIZE + 1ULL);
    rand_table();
}

void PhaseDistortionProcess::spectral_process(const PolarVector& in, PolarVector& out, int bins) {
    if (m_update_random_table) {
        rand_table();
    }

    switch (m_phase_distortion_type) {
    case PhaseDistortionType::SPECTURM_SAME:
        spectrum_same_distortion(in, out, bins, m_distortion_amount);
        break;
    case PhaseDistortionType::SPECTURM_RANDOM:
        spectrum_random_distortion(in, out, bins);
        break;
    case PhaseDistortionType::SPECTURM_DISTORTION_1:
        spectrum_distortion_1(in, out, bins, m_distortion_amount, static_cast<float>(getSampleRate()));
        break;
    case PhaseDistortionType::DISPERSION:
        spectrum_dispersion(in, out, bins, m_distortion_amount);
        break;
    default:
        jassertfalse;
        break;
    }
}

void PhaseDistortionProcess::set_phase_distortion_type(int type) {
    jassert(type >= 0 && type < static_cast<int>(PhaseDistortionType::NUM_DISTORTION_TYPE));

    set_phase_distortion_type(static_cast<PhaseDistortionType>(type));
}

void PhaseDistortionProcess::set_phase_distortion_type(PhaseDistortionType type) {
    if (type == m_phase_distortion_type) {
        return;
    }

    m_phase_distortion_type = type;
}

void PhaseDistortionProcess::set_distortion_amount(float amount) {
    m_distortion_amount = amount;
}

void PhaseDistortionProcess::set_dynamic(bool dynamic) {
    m_update_random_table = dynamic;
}

void PhaseDistortionProcess::rand_table() {
    for (auto& s : m_random_table) {
        s = static_cast<float>(m_random_device()) / static_cast<float>(std::random_device::max());
    }
}

void PhaseDistortionProcess::spectrum_random_distortion(PolarVector const& in, PolarVector& out, int bins) {
    for (int i = 0; i < bins; ++i) {
        size_t const idx = static_cast<size_t>(static_cast<float>(i) / static_cast<float>(bins) * RANDOM_TABLE_SIZE);
        float const rd_val = m_random_table[idx];
        out[i].m_mag = in[i].m_mag;
        out[i].m_phase = in[i].m_phase + two_pi<float> * m_distortion_amount * rd_val;
    }
}

static void spectrum_same_distortion(PolarVector const& in, PolarVector& out, int bins, float amount) {
    for (int i = 0; i < bins; ++i) {
        out[i].m_mag = in[i].m_mag;
        out[i].m_phase = in[i].m_phase + amount * two_pi<float>;
    }
}

static void spectrum_distortion_1(PolarVector const& in, PolarVector& out, int bins, float amount, float sample_rate) {
    float const nyquist_rate = sample_rate * 0.5F;
    float const right_amount = 1.1F * amount;
    for (int i = 0; i < bins; ++i) {
        float const frequency = static_cast<float>(i) / static_cast<float>(bins) * nyquist_rate;
        out[i].m_mag = in[i].m_mag;
        out[i].m_phase = in[i].m_phase + std::pow(frequency, right_amount) * two_pi<float>;
    }
}

static void spectrum_dispersion(PolarVector const& in, PolarVector& out, int bins, float amount) {
    float dispersion_amount = juce::jmap(amount, -0.005F, 0.005F);
    for (int i = 0; i < bins; ++i) {
        auto const p = static_cast<float>(i);
        float const phase_increase = p * p;
        out[i].m_mag = in[i].m_mag;
        out[i].m_phase = in[i].m_phase + phase_increase * dispersion_amount * two_pi<float>;
    }
}
