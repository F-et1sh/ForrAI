#include "BusinessLogic.hpp"

fa::BusinessLogic::BusinessLogic(ApplicationContext& context) : m_Context(context) {}

void fa::BusinessLogic::Update() {
    this->checkCanvasDirty();
    this->checkIsReading();
}

void fa::BusinessLogic::checkCanvasDirty() {
    if (!m_Context.is_canvas_dirty) return;

    std::array<float, 10> predictions = m_Perceptron.forward(m_Context.canvas_pixels);
    m_Context.current_predictions     = predictions;

    m_Context.is_canvas_dirty = false;
}

void fa::BusinessLogic::checkIsReading() {
    if (!m_Context.is_reading) return;

    fa::Serializer serializer{ m_Perceptron };
    serializer.Read(m_Context.path_to_file);

    m_Context.is_reading = false;
}
