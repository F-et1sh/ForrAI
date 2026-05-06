#include "Perceptron.hpp"

fa::Perceptron::Perceptron(std::size_t input_layer_neurons_count,
                           std::size_t hidden_layer_neurons_count,
                           std::size_t layers_count) {
    this->create_layer(input_layer_neurons_count);

    if (layers_count >= 2) {
        for (std::size_t i = 0; i < layers_count - 1; i++) {
            this->create_layer(hidden_layer_neurons_count);
        }
    }

    this->create_layer(10);
}

void fa::Perceptron::create_layer(std::size_t neurons_count) {
    std::size_t last_layer_neuron_count = m_Topology.empty() ? 0 : m_Topology.back().values_count;
    std::size_t weights_to_add          = neurons_count * last_layer_neuron_count;

    auto& new_layer         = m_Topology.emplace_back();
    new_layer.values_start  = m_Values.size();
    new_layer.weights_start = m_Weights.size();
    new_layer.values_count  = neurons_count;
    new_layer.weights_count = weights_to_add;

    m_Values.resize(m_Values.size() + neurons_count, 0.0f);
    m_Biases.resize(m_Biases.size() + neurons_count, 0.0f);
    m_GradBiases.resize(m_GradBiases.size() + neurons_count, 0.0f);
    m_Deltas.resize(m_Deltas.size() + neurons_count, 0.0f);

    if (weights_to_add > 0) {
        this->randomly_initialize_weight(m_Weights, weights_to_add);
        m_GradWeights.resize(m_GradWeights.size() + weights_to_add, 0.0f);
    }
}

void fa::Perceptron::randomly_initialize_weight(container_t<neuron_value_t>& dst, std::size_t elements_count) {
    static std::mt19937                            gen{ std::random_device{}() };
    std::uniform_real_distribution<neuron_value_t> uni{ -1.0, 1.0 };

    dst.reserve(dst.size() + elements_count);

    for (size_t i = 0; i < elements_count; i++) {
        dst.push_back(static_cast<neuron_value_t>(0.01 * uni(gen)));
    }
}
