/*
  ==============================================================================

    LinePointsUI.cpp
    Created: 6 Feb 2024 1:27:36pm
    Author:  mana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LinePointsUI.h"
#include "../utli.h"

constexpr auto kHalfPointSize = 8;
constexpr auto kPointSize = kHalfPointSize * 2;
constexpr auto kDrawCircleSize = 8;

namespace snd {
namespace detail {
/**
 * @brief A component that display the point's x and y
 */
class PointComponent
    : public juce::Component {
public:
    void set_index(size_t idx) { m_index = idx; }
    void inc_index() { ++m_index; }
    void dec_index() { --m_index; }
    size_t get_index() const { return m_index; }

    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::white);
        g.fillEllipse(getLocalBounds().withSizeKeepingCentre(kDrawCircleSize, kDrawCircleSize).toFloat());

        g.setColour(juce::Colours::black);
        g.drawEllipse(getLocalBounds().withSizeKeepingCentre(kDrawCircleSize, kDrawCircleSize).toFloat(), 1.0f);
    }
private:
    size_t m_index{};
};

/**
 * @brief A component that display the point's power
 */
class PowerPointComponent
    : public juce::Component {
public:
    void set_index(size_t idx) { m_index = idx; }
    void inc_index() { ++m_index; }
    void dec_index() { --m_index; }
    size_t get_index() const { return m_index; }

    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::black);
        g.fillEllipse(getLocalBounds().withSizeKeepingCentre(kDrawCircleSize, kDrawCircleSize).toFloat());

        g.setColour(juce::Colours::white);
        g.drawEllipse(getLocalBounds().withSizeKeepingCentre(kDrawCircleSize, kDrawCircleSize).toFloat(), 1.0f);
    }
private:
    size_t m_index{};
};

/**
 * @brief Process logic of xy point
 */
class PointProcessor
    : public juce::MouseListener {
public:
    explicit PointProcessor(CurveComponent& c)
        : m_curve_component(c) {
    }

    void mouseDown(const juce::MouseEvent& event) override {
        m_component_dragger.startDraggingComponent(event.eventComponent, event);
    }

    void mouseDrag(const juce::MouseEvent& event) override {
        m_component_dragger.dragComponent(event.eventComponent, event, nullptr);
        mouseUp(event);
    }

    void mouseUp(const juce::MouseEvent& event) override {
        const auto* ptr_point_c = static_cast<PointComponent*>(event.eventComponent);
        m_curve_component.xy_point_moved(ptr_point_c->get_index());
    }

    void mouseDoubleClick(const juce::MouseEvent& event) override {
        const auto* ptr_point_c = static_cast<PointComponent*>(event.eventComponent);
        m_curve_component.remove_point_component(ptr_point_c->get_index());
    }
private:
    juce::ComponentDragger m_component_dragger;
    CurveComponent& m_curve_component;
};

/**
 * @brief Process logic of power point
 */
class PowerPointProcessor
    : public juce::MouseListener {
public:
    explicit PowerPointProcessor(CurveComponent& c)
        : m_curve_component(c) {
    }

    void mouseDown(const juce::MouseEvent& event) override {
        m_component_dragger.startDraggingComponent(event.eventComponent, event);
        m_old_x = event.eventComponent->getX();
    }

    void mouseDrag(const juce::MouseEvent& event) override {
        m_component_dragger.dragComponent(event.eventComponent, event, nullptr);
        event.eventComponent->setBounds(event.eventComponent->getBounds().withX(m_old_x));
        mouseUp(event);
    }

    void mouseUp(const juce::MouseEvent& event) override {
        const auto* ptr_point_c = static_cast<PointComponent*>(event.eventComponent);
        m_curve_component.power_point_moved(ptr_point_c->get_index());
    }

    void mouseDoubleClick(const juce::MouseEvent& e) override {
        const auto* ptr_point_c = static_cast<PointComponent*>(e.eventComponent);
        m_curve_component.power_point_reset(ptr_point_c->get_index());
    }
private:
    int m_old_x{};
    juce::ComponentDragger m_component_dragger;
    CurveComponent& m_curve_component;
};
}

