#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"

namespace snd{
class CurveComponent;
class CurveEditor;
}

namespace snd {
class GDContainer : public ParameterContainerComponent {
public:
    GDContainer(std::shared_ptr<PluginParameters> valueTreeState, juce::Colour textColour, int textBoxHeight);
    ~GDContainer() override;

    std::shared_ptr<PluginParameters> getPluginParameters() override {
        return valueTreeState_;
    }

    const int getComponentHeight() override;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void OnShowLineTextEditorChanged();
    bool IsShowLineTextEditor() const;

    std::shared_ptr<PluginParameters> valueTreeState_;
    std::unique_ptr<CurveComponent> line_graphy_editor_;
    std::unique_ptr<CurveEditor> line_text_editor_;
    std::unique_ptr<juce::Component> show_text_editor_checkbox_;

    std::vector<std::shared_ptr<juce::Component>> param_components_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GDContainer)
};
}