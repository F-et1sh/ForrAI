#pragma once
#include <assert.h>
#include <span>
#include <array>
#include <vector>
#include <random>

namespace fa {
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using container_t    = std::vector<_Ty, _Alloc>;
    using neuron_value_t = float;

    class Perceptron {
    private:
        struct Layer {
            bool is_active = true;

            std::size_t values_start{};
            std::size_t weights_start{};

            std::size_t values_count{};
            std::size_t weights_count{};
        };

    public:
        Perceptron(std::size_t input_layer_neurons_count,
                   std::size_t hidden_layer_neurons_count,
                   std::size_t layers_count);
        ~Perceptron() = default;

        template <template <typename...> class Container = std::vector, typename Value = float>
        std::array<neuron_value_t, 10> forward(const Container<Value>& input_data) {

            for (std::size_t i = 0; i < input_data.size(); i++)
                m_Values[i] = input_data[i];

            for (std::size_t i = 1; i < m_LayersTopology.size(); i++) {
                const auto& this_layer_topology = m_LayersTopology[i];
                const auto& past_layer_topology = m_LayersTopology[i - 1];

                auto this_layer_values = this->GetLayerBiases(this_layer_topology);
                auto this_layer_biases = this->GetLayerBiases(this_layer_topology);

                auto past_layer_values = this->GetLayerValues(past_layer_topology);

                for (std::size_t j = 0; j < this_layer_topology.values_count; j++) {
                    auto this_layer_neuron_weights = this->GetNeuronWeights(this_layer_topology, past_layer_topology, j);

                    neuron_value_t pre_activation = this_layer_biases[j];

                    for (std::size_t n = 0; n < past_layer_topology.values_count; n++) {
                        pre_activation += past_layer_values[n] * this_layer_neuron_weights[n];
                    }

                    this_layer_values[j] = this_layer_topology.is_active ? std::max<neuron_value_t>(0, pre_activation) : pre_activation;
                }
            }

            std::array<neuron_value_t, 10> result{};
            std::size_t                    start = m_Values.size() - 10;

            for (std::size_t i = 0; i < result.size(); i++)
                result[i] = m_Values[start + i];

            return result;
        }

        void backward(const std::array<neuron_value_t, 10>& error_signal) {
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



    private:
        void create_layer(std::size_t neurons_count);
        void randomly_initialize_weight(container_t<neuron_value_t>& dst, std::size_t elements_count);

    private:
        std::span<neuron_value_t> GetLayerValues(const Layer& topology) {
            return { &m_Values[topology.values_start], topology.values_count };
        }

        std::span<neuron_value_t> GetLayerBiases(const Layer& topology) {
            return { &m_Biases[topology.values_start], topology.values_count };
        }

        std::span<neuron_value_t> GetLayerGradBiases(const Layer& topology) {
            return { &m_GradBiases[topology.values_start], topology.values_count };
        }

        std::span<neuron_value_t> GetLayerDeltas(const Layer& topology) {
            return { &m_Deltas[topology.values_start], topology.values_count };
        }

        std::span<neuron_value_t> GetLayerWeights(const Layer& topology) {
            return { &m_Weights[topology.weights_start], topology.weights_count };
        }

        std::span<neuron_value_t> GetLayerGradWeights(const Layer& topology) {
            return { &m_GradWeights[topology.weights_start], topology.weights_count };
        }

        std::span<neuron_value_t> GetNeuronWeights(const Layer& this_layer_topology,
                                                   const Layer& past_layer_topology,
                                                   std::size_t  neuron_index) {
            std::size_t offset = this_layer_topology.weights_start + (neuron_index * past_layer_topology.values_count);
            return { &m_Weights[offset], past_layer_topology.values_count };
        }

        std::span<neuron_value_t> GetNeuronGradWeights(const Layer& this_layer_topology,
                                                       const Layer& past_layer_topology,
                                                       std::size_t  neuron_index) {
            std::size_t offset = this_layer_topology.weights_start + (neuron_index * past_layer_topology.values_count);
            return { &m_GradWeights[offset], past_layer_topology.values_count };
        }

    private:
        // values
        container_t<neuron_value_t> m_Values;
        container_t<neuron_value_t> m_Biases;
        container_t<neuron_value_t> m_GradBiases;
        container_t<neuron_value_t> m_Deltas;

        // weights
        container_t<neuron_value_t> m_Weights;
        container_t<neuron_value_t> m_GradWeights;

        std::vector<Layer> m_LayersTopology;
    };
} // namespace fa
