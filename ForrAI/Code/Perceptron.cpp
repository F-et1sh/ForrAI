#include "Perceptron.hpp"

fa::Perceptron::Perceptron(std::size_t input_layer_neurons_count, std::vector<std::size_t> hidden_layers) {
    this->create_layer(input_layer_neurons_count);
    for (auto e : hidden_layers) this->create_layer(e);
    this->create_layer(10);
}

void fa::Perceptron::backward(const std::array<neuron_value_t, 10>& error_signal) {
    //FA_SCOPE_TIMER("backward")

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
            std::ranges::fill(this_layer_deltas, 0.0);

            const auto& next_layer_topology = m_LayersTopology[i + 1];
            auto        next_layer_deltas   = this->GetLayerDeltas(next_layer_topology);

            for (std::size_t k = 0; k < next_layer_topology.values_count; k++) {
                auto           next_layer_weights = this->GetNeuronWeights(next_layer_topology, this_layer_topology, k);
                neuron_value_t next_layer_delta   = next_layer_deltas[k];

                for (std::size_t j = 0; j < this_layer_topology.values_count; j++) {
                    this_layer_deltas[j] += next_layer_delta * next_layer_weights[j];
                }
            }

            for (std::size_t j = 0; j < this_layer_topology.values_count; j++) {
                this_layer_deltas[j] = (this_layer_values[j] > 0) ? this_layer_deltas[j] : 0;
            }
        }

        for (std::size_t j = 0; j < this_layer_topology.values_count; j++) {
            neuron_value_t this_layer_delta = this_layer_deltas[j];
            if (this_layer_delta == 0) continue;

            auto grad_weights = GetNeuronGradWeights(this_layer_topology, past_layer_topology, j);
            this_layer_grad_biases[j] += this_layer_delta;

            for (std::size_t k = 0; k < past_layer_topology.values_count; k++) {
                grad_weights[k] += this_layer_delta * past_layer_values[k];
            }
        }
    }
}

void fa::Perceptron::step(neuron_value_t learning_rate) {
    //FA_SCOPE_TIMER("step")

    constexpr static fa::neuron_value_t alpha = 0.8;

    for (size_t i = 0; i < m_Weights.size(); i++) {
        m_VelocityWeights[i] = alpha * m_VelocityWeights[i] + (1.0f - alpha) * m_GradWeights[i];
        m_Weights[i] -= learning_rate * m_VelocityWeights[i];
    }

    for (size_t i = 0; i < m_Biases.size(); i++) {
        m_VelocityBiases[i] = alpha * m_VelocityBiases[i] + (1.0f - alpha) * m_GradBiases[i];
        m_Biases[i] -= learning_rate * m_VelocityBiases[i];
    }

    std::ranges::fill(m_GradWeights, 0);
    std::ranges::fill(m_GradBiases, 0);
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
    m_VelocityBiases.resize(m_VelocityBiases.size() + neurons_count, 0.0f);

    if (weights_to_add > 0) {
        this->randomly_initialize_weight(m_Weights, weights_to_add);
        m_GradWeights.resize(m_GradWeights.size() + weights_to_add, 0.0f);
        m_VelocityWeights.resize(m_VelocityWeights.size() + weights_to_add, 0.0f);
    }
}

void fa::Perceptron::randomly_initialize_weight(container_t<neuron_value_t>& dst, std::size_t elements_count) {
    std::size_t past_layer_count = m_LayersTopology.back().values_count;

    static std::mt19937                      gen{ std::random_device{}() };
    std::normal_distribution<neuron_value_t> dist{ 0.0, static_cast<neuron_value_t>(std::sqrt(2.0 / static_cast<double>(past_layer_count))) };

    dst.reserve(dst.size() + elements_count);

    for (size_t i = 0; i < elements_count; i++) {
        dst.push_back(dist(gen));
    }
}

std::span<fa::neuron_value_t> fa::Perceptron::GetLayerValues(const Layer& topology) {
    return { &m_Values[topology.values_start], topology.values_count };
}

std::span<fa::neuron_value_t> fa::Perceptron::GetLayerBiases(const Layer& topology) {
    return { &m_Biases[topology.values_start], topology.values_count };
}

std::span<fa::neuron_value_t> fa::Perceptron::GetLayerGradBiases(const Layer& topology) {
    return { &m_GradBiases[topology.values_start], topology.values_count };
}

std::span<fa::neuron_value_t> fa::Perceptron::GetLayerDeltas(const Layer& topology) {
    return { &m_Deltas[topology.values_start], topology.values_count };
}

std::span<fa::neuron_value_t> fa::Perceptron::GetLayerWeights(const Layer& topology) {
    return { &m_Weights[topology.weights_start], topology.weights_count };
}

std::span<fa::neuron_value_t> fa::Perceptron::GetLayerGradWeights(const Layer& topology) {
    return { &m_GradWeights[topology.weights_start], topology.weights_count };
}

std::span<fa::neuron_value_t> fa::Perceptron::GetNeuronWeights(const Layer& this_layer_topology, const Layer& past_layer_topology, std::size_t neuron_index) {
    std::size_t offset = this_layer_topology.weights_start + (neuron_index * past_layer_topology.values_count);
    return { &m_Weights[offset], past_layer_topology.values_count };
}

std::span<fa::neuron_value_t> fa::Perceptron::GetNeuronGradWeights(const Layer& this_layer_topology, const Layer& past_layer_topology, std::size_t neuron_index) {
    std::size_t offset = this_layer_topology.weights_start + (neuron_index * past_layer_topology.values_count);
    return { &m_GradWeights[offset], past_layer_topology.values_count };
}

// clang-format off
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerValues() { return m_Values; }
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerBiases() { return m_Biases; }
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerGradBiases() { return m_GradBiases; }
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerVelocityBiases() { return m_VelocityBiases; }
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerDeltas() { return m_Deltas; }
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerWeights() { return m_Weights; }
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerGradWeights() { return m_GradWeights; }
fa::container_t<fa::neuron_value_t> fa::Perceptron::GetLayerVelocityWeights() { return m_VelocityWeights; }
std::vector<fa::Perceptron::Layer> fa::Perceptron::GetLayersTopology() { return m_LayersTopology; }
// clang-format on
