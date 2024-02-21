#include "SNDParameters.h"
#include "SNDProcessor.h"

namespace snd {
PDParameters::PDParameters(juce::AudioProcessor* processor)
    : PluginParameters(processor) {
    createAndAddParameter(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("delay_mode", 0),
        "delay_mode",
        GetDelayModeChoiceStr(),
        0
    ));
    createAndAddParameter(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("pitch_mode", 0),
        "pitch_mode",
        false
    ));
    createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("delay_time", 0),
        "delay_time",
        juce::NormalisableRange<float>(-1000.0f, 1000.0f, 0.1f),
        kDefaultDelayTime,
        "ms"
    ));
    createAndAddParameter(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("gd_begin_angle", 0),
        "gd_begin_phase",
        juce::NormalisableRange<float>(0.0f, 360.0f, 1.0f),
        0.0f,
        "degree"
    ));
    createAndAddParameter(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("time_mirror", 0),
        "time_mirror",
        false
    ));
    createAndAddParameter(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("time_div_10", 0),
        "time_div_10",
        false
    ));
}

juce::AudioParameterChoice* PDParameters::GetDelayModeParam() {
    return getParameterChoice("delay_mode");
}

juce::AudioParameterBool* PDParameters::GetPitchModeParam() {
    return static_cast<juce::AudioParameterBool*>(getParameter("pitch_mode"));
}

juce::AudioParameterFloat* PDParameters::GetDelayTimeParam() {
    return static_cast<juce::AudioParameterFloat*>(getParameter("delay_time"));
}

juce::AudioParameterFloat* PDParameters::GetGDBeginAngleParam() {
    return static_cast<juce::AudioParameterFloat*>(getParameter("gd_begin_angle"));
}

juce::AudioParameterBool* PDParameters::GetTimeMirrorParam() {
    return static_cast<juce::AudioParameterBool*>(getParameter("time_mirror"));
}

juce::AudioParameterBool* PDParameters::GetTimeDiv10Param() {
    return static_cast<juce::AudioParameterBool*>(getParameter("time_div_10"));
}

void PDParameters::replaceState(const ValueTree& newState) {
    PluginParameters::replaceState(newState);

    auto value_tree = newState.getChildWithName("curve_points_data");
    if (!value_tree.isValid()) {
        line_points_.fill_default();
        return;
    }

    std::vector<LinePoints::Point> points;
    for (const auto& point_vt : value_tree) {
        const auto& x = point_vt["x"];
        const auto& y = point_vt["y"];
        const auto& power = point_vt["power"];

        if (x.isUndefined() || y.isUndefined() || power.isUndefined()) {
            line_points_.fill_default();
            return;
        }
        else {
            points.emplace_back(LinePoints::Point{
                .x = x,
                .y = y,
                .power = power });
        }
    }

    line_points_.fill_points(points);
}

ValueTree PDParameters::copyState() {
    juce::ValueTree point_value_tree{ "curve_points_data" };

    for (const auto points = line_points_.get_points(); const auto & p : points) {
        point_value_tree.appendChild(juce::ValueTree("point")
            .setProperty("x", p.x, nullptr)
            .setProperty("y", p.y, nullptr)
            .setProperty("power", p.power, nullptr), nullptr);
    }

    auto vt = PluginParameters::copyState();
    vt.appendChild(point_value_tree, nullptr);
    return vt;
}

juce::StringArray GetDelayModeChoiceStr() {
    return {
        "phase delay",
        "group delay"
    };
}

}