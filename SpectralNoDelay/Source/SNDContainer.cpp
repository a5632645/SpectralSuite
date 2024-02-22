#include "SNDContainer.h"
#include "SNDParameters.h"
#include "ui/LinePointsUI.h"
#include "ui/LineTextEditor.h"
#include "ui/HorizontalSlider.h"
#include "ui/TitledCheckBox.h"
#include "ui/TitledCombBox.h"

namespace snd {
GDContainer::GDContainer(std::shared_ptr<PluginParameters> valueTreeState, juce::Colour textColour, int textBoxHeight)
    : valueTreeState_(valueTreeState) {
    auto& plugin_param = *static_cast<PDParameters*>(valueTreeState.get());
    line_graphy_editor_ = std::make_unique<snd::LineGraphEditor>(plugin_param.GetLinePoints());
    addAndMakeVisible(line_graphy_editor_.get());

    line_text_editor_ = std::make_unique<snd::CurveEditor>(plugin_param.GetLinePoints());
    addChildComponent(line_text_editor_.get());

    line_graph_control_ = std::make_unique<LineGraphControl>(*line_graphy_editor_);
    addAndMakeVisible(line_graph_control_.get());

    show_text_editor_checkbox_ = std::make_unique<snd::TitledCheckBox>("show curve editor");
    static_cast<TitledCheckBox*>(show_text_editor_checkbox_.get())->GetCheckBox().onClick =
        std::bind_front(&GDContainer::OnShowLineTextEditorChanged, this);
    addAndMakeVisible(show_text_editor_checkbox_.get());

    param_components_.emplace_back(std::make_shared<snd::HorizontalSlider>("delay_time", *valueTreeState));
    param_components_.emplace_back(std::make_shared<snd::TitledCheckBox>("time_div_10", *valueTreeState));
    param_components_.emplace_back(std::make_shared<snd::TitledCheckBox>("time_mirror", *valueTreeState));
    param_components_.emplace_back(std::make_shared<snd::TitledCombBox>("delay_mode", *valueTreeState));
    param_components_.emplace_back(std::make_shared<snd::HorizontalSlider>("gd_begin_angle", *valueTreeState));
    param_components_.emplace_back(std::make_shared<snd::TitledCheckBox>("pitch_mode", *valueTreeState));

    for (const auto& p : param_components_) {
        addAndMakeVisible(p.get());
    }
}

GDContainer::~GDContainer() = default;

const int GDContainer::getComponentHeight() {
    return 500;
}

void GDContainer::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
}

void GDContainer::resized() {
    static constexpr int margin = 3;

    auto b = getLocalBounds();
    line_graph_control_->setBounds(b.removeFromTop(20));
    b.removeFromLeft(margin);
    b.removeFromRight(margin);

    line_graphy_editor_->setBounds(b.withHeight(getWidth() * 0.5f));
    b.removeFromTop(line_graphy_editor_->getHeight() + margin);

    show_text_editor_checkbox_->setBounds(b.removeFromTop(30));

    if (IsShowLineTextEditor()) {
        line_text_editor_->setBounds(b.removeFromTop(150));
    }

    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::column;
    for (const auto& p : param_components_) {
        fb.items.add(juce::FlexItem(*p).withWidth(b.getWidth()).withHeight(50));
    }
    fb.performLayout(b);
}

void GDContainer::OnShowLineTextEditorChanged() {
    line_text_editor_->setVisible(IsShowLineTextEditor());
    resized();
}

bool GDContainer::IsShowLineTextEditor() const {
    return static_cast<TitledCheckBox*>(show_text_editor_checkbox_.get())->GetCheckBox().getToggleState();
}
}