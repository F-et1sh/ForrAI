#pragma once
#include <iostream>
#include <assert.h>
#include <ranges>
#include <span>
#include <array>
#include <vector>
#include <random>

#include "Profiler.hpp"

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
    public:
        struct Layer {
            bool is_active = true;

            std::size_t values_start{};
            std::size_t weights_start{};

            std::size_t values_count{};
            std::size_t weights_count{};
        };

    public:
        Perceptron(std::size_t input_layer_neurons_count, std::vector<std::size_t> hidden_layers);
        Perceptron(const container_t<neuron_value_t>& values,
                   const container_t<neuron_value_t>& biases,
                   const container_t<neuron_value_t>& grad_biases,
                   const container_t<neuron_value_t>& deltas,
                   const container_t<neuron_value_t>& weights,
                   const container_t<neuron_value_t>& grad_weights,
                   const std::vector<Layer>&          layers_topology)
            : m_Values(values),
              m_Biases(biases),
              m_GradBiases(grad_biases),
              m_Deltas(deltas),
              m_Weights(weights),
              m_GradWeights(grad_weights),
              m_LayersTopology(layers_topology) {}
        ~Perceptron() = default;

        template <template <typename...> class Container = std::vector, typename Value = uint8_t>
        std::array<neuron_value_t, 10> forward(const Container<Value>& input_data) {
            //FA_SCOPE_TIMER("forward")

            constexpr float value_max = std::numeric_limits<Value>::max();

            for (std::size_t i = 0; i < input_data.size(); i++)
                m_Values[i] = static_cast<float>(input_data[i]) / value_max;

            for (std::size_t i = 1; i < m_LayersTopology.size(); i++) {
                const auto& this_layer_topology = m_LayersTopology[i];
                const auto& past_layer_topology = m_LayersTopology[i - 1];

                auto this_layer_values = this->GetLayerValues(this_layer_topology);
                auto this_layer_biases = this->GetLayerBiases(this_layer_topology);

                auto past_layer_values = this->GetLayerValues(past_layer_topology);

#pragma omp parallel for schedule(static)
                for (std::int64_t j = 0; j < this_layer_topology.values_count; j++) {
                    auto this_layer_neuron_weights = this->GetNeuronWeights(this_layer_topology, past_layer_topology, j);

                    neuron_value_t pre_activation = this_layer_biases[j];

                    for (std::size_t n = 0; n < past_layer_topology.values_count; n++) {
                        pre_activation += past_layer_values[n] * this_layer_neuron_weights[n];
                    }

                    this_layer_values[j] = this_layer_topology.is_active ? std::max<neuron_value_t>(0, pre_activation) : pre_activation;
                }
            }

            const auto& last_layer        = m_LayersTopology.back();
            auto        last_layer_values = this->GetLayerValues(last_layer);

            std::array<neuron_value_t, 10> result{};

            for (std::size_t i = 0; i < last_layer_values.size(); i++)
                result[i] = last_layer_values[i];

            neuron_value_t max_value = *std::ranges::max_element(result);

            neuron_value_t sum_exp = 0;
            for (auto& value : result) {
                value = std::exp(value - max_value);
                sum_exp += value;
            }

            for (auto& value : result)
                value /= sum_exp;

            return result;
        }
        void backward(const std::array<neuron_value_t, 10>& error_signal);
        void step(neuron_value_t learning_rate);

    private:
        void create_layer(std::size_t neurons_count);
        void randomly_initialize_weight(container_t<neuron_value_t>& dst, std::size_t elements_count);

    private:
        std::span<neuron_value_t> GetLayerValues(const Layer& topology);
        std::span<neuron_value_t> GetLayerBiases(const Layer& topology);
        std::span<neuron_value_t> GetLayerGradBiases(const Layer& topology);
        std::span<neuron_value_t> GetLayerDeltas(const Layer& topology);
        std::span<neuron_value_t> GetLayerWeights(const Layer& topology);
        std::span<neuron_value_t> GetLayerGradWeights(const Layer& topology);
        std::span<neuron_value_t> GetNeuronWeights(const Layer& this_layer_topology, const Layer& past_layer_topology, std::size_t neuron_index);
        std::span<neuron_value_t> GetNeuronGradWeights(const Layer& this_layer_topology, const Layer& past_layer_topology, std::size_t neuron_index);

    public:
        container_t<neuron_value_t> GetLayerValues();
        container_t<neuron_value_t> GetLayerBiases();
        container_t<neuron_value_t> GetLayerGradBiases();
        container_t<neuron_value_t> GetLayerVelocityBiases();
        container_t<neuron_value_t> GetLayerDeltas();
        container_t<neuron_value_t> GetLayerWeights();
        container_t<neuron_value_t> GetLayerGradWeights();
        container_t<neuron_value_t> GetLayerVelocityWeights();

        std::vector<Layer> GetLayersTopology();

    private:
        // values
        container_t<neuron_value_t> m_Values{};
        container_t<neuron_value_t> m_Deltas{};
        container_t<neuron_value_t> m_Biases{};
        container_t<neuron_value_t> m_GradBiases{};
        container_t<neuron_value_t> m_VelocityBiases{};

        // weights
        container_t<neuron_value_t> m_Weights{};
        container_t<neuron_value_t> m_GradWeights{};
        container_t<neuron_value_t> m_VelocityWeights{};

        std::vector<Layer> m_LayersTopology{};
    };
} // namespace fa
