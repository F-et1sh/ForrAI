#pragma once
#include <assert.h>
#include <span>
#include <vector>
#include <random>

namespace fa {
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using container_t    = std::vector<_Ty, _Alloc>;
    using neuron_value_t = float;

    class Perceptron {
    public:
        Perceptron(std::size_t input_layer_neurons_count,
                   std::size_t hidden_layer_neurons_count,
                   std::size_t layers_count);
        ~Perceptron() = default;

        template <template <typename...> class Container = std::vector, typename Value = float>
        std::array<neuron_value_t, 10> forward(const Container<Value>& input_data) {
            
        }

    private:
        void create_layer(std::size_t neurons_count);
        void randomly_initialize_weight(container_t<neuron_value_t>& dst, std::size_t elements_count);

    private:
        // values
        container_t<neuron_value_t> m_Values;
        container_t<neuron_value_t> m_Biases;
        container_t<neuron_value_t> m_GradBiases;
        container_t<neuron_value_t> m_Deltas;

        // weights
        container_t<neuron_value_t> m_Weights;
        container_t<neuron_value_t> m_GradWeights;

        struct Layer {
            bool is_active = true;

            std::size_t values_start{};
            std::size_t weights_start{};

            std::size_t values_count{};
            std::size_t weights_count{};
        };

        std::vector<Layer> m_Topology;
    };
} // namespace fa
