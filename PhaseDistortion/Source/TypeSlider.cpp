/*
  ==============================================================================

    TypeSlider.cpp
    Created: 18 Jan 2024 3:12:11pm
    Author:  mana

  ==============================================================================
*/

#include "TypeSlider.h"

TypeSlider::TypeSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) {
    type_combo_box.setTooltip("The distortion type of this vst");

    type_combo_box.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    type_combo_box.addItemList({ "same","random","distortion1","dispersion"}, 1);
    addAndMakeVisible(&type_combo_box);
    type_attachment.reset(valueTreeState->createComboBoxAttachment("type", type_combo_box));

    type_label.setText("Distortion Type", NotificationType::dontSendNotification);
    type_label.attachToComponent(&type_combo_box, false);
    type_label.setColour(Label::ColourIds::textColourId, textColour);
    addAndMakeVisible(type_label);
}

void TypeSlider::paint(Graphics& g) {
    g.fillAll(Colours::white);
}

void TypeSlider::resized() {
    int y = 0;
    type_label.setBounds(0, y, getWidth() / 3, 30);

    y += type_label.getBounds().getBottom() + 10;;
    type_combo_box.setBounds(0, y, getWidth(), getHeight() / 2);
}
