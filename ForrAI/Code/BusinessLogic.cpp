#include "BusinessLogic.hpp"

fa::BusinessLogic::BusinessLogic(ApplicationContext& context) : m_Context(context) {
    fa::Serializer serializer{m_Perceptron};
    serializer.Read("ForrAI-final.bin");
}

void fa::BusinessLogic::Update() {
    if (!m_Context.is_canvas_dirty) return;

    std::array<float, 10> predictions = m_Perceptron.forward(m_Context.canvas_pixels);
    m_Context.current_predictions     = predictions;

    m_Context.is_canvas_dirty = false;
}