// ============================================================================
CurveComponent::CurveComponent(LinePoints& generator)
    : line_points_(generator)
    , line_generator_(generator) {
    m_point_processor = std::make_unique<detail::PointProcessor>(*this);
    m_power_point_processor = std::make_unique<detail::PowerPointProcessor>(*this);

    line_generator_.AddListener(this);
    line_points_.add_listener(this);
    rebuild_interface();
}

CurveComponent::~CurveComponent() {
    line_points_.remove_listener(this);
    line_generator_.RemoveListener(this);
}

void CurveComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::green);

    const auto& buffer = line_generator_.GetRenderBuffer();
    juce::Path path;

    path.preallocateSpace(buffer.size());
    for (int x = 0; const float ratio_y : buffer) {
        const auto x_f = static_cast<float>(x);
        const float y = (1.0f - ratio_y) * getHeight();

        if (x == 0) {
            path.startNewSubPath(x_f, y);
        }
        else {
            path.lineTo(x_f, y);
        }

        ++x;
    }

    g.strokePath(path, juce::PathStrokeType(3.0f));
}

void CurveComponent::resized() {
    const auto num_point = m_points.size();
    for (int i = 0; i < num_point; ++i) {
        adjust_point_position(i);
    }

    const auto num_power_point = m_power_points.size();
    for (int i = 0; i < num_power_point; ++i) {
        adjust_power_point_position(i);
    }

    line_generator_.SetBufferSize(std::max(0, getWidth()));
}

void CurveComponent::remove_point_component(size_t idx) {
    /* can not delete head and tail point */
    if (idx == 0 || idx == m_points.size() - 1) {
        return;
    }

    line_points_.remove(idx);
}

void CurveComponent::mouseDoubleClick(const juce::MouseEvent& e) {
    /* generate a point at here */
    const auto bound = getLocalBounds().toFloat();
    LinePoints::Point point{
        .x = e.mouseDownPosition.x / bound.getWidth(),
        .y = 1.0f - e.mouseDownPosition.y / bound.getHeight(),
        .power = 0.0f
    };

    line_points_.add_point(point);
}

void CurveComponent::rebuild_interface() {
    const auto num_point = line_points_.GetNumPoints();
    m_points.clear();
    m_power_points.clear();

    for (int idx = 0; idx < num_point; ++idx) {
        auto point_c = create_point_component();
        point_c->set_index(idx);
        m_points.add(std::move(point_c));
    }

    for (int idx = 0; idx < num_point - 1; ++idx) {
        auto power_point_c = create_power_point_component();
        power_point_c->set_index(idx);
        m_power_points.add(std::move(power_point_c));
    }

    for (auto* ptr_point_c : m_points) {
        addAndMakeVisible(ptr_point_c);
    }

    for (auto* ptr_power_point_c : m_power_points) {
        addAndMakeVisible(ptr_power_point_c);
    }

    resized();
}

void CurveComponent::xy_point_moved(size_t idx) {
    if (idx == 0) {
        /* First point can only move up and down */
        auto* pc = m_points.getFirst();
        pc->setBounds(pc->getBounds().withCentre(
            pc->getBounds().getCentre().withX(0)));
    }
    else if (idx == m_points.size() - 1) {
        /* last point can only on and down too */
        auto* pc = m_points.getLast();
        pc->setBounds(pc->getBounds().withCentre(
            pc->getBounds().getCentre().withX(getWidth())
        ));
    }
    else /* limit position */ {
        auto* ptr_c = m_points[static_cast<int>(idx)];
        const auto* ptr_before_c = m_points[idx - 1];
        const auto* ptr_after_c = m_points[idx + 1];

        const auto x = std::clamp(ptr_c->getX(), ptr_before_c->getX(), ptr_after_c->getX());
        const auto y = std::clamp(ptr_c->getY(), -kHalfPointSize, getHeight() - kHalfPointSize);
        ptr_c->setBounds(ptr_c->getBounds().withX(x).withY(y));
    }

    const auto* ptr_c = m_points[static_cast<int>(idx)];
    /* use center instead of x */
    const auto center = ptr_c->getBounds().getCentre().toFloat();
    const float ratio_x = center.x / getWidth();
    const float ratio_y = 1.0f - center.y / getHeight();

    auto point = line_points_.get_point(idx);
    point.x = ratio_x;
    point.y = ratio_y;
    line_points_.change(idx, point);
}

