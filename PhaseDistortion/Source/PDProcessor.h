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
#include <random>

enum class PhaseDistortionType : size_t {
    SPECTURM_SAME = 0,
    SPECTURM_RANDOM,
    SPECTURM_DISTORTION_1,
    DISPERSION,
    NUM_DISTORTION_TYPE
};

class PhaseDistortionProcess : public PhaseVocoder {
public:
    PhaseDistortionProcess(int fftSize, int hopSize, int offset, int sampleRate, std::shared_ptr<PhaseBuffer> phaseBuffer);
    ~PhaseDistortionProcess() override = default;

    void spectral_process(const PolarVector& in, PolarVector& out, int bins) override;

    void set_phase_distortion_type(int type);
    void set_phase_distortion_type(PhaseDistortionType type);

    void set_distortion_amount(float amount);
    void set_dynamic(bool dynamic);
private:
    void rand_table();

    void spectrum_random_distortion(PolarVector const& in, PolarVector& out, int bins);

    PhaseDistortionType m_phase_distortion_type{ PhaseDistortionType::SPECTURM_SAME };
    std::vector<float> m_random_table;
    std::random_device m_random_device;
    bool m_update_random_table{};
    float m_distortion_amount{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseDistortionProcess);
};