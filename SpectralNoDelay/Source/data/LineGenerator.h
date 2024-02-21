/*
  ==============================================================================

    LineGenerator.h
    Created: 21 Feb 2024 10:58:08am
    Author:  mana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include "LinePoints.h"

namespace snd {
class LineGenerator
    : private LinePoints::Listener {
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void DataChanged(LineGenerator* ptr_generator, int begin, int end) = 0;
    };

    explicit LineGenerator(LinePoints& lp);
    ~LineGenerator() override;

    void SetBufferSize(size_t size);

    void AddListener(Listener* l) { listeners_.add(l); }
    void RemoveListener(Listener* l) { listeners_.remove(l); }

    const std::vector<float>& GetRenderBuffer() const { return render_buffer_; }
private:
    // implement for line_points::Listener
    void point_changed(LinePoints& generator, int idx) override;
    void point_added_at(LinePoints& generator, int idx_at, LinePoints::Point point) override;
    void point_removed(LinePoints& generator, int idx) override;
    void reloaded(LinePoints& generator) override;

    void RenderPoint(int begin, int end);

    juce::ListenerList<Listener> listeners_;
    std::vector<float> render_buffer_;
    LinePoints& line_points_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LineGenerator);
};
}