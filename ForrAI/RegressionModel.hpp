#pragma once

#include "core.hpp"

namespace fa {
    class RegressionModel {
    public:
        // create a model with given number of units in input m_Layers, number of units in hidden layers, and number of layers
        RegressionModel(size_t input_count, size_t hidden_count, size_t layer_count);
        ~RegressionModel() = default;

        // compute forward pass
        float_t forward(const std::vector<float_t>& input_data);

        // compute backward pass
        void backward(float_t error_signal);

        // do a small step in direction of the negative gradient, as this reduces the loss
        void step(float_t lr);

    private:
        std::vector<Layer> m_Layers;
    };
} // namespace fa
