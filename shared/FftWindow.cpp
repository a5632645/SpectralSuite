/*
  ==============================================================================

    FftWindow.cpp
    Created: 20 Jul 2024 4:39:48pm
    Author:  uavcs

  ==============================================================================
*/

#include "FftWindow.h"

static constexpr auto kTwoPi = juce::MathConstants<FftDecimal>::twoPi;

void FftWindow::setWindowLen(int size) {
    if (size == m_windowLen) 
    {
        return;
    }

    m_windowLen = size;
    m_data.resize(size);
    fillWindow(m_data, size, m_winType);
}

void FftWindow::SetWindowType(FftWindowType newType)
{
    if (newType == m_winType)
    {
        return;
    }

    m_winType = newType;
    fillWindow(m_data, m_windowLen, m_winType);
}

void FftWindow::fillWindow(std::vector<FftDecimal>& table, int size, FftWindowType type)
{
    switch (type) {
    case FftWindowType::HANN: {
        fillHann(table, size);
        break;
    }
    case BLACKMAN: {
        fillBlackman(table, size);
        break;
    }
    case HAMMING: {
        fillHamming(table, size);
        break;
    }
    case BLACKMAN_HARRIS: {
        fillBlackmanHarris(table, size);
        break;
    }
    default:
        jassertfalse;
        break;
    }
}

void FftWindow::fillHann(std::vector<FftDecimal>& table, int size)
{
    float w;
    float invert_Cos;
    float max = size - 1;
    for (int n = 0; n < size; ++n) {
        //hann window -- inverted cosine
        w = kTwoPi * (float(n) / max);
        invert_Cos = 1.f - cos(w);
        //normalise to 0 to 1
        table[n] = 0.5f * invert_Cos;
    }
}

void FftWindow::fillBlackman(std::vector<FftDecimal>& table, int size)
{
    float max = size - 1;
    for (int n = 0; n < size; ++n) {
        float w1 = kTwoPi * (float(n) / max);
        float w2 = kTwoPi * 2.0 * (float(n) / max);
        float c1 = 0.5 * cos(w1);
        float c2 = 0.08 * cos(w2);
        table[n] = 0.42 - (c1 + c2);
    }
}

void FftWindow::fillHamming(std::vector<FftDecimal>& table, int size)
{
    float max = size + 1.0;
    for (int n = 0; n < size; ++n) {
        float w = kTwoPi * (float(n) / max);
        table[n] = 0.53836 - 0.46164 * cos(w);
    }
}

void FftWindow::fillBlackmanHarris(std::vector<FftDecimal>& table, int size)
{
    float max = size - 1;
    for (int n = 0; n < size; ++n) {
        float increment = (float(n) / max);
        float w1 = kTwoPi * increment;
        float w2 = kTwoPi * 2.0 * increment;
        float w3 = kTwoPi * 6.0 * increment;

        float c1 = 0.48829 * cos(w1);
        float c2 = 0.14128 * cos(w2);
        float c3 = 0.01168 * cos(w3);

        table[n] = 0.35875 - c1 + c2 - c3;
    }
}
