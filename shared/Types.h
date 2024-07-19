#pragma once
#include "kissfft.hh"
#include "Polar.h"

typedef float FftDecimal;
typedef std::vector<Polar<FftDecimal>> PolarVector;
// Cpx will be used in this class as a shorthand for the complex type
typedef kissfft<FftDecimal>::cpx_type Cpx;

using ChannelAudioData = std::vector<float>;
using BusAudioData = std::vector<ChannelAudioData>;

using ChannelPolarData = PolarVector;
using BusPolarData = std::vector<ChannelPolarData>;