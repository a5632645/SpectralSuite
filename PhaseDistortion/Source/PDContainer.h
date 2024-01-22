#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"
#include "PDParameters.h"
#include "AmountSlider.h"
#include "TypeSlider.h"

class SliderContainer : public ParameterContainerComponent {
public:
    SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer() override = default;

    std::shared_ptr<PluginParameters> getPluginParameters() override {
        return valueTreeState;
    }

    const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 2; }
    void paint(Graphics&) override;
    void resized() override;

private:
    std::shared_ptr<PluginParameters> valueTreeState;
    AmountSlider amount_slider;
    TypeSlider type_combo_box;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderContainer)
};