void CurveComponent::adjust_point_position(size_t idx) const {
    const auto point = line_points_.get_point(idx);
    const auto x = static_cast<int>(getWidth() * point.x - kHalfPointSize);
    const auto y = static_cast<int>(std::round(getHeight() * (1.0f - point.y) - kHalfPointSize));
    m_points[static_cast<int>(idx)]->setBounds(x, y, kPointSize, kPointSize);
}

void CurveComponent::adjust_power_point_position(size_t idx) const {
    if (idx >= m_power_points.size()) {
        return;
    }

    const auto point_left = line_points_.get_point(idx);
    const auto point_right = line_points_.get_point(idx + 1);
    const auto ratio_x = std::midpoint(point_left.x, point_right.x);
    const auto x = static_cast<int>(getWidth() * ratio_x);
    const auto y = static_cast<int>(std::round(getHeight() 
        * (1.0f - utli::linear_interpole(point_left.y, point_right.y, 0.5f, point_left.power))));
    m_power_points[idx]->setBounds(x - kHalfPointSize, y - kHalfPointSize, kPointSize, kPointSize);
}

void CurveComponent::power_point_moved(size_t idx) {
    const auto component_y = static_cast<float>(m_power_points[idx]->getBounds().getCentreY());
    const auto y_ratio = 1.0f - component_y / getHeight();
    const auto point_left_y_ratio = line_points_.get_point(idx).y;
    const auto point_right_y_ratio = line_points_.get_point(idx + 1).y;
    const auto reverse_blend_y = (y_ratio - point_left_y_ratio) / (point_right_y_ratio - point_left_y_ratio);
    const auto power = (0.5f - reverse_blend_y) / 0.5f;

    auto point = line_points_.get_point(idx);
    point.power = power;
    line_points_.change(idx, point);
}

void CurveComponent::power_point_reset(size_t idx) {
    auto point = line_points_.get_point(idx);
    point.power = 0.0f;
    line_points_.change(idx, point);
}

std::unique_ptr<detail::PointComponent> CurveComponent::create_point_component() {
    auto p = std::make_unique<detail::PointComponent>();
    p->addMouseListener(m_point_processor.get(), false);
    return p;
}

std::unique_ptr<detail::PowerPointComponent> CurveComponent::create_power_point_component() {
    auto p = std::make_unique<detail::PowerPointComponent>();
    p->addMouseListener(m_power_point_processor.get(), false);
    return p;
}

void CurveComponent::DataChanged(LineGenerator* /*ptr_generator*/, int begin, int end) {
    repaint(juce::Rectangle<int>(begin, 0, end - begin, getHeight()));
}

void CurveComponent::point_changed(LinePoints& generator, int idx) {
    adjust_point_position(idx);
    adjust_power_point_position(idx);
    adjust_power_point_position(idx - 1);
}

void CurveComponent::point_added_at(LinePoints& generator, int idx_at, LinePoints::Point point) {
    auto* pc = m_points.insert(idx_at, create_point_component());
    auto* ppc = m_power_points.insert(idx_at, create_power_point_component());

    pc->set_index(idx_at);
    for (int i = idx_at + 1; i < m_points.size(); ++i) {
        m_points[i]->inc_index();
    }

    ppc->set_index(idx_at);
    for (int i = idx_at + 1; i < m_power_points.size(); ++i) {
        m_power_points[i]->inc_index();
    }

    addAndMakeVisible(pc);
    addAndMakeVisible(ppc);
    adjust_point_position(idx_at);
    adjust_power_point_position(idx_at);
    adjust_power_point_position(idx_at - 1);
}

void CurveComponent::point_removed(LinePoints& generator, int idx) {
    m_points.remove(idx);
    m_power_points.remove(idx);

    for (auto i = idx; i < m_points.size(); ++i) {
        m_points[i]->dec_index();
    }

    for (auto i = idx; i < m_power_points.size(); ++i) {
        m_power_points[i]->dec_index();
    }

    adjust_power_point_position(idx);
    adjust_power_point_position(idx - 1);
}

void CurveComponent::reloaded(LinePoints& generator) {
    rebuild_interface();
}
}