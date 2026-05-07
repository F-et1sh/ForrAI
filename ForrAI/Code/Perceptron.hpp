#pragma once
#include <iostream>
#include <assert.h>
#include <ranges>
#include <span>
#include <array>
#include <vector>
#include <random>

namespace fa {
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using container_t    = std::vector<_Ty, _Alloc>;
    using neuron_value_t = float;

    static std::size_t classify(const std::array<neuron_value_t, 10>& predictions) {
        std::size_t    result = 0;
        neuron_value_t max    = predictions[0];

        for (std::size_t i = 1; i < predictions.size(); i++) {
            if (predictions[i] > max) {
                max    = predictions[i];
                result = i;
            }
        }
        return result;
    }

    template <template <typename...> class Container = std::vector, typename Value = uint8_t>
    static void print_image(const Container<Value>& image) {
        for (size_t i = 0; i < image.size(); i++) {
            std::cerr << (image[i] > 0.5 ? "X" : " ");
            if (i > 0 && i % 28 == 0) {
                std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;
    }

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

        template <template <typename...> class Container = std::vector, typename Value = uint8_t>
        std::array<neuron_value_t, 10> forward(const Container<Value>& input_data) {

            for (std::size_t i = 0; i < input_data.size(); i++)
                m_Values[i] = static_cast<float>(input_data[i]) / 255.0f;

            for (std::size_t i = 1; i < m_LayersTopology.size(); i++) {
                const auto& this_layer_topology = m_LayersTopology[i];
                const auto& past_layer_topology = m_LayersTopology[i - 1];

                auto this_layer_values = this->GetLayerValues(this_layer_topology);
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
            const Layer&                   last_layer = m_LayersTopology.back();
            for (std::size_t i = 0; i < result.size(); i++) {
                result[i] = m_Values[last_layer.values_start + i];
            }

            return result;
        }

        void backward(const std::array<neuron_value_t, 10>& error_signal);

        void step(neuron_value_t learning_rate);

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
