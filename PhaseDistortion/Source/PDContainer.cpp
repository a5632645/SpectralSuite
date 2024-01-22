#include "PDContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) :
	amount_slider(valueTreeState, textColour, textBoxHeight),
	type_combo_box(valueTreeState, textColour, textBoxHeight) {
	this->valueTreeState = valueTreeState;
	addAndMakeVisible(amount_slider);
	addAndMakeVisible(type_combo_box);
}

void SliderContainer::paint(Graphics& g) {
	g.fillAll(Colours::white);
}

void SliderContainer::resized() {
	int y = 0;
	amount_slider.setBounds(0, y, getWidth(), getHeight() / 2);

	y += 100;
	type_combo_box.setBounds(0, y, getWidth(), getHeight() / 2);
}
