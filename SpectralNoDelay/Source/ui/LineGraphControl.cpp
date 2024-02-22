/*
  ==============================================================================

    LineGraphControl.cpp
    Created: 22 Feb 2024 7:31:18pm
    Author:  mana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LineGraphControl.h"

namespace snd {
LineGraphControl::LineGraphControl(LineGraphEditor& editor)
    : editor_(editor) {
    
    grid_button_.setButtonText("grid");
    grid_button_.setToggleState(true, juce::dontSendNotification);
    grid_button_.onClick = [this]() {
        editor_.ShowGrid(grid_button_.getToggleState());
    };
    addAndMakeVisible(grid_button_);

    snap_button_.setButtonText("snap");
    snap_button_.setToggleState(true, juce::dontSendNotification);
    snap_button_.onClick = [this]() {
        editor_.EnableSnapGrid(snap_button_.getToggleState());
    };
    addAndMakeVisible(snap_button_);

    x_grid_slider_.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    x_grid_slider_.setRange(1, 16, 1);
    x_grid_slider_.setValue(4, juce::dontSendNotification);
    x_grid_slider_.onValueChange = [this]() {
        editor_.SetXGrid(static_cast<int>(x_grid_slider_.getValue()));
    };
    addAndMakeVisible(x_grid_slider_);

    y_grid_slider_.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    y_grid_slider_.setRange(1, 16, 1);
    y_grid_slider_.setValue(4, juce::dontSendNotification);
    y_grid_slider_.onValueChange = [this]() {
        editor_.SetYGrid(static_cast<int>(y_grid_slider_.getValue()));
    };
    addAndMakeVisible(y_grid_slider_);
}

void LineGraphControl::resized() {
    juce::FlexBox fb;
    fb.items.add(juce::FlexItem(grid_button_).withWidth(30).withHeight(getHeight()));
    fb.items.add(juce::FlexItem(snap_button_).withWidth(30).withHeight(getHeight()));
    fb.items.add(juce::FlexItem(x_grid_slider_).withFlex(1).withHeight(getHeight()));
    fb.items.add(juce::FlexItem(y_grid_slider_).withFlex(1).withHeight(getHeight()));

    auto b = getLocalBounds();
    b.removeFromRight(20);
    fb.performLayout(b);
}
}