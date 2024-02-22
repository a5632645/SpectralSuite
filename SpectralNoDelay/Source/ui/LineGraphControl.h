/*
  ==============================================================================

    LineGraphControl.h
    Created: 22 Feb 2024 7:31:18pm
    Author:  mana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LinePointsUI.h"
#include "TextToggleButton.h"

namespace snd {
class LineGraphControl : public juce::Component {
public:
    LineGraphControl(LineGraphEditor& editor);

    void resized() override;
private:
    LineGraphEditor& editor_;

    TextToggleButton grid_button_;
    TextToggleButton snap_button_;
    juce::Slider x_grid_slider_;
    juce::Slider y_grid_slider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LineGraphControl)
};
}