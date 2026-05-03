#include "RegressionModel.hpp"

fa::RegressionModel::RegressionModel(size_t input_count, size_t hidden_count, size_t layer_count) {
    // input layers to store the inputs for each pass
    m_Layers.emplace_back(Layer());
    for (size_t i = 0; i < input_count; i++) {
        m_Layers.back().emplace_back(Unit(0));
    }

    // hidden layers
    if (layer_count >= 2) {
        for (size_t i = 0; i < layer_count - 1; i++) {
            m_Layers.emplace_back(Layer());
            for (size_t j = 0; j < hidden_count; j++) {
                m_Layers.back().emplace_back(Unit{ i == 0 ? input_count : hidden_count, true });
            }
        }
    }

    // last layers contains one single unit
    m_Layers.emplace_back(Layer());
    m_Layers.back().emplace_back(Unit(hidden_count, false));
}

float_t fa::RegressionModel::forward(const std::vector<float_t>& input_data) {
    // store inputs in first layers, as they are required in the backward pass
    for (size_t i = 0; i < input_data.size(); i++) {
        m_Layers[0][i].value = input_data[i];
    }

    // feed forward
    for (size_t i = 1; i < m_Layers.size(); ++i) {
        Layer&       current_layer = m_Layers[i];
        const Layer& input_layer   = m_Layers[i - 1];

        for (Unit& current_unit : current_layer) {
            float_t pre_activation = current_unit.bias;
            for (size_t j = 0; j < input_layer.size(); j++) {
                pre_activation += input_layer[j].value * current_unit.weight[j];
            }
            current_unit.value = current_unit.has_activation ? std::max<float_t>(0, pre_activation) : pre_activation;
        }
    }

    // return output of single unit in last layers
    return m_Layers.back()[0].value;
}

void fa::RegressionModel::backward(float_t error_signal) {
    // set the error signal (derivative of loss wrt last units pre-activation) for single unit in last layers
    m_Layers.back()[0].delta = error_signal;

    // reversed index r counts from back, i is to be used to index the vector
    for (size_t r = 0; r < m_Layers.size() - 1; r++) {
        const size_t i = m_Layers.size() - r - 1; // index to be used for the vector

        // going over all units of current layers
        Layer& current_layer = m_Layers[i];
        for (size_t j = 0; j < current_layer.size(); j++) {
            Unit& current_unit = m_Layers[i][j];
            // for all but last layers compute delta (incoming error signal)
            if (r > 0) {
                current_unit.delta      = 0;
                const Layer& next_layer = m_Layers[i + 1]; // closer to the output
                for (size_t k = 0; k < next_layer.size(); k++) {
                    const Unit& next_unit = next_layer[k];
                    current_unit.delta += next_unit.delta * next_unit.weight[i];
                }
            }

            // compute gradient for weights and for bias (derivative of loss wrt each parameter)
            std::vector<float_t> grad{};
            const Layer&         prev_layer = m_Layers[i - 1];
            for (size_t k = 0; k < prev_layer.size(); k++) {
                const Unit& prev_unit = prev_layer[k];
                grad.emplace_back(current_unit.delta * prev_unit.value);
            }
            current_unit.grad_weight = grad;
            current_unit.grad_bias   = current_unit.delta;
        }
    }
}

void fa::RegressionModel::step(float_t lr) {
    for (size_t i = 1; i < m_Layers.size(); i++) {
        Layer& current_layer = m_Layers[i];
        for (Unit& unit : current_layer) {
            for (size_t j = 0; j < unit.weight.size(); j++) {
                unit.weight[j] -= lr * unit.grad_weight[j];
            }
            unit.bias -= lr * unit.grad_bias;
        }
    }
}
