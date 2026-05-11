#include "BusinessLogic.hpp"

fa::BusinessLogic::BusinessLogic(ApplicationContext& context) : m_Context(context) {}

void fa::BusinessLogic::Update() {
    this->checkCanvasDirty();
    this->checkIsReading();
}

void fa::BusinessLogic::checkCanvasDirty() {
    if (!m_Context.is_canvas_dirty) return;

    auto max_element = std::ranges::max_element(m_Context.canvas_pixels);
    if (*max_element != 0) {
        std::array<float, 10> predictions = m_Perceptron.forward(m_Context.canvas_pixels);
        m_Context.current_predictions     = predictions;
    }
    else {
        m_Context.current_predictions = {};
    }

    m_Context.is_canvas_dirty = false;
}

void fa::BusinessLogic::checkIsReading() {
    if (!m_Context.is_reading) return;

    fa::Serializer serializer{ m_Perceptron };
    serializer.Read(m_Context.path_to_file, m_Context);

    m_Context.is_reading = false;

    m_Context.is_canvas_dirty = true; // update predictions
}
