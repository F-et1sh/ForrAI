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

void fa::Perceptron::backward(const std::array<neuron_value_t, 10>& error_signal) {
    auto last_layer_deltas = this->GetLayerDeltas(m_LayersTopology.back());

    for (std::size_t i = 0; i < error_signal.size(); i++)
        last_layer_deltas[i] = error_signal[i];

    for (std::size_t i = m_LayersTopology.size() - 1; i > 0; i--) {
        const auto& this_layer_topology = m_LayersTopology[i];
        const auto& past_layer_topology = m_LayersTopology[i - 1];

        auto this_layer_values      = this->GetLayerValues(this_layer_topology);
        auto this_layer_deltas      = this->GetLayerDeltas(this_layer_topology);
        auto this_layer_grad_biases = this->GetLayerGradBiases(this_layer_topology);
        auto past_layer_values      = this->GetLayerValues(past_layer_topology);

        if (i < m_LayersTopology.size() - 1) {
            const auto& next_layer_topology = m_LayersTopology[i + 1];
            auto        next_layer_deltas   = this->GetLayerDeltas(next_layer_topology);

            for (std::size_t j = 0; j < this_layer_topology.values_count; j++) {
                neuron_value_t error_sum{};

                for (std::size_t k = 0; k < next_layer_topology.values_count; k++) {
                    auto next_weights = this->GetNeuronWeights(next_layer_topology, this_layer_topology, k);
                    error_sum += next_layer_deltas[k] * next_weights[j];
                }

                this_layer_deltas[j] = (this_layer_values[j] > 0) ? error_sum : 0;
            }
        }

        for (std::size_t j = 0; j < this_layer_topology.values_count; j++) {
            auto grad_weights = GetNeuronGradWeights(this_layer_topology, past_layer_topology, j);

            for (std::size_t k = 0; k < past_layer_topology.values_count; k++) {
                grad_weights[k] += this_layer_deltas[j] * past_layer_values[k];
            }

            this_layer_grad_biases[j] = this_layer_deltas[j];
        }
    }
}

void fa::Perceptron::step(neuron_value_t learning_rate) {
    for (std::size_t i = 1; i < m_LayersTopology.size(); i++) {
        const auto& this_layer_topology = m_LayersTopology[i];
        const auto& past_layer_topology = m_LayersTopology[i - 1];

        auto this_layer_biases      = this->GetLayerBiases(this_layer_topology);
        auto this_layer_grad_biases = this->GetLayerGradBiases(this_layer_topology);

        for (std::size_t j = 0; j < this_layer_topology.values_count; j++) {

            auto this_layer_neuron_weights      = this->GetNeuronWeights(this_layer_topology, past_layer_topology, j);
            auto this_layer_neuron_grad_weights = this->GetNeuronGradWeights(this_layer_topology, past_layer_topology, j);

            for (std::size_t k = 0; k < this_layer_neuron_weights.size(); k++)
                this_layer_neuron_weights[k] -= learning_rate * this_layer_neuron_grad_weights[k];

            this_layer_biases[j] -= learning_rate * this_layer_grad_biases[j];
        }
    }
}

void fa::Perceptron::create_layer(std::size_t neurons_count) {
    std::size_t past_layer_neurons_count = m_LayersTopology.empty() ? 0 : m_LayersTopology.back().values_count;
    std::size_t weights_to_add           = neurons_count * past_layer_neurons_count;

    auto& new_layer         = m_LayersTopology.emplace_back();
    new_layer.values_start  = m_Values.size();
    new_layer.weights_start = m_Weights.size();
    new_layer.values_count  = neurons_count;
    new_layer.weights_count = weights_to_add;

    m_Values.resize(m_Values.size() + neurons_count, 0.0f);
    m_Deltas.resize(m_Deltas.size() + neurons_count, 0.0f);
    m_Biases.resize(m_Biases.size() + neurons_count, 0.0f);
    m_GradBiases.resize(m_GradBiases.size() + neurons_count, 0.0f);

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
