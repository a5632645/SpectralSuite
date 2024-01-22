/*
  ==============================================================================

    AmountSlider.cpp
    Created: 18 Jan 2024 3:12:27pm
    Author:  mana

  ==============================================================================
*/

#include "AmountSlider.h"

AmountSlider::AmountSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) {
    amount_slider.setSliderStyle(Slider::LinearHorizontal);
    amount_slider.setRange(0.0, 1.0, 0.01);
    amount_slider.setValue(6.0);
    
    amount_slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
    amount_slider.setTooltip("The distortion amount of this vst");

    amount_slider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    addAndMakeVisible(&amount_slider);
    freqAttachment.reset(valueTreeState->createSliderAttachment("amount", amount_slider));

    amount_label.setText("Distortion amount", NotificationType::dontSendNotification);
    amount_label.attachToComponent(&amount_slider, false);
    amount_label.setColour(Label::ColourIds::textColourId, textColour);
    addAndMakeVisible(amount_label);
}

void AmountSlider::paint(Graphics& g) {
    g.fillAll(Colours::white);
}

void AmountSlider::resized() {
    int y = 0;
    amount_label.setBounds(0, y, getWidth() / 3, 30);

    y += amount_label.getBounds().getBottom() + 10;;
    amount_slider.setBounds(0, y, getWidth(), getHeight() / 2);
}
