/*
  ==============================================================================

    FftCircleBuffer.h
    Created: 19 Jul 2024 9:52:27pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include "Types.h"
#include "SpectralAudioProcessorInteractor.h"
#include "FftStyleChoiceAdapter.h"

class FftCircleBuffer {
public:
    FftCircleBuffer(std::unique_ptr<SpectralAudioProcessorInteractor> processor,
                    int fftSize, int fftHop, float sampleRate);

    void processBlock(std::vector<BusAudioData>& in, BusAudioData& out, int blockSize);
    decltype(auto) getProcessor() { return (m_spectralProcessor); }
private:
    std::unique_ptr<SpectralAudioProcessorInteractor> m_spectralProcessor;
    std::unique_ptr<kissfft<FftDecimal>> m_fft;
    std::unique_ptr<kissfft<FftDecimal>> m_ifft;
    // input
    std::vector<BusAudioData> m_input;
    int m_inputWriteOffset{};
    int m_inputReadOffset{};
    int m_inputWriteCount{};
    // output
    BusAudioData m_output;
    int m_outputWriteOffset{};
    int m_outputReadOffset{};
    int m_outputWriteCount{};
    int m_hopSize{};
    int m_fftSize{};
    int m_spectralDataLen{};
    float m_sampleRate{};

    // spectral data
    FftStyle m_fftStyle{ FftStyle::DEFAULT };
    std::vector<BusPolarPlusData> m_polarInput;
    BusPolarPlusData m_polarOutput;
    ChannelCpxData m_fftCpxIn;
    ChannelCpxData m_fftCpxOut;
};