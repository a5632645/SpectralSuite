#pragma once

#include "Types.h"
#include "SpectralAudioProcessorInteractor.h"
#include "FftStyleChoiceAdapter.h"
#include "FftWindow.h"

class FftCircleBuffer {
public:
    FftCircleBuffer(std::unique_ptr<SpectralAudioProcessorInteractor> processor,
                    int fftSize, int overlay, float sampleRate);

    void processBlock(std::vector<BusAudioData>& in, BusAudioData& out, int blockSize);
    decltype(auto) getProcessor() { return (m_spectralProcessor); }

    void SetFftSize(int size);
    int GetFftSize() const { return m_fftSize; }
    void SetFftNumOverlay(int size);
    int GetFftNumOverlay() const { return m_numOverlay; }
    int GetFftHop() const { return m_hopSize; }
private:
    void SetFftHop(int size);

    FftWindow m_window;
    std::unique_ptr<SpectralAudioProcessorInteractor> m_spectralProcessor;
    std::unique_ptr<kissfft<FftDecimal>> m_fft;
    std::unique_ptr<kissfft<FftDecimal>> m_ifft;
    // input
    int m_ioBufferSize{};
    std::vector<BusAudioData> m_input;
    int m_inputWriteOffset{};
    int m_inputReadOffset{};
    int m_inputWriteCount{};
    // output
    BusAudioData m_output;
    int m_outputWriteOffset{};
    int m_outputReadOffset{};
    int m_outputWriteCount{};
    int m_numOverlay{};
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