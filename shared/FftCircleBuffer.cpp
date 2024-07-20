/*
  ==============================================================================

    FftCircleBuffer.cpp
    Created: 19 Jul 2024 9:52:27pm
    Author:  mana

  ==============================================================================
*/

#include "FftCircleBuffer.h"

FftCircleBuffer::FftCircleBuffer(std::unique_ptr<SpectralAudioProcessorInteractor> processor, int fftSize, int overlay, float sampleRate)
    : m_spectralProcessor(std::move(processor)) {
    m_sampleRate = sampleRate;
    SetFftNumOverlay(overlay);
    SetFftSize(fftSize);
}

void FftCircleBuffer::processBlock(std::vector<BusAudioData>& in, BusAudioData& out, int blockSize) {
    jassert(blockSize == m_hopSize);
    const auto inBusNum = in.size();
    const auto inChannelNum = in[0].size();

    // copy a block
    for (size_t i = 0; i < in.size(); ++i) {
        auto& busIn = in[i];
        auto& writeBus = m_input[i];
        for (size_t j = 0; j < busIn.size(); ++j) {
            std::copy(busIn[j].cbegin(), busIn[j].cend(), writeBus[j].begin() + m_inputWriteOffset);
        }
    }
    m_inputWriteCount += blockSize;
    m_inputWriteOffset += blockSize;
    m_inputWriteOffset &= (m_ioBufferSize - 1); // limit

    if (m_inputWriteCount >= m_fftSize) {
        // float into complex
        for (size_t bus = 0; bus < inBusNum; ++bus) {
            auto& busData = m_input[bus];
            for (size_t ch = 0; ch < inChannelNum; ++ch) {
                auto it = std::transform(busData[ch].cbegin() + m_inputReadOffset,
                                         busData[ch].cend(),
                                         m_fftCpxIn.begin(),
                                         [](float v) {return Cpx{ v,0.0f }; });
                std::transform(busData[ch].cbegin(),
                               busData[ch].cbegin() + m_inputReadOffset,
                               it,
                               [](float v) {return Cpx{ v,0.0f }; });
                m_window.applyWindow(m_fftCpxIn);

                // do fft
                m_fft->transform(m_fftCpxIn.data(), m_fftCpxOut.data());
                // cpx to polar plus
                auto& channelData = m_polarInput[bus][ch];
                for (int i = 0; i < m_spectralDataLen; ++i) {
                    channelData.polar[i].m_mag = std::abs(m_fftCpxOut[i + 1]) / (m_fftSize / 2.0f); // ignore dc
                    channelData.polar[i].m_phase = std::arg(m_fftCpxOut[i + 1]);
                }
            }
        }
        m_inputReadOffset += blockSize;
        m_inputReadOffset &= (m_ioBufferSize - 1); // limit
        m_inputWriteCount -= blockSize; // step hop size

        // freq processing
        if (m_fftStyle == FftStyle::DEFAULT) {
            for (size_t bus = 0; bus < inBusNum; ++bus) {
                for (size_t ch = 0; ch < inChannelNum; ++ch) {
                    auto& channelData = m_polarInput[bus][ch];
                    for (int i = 0; i < m_spectralDataLen; ++i) {
                        channelData.freqs[i] = (i + 1.0f) * m_sampleRate / m_fftSize;
                    }
                }
            }
        }
        else if (m_fftStyle == FftStyle::PVOC) {
        }

        // spectral processing
        //m_spectralProcessor->prepareProcess(nullptr);
        m_spectralProcessor->spectralProcess(m_polarInput, m_polarOutput);

        // post frequency processing
        if (m_fftStyle == FftStyle::PVOC) {
        }

        // spectral to time domain
        for (size_t ch = 0; ch < inChannelNum; ++ch) {
            std::fill(m_fftCpxIn.begin(), m_fftCpxIn.end(), Cpx{});
            for (int i = 0; i < m_spectralDataLen; ++i) {
                m_fftCpxIn[i] = std::polar(m_polarOutput[ch].polar[i].m_mag, m_polarOutput[ch].polar[i].m_phase);
            }

            // ifft
            m_ifft->transform(m_fftCpxIn.data(), m_fftCpxOut.data());
            m_window.applyWindow(m_fftCpxOut);

            // overlay add output
            auto can_write = m_ioBufferSize - m_outputWriteOffset;
            std::transform(m_fftCpxOut.cbegin(), m_fftCpxOut.cbegin() + can_write,
                           m_output[ch].cbegin() + m_outputWriteOffset,
                           m_output[ch].begin() + m_outputWriteOffset,
                           [](Cpx const& cpx, float v) {return cpx.real() + v; });
            std::transform(m_fftCpxOut.cbegin() + can_write, m_fftCpxOut.cend(),
                           m_output[ch].cbegin(),
                           m_output[ch].begin(),
                           [](Cpx const& cpx, float v) {return cpx.real() + v; });
        }
        m_outputWriteOffset += blockSize;
        m_outputWriteOffset &= (m_ioBufferSize - 1); // limit
        m_outputWriteCount += blockSize;
    }

    // read output
    if (m_outputWriteCount >= blockSize) {
        for (int ch = 0; ch < inChannelNum; ++ch) {
            std::copy_n(m_output[ch].cbegin() + m_outputReadOffset, blockSize, out[ch].begin());
            std::fill_n(m_output[ch].begin() + m_outputReadOffset, blockSize, 0.0f);
        }
        m_outputReadOffset += blockSize;
        m_outputReadOffset &= (m_fftSize - 1); // limit
        m_outputWriteCount -= blockSize;
    }
}

void FftCircleBuffer::SetFftSize(int newFftSize)
{
    if (m_fftSize == newFftSize) {
        return;
    }

    if (newFftSize > m_fftSize) { // expand io buffer
        m_ioBufferSize = newFftSize;
        auto busLayout = m_spectralProcessor->getProcessorBusWant();
        m_input.resize(busLayout.inputBusCount);
        for (auto& bus : m_input) {
            bus.resize(busLayout.channelPerInputBus);
            for (auto& ch : bus) {
                ch.resize(m_ioBufferSize);
            }
        }
        m_output.resize(busLayout.outputBusChannelCount);
        for (auto& ch : m_output) {
            ch.resize(m_ioBufferSize);
        }
    }

    m_fft = std::make_unique<kissfft<FftDecimal>>(newFftSize, false);
    m_ifft = std::make_unique<kissfft<FftDecimal>>(newFftSize, true);

    { // resize io polar
        auto busLayout = m_spectralProcessor->getProcessorBusWant();
        m_polarInput.resize(busLayout.inputBusCount);
        for (auto& bus : m_polarInput) {
            bus.resize(busLayout.channelPerInputBus);
            for (auto& ch : bus) {
                ch.freqs.resize(newFftSize);
                ch.polar.resize(newFftSize);
            }
        }
        m_polarOutput.resize(busLayout.outputBusChannelCount);
        for (auto& ch : m_polarOutput) {
            ch.freqs.resize(newFftSize);
            ch.polar.resize(newFftSize);
        }
    }

    // resize fft block
    m_fftCpxIn.resize(newFftSize);
    m_fftCpxOut.resize(newFftSize);

    // set others
    SetFftHop(newFftSize / m_numOverlay);
    m_window.setWindowLen(newFftSize);
    m_fftSize = newFftSize;
    m_spectralDataLen = newFftSize / 2;
}

void FftCircleBuffer::SetFftNumOverlay(int size)
{
    m_numOverlay = size;
    SetFftHop(m_fftSize / m_numOverlay);
}

void FftCircleBuffer::SetFftHop(int size)
{
    m_hopSize = size;
}
