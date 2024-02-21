/*
  ==============================================================================

    utli.h
    Created: 21 Feb 2024 4:17:32pm
    Author:  mana

  ==============================================================================
*/

#pragma once
namespace snd {
namespace utli {
template<typename type>
static type blend_01_interpole(type x, type power) {
    static constexpr auto one = static_cast<type>(1);
    static constexpr auto two = static_cast<type>(2);
    const type up = x * (one - power);
    const type down = one - power * (two * x - one);
    return up / down;
}

template<typename type>
static type linear_interpole(type y_left, type y_right, type x, type power = {}) {
    return std::lerp(y_left, y_right, blend_01_interpole(x, power));
}
}
}