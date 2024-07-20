#pragma once

#include <vector>
#include "FftWindowType.h"
#include "Types.h"

class FftWindow {
public:
    void setWindowLen(int size);
    void SetWindowType(FftWindowType newType);
    
    template<typename T>
    void applyWindow(std::vector<T>& vec) {
        jassert(vec.size() == m_windowLen);
        std::transform(m_data.cbegin(),  m_data.cend(),
                       vec.begin(),
                       vec.begin(),
                       [](auto a, auto& b) {return a * b; });
    }
private:
    static void fillWindow(std::vector<FftDecimal>& table, int size, FftWindowType type);
    static void fillHann(std::vector<FftDecimal>& table, int size);
    static void fillBlackman(std::vector<FftDecimal>& table, int size);
    static void fillHamming(std::vector<FftDecimal>& table, int size);
    static void fillBlackmanHarris(std::vector<FftDecimal>& table, int size);

    int m_windowLen{};
    FftWindowType m_winType{ FftWindowType::HANN };
    std::vector<FftDecimal> m_data;
};