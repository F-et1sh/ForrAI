#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fa {
    using float_t = float;

    // an MNIST sample holding the image ( 28*28 float values ) and a label ( 0 or 1 )
    struct Sample {
        std::vector<float> image{};
        float              target{};

        Sample()  = default;
        ~Sample() = default;
    };

    using Dataset = std::vector<Sample>;

    // load the MNIST dataset from a text file
    Dataset load_dataset(const std::filesystem::path& relative_path);

    // show an MNIST image on the console
    void print_image(const std::vector<float_t>& image);

    // create a weight vector containing random values
    std::vector<float_t> random_weight(size_t elements_count);

    // compute class label from raw prediction
    float_t classify(float_t prediction);

    // unit is the main buildig block of the model, aka "neuron"
    struct Unit {
        bool has_activation{};

        float_t bias{};
        float_t value{};
        float_t grad_bias{};
        float_t delta{};

        std::vector<float_t> weight{};
        std::vector<float_t> grad_weight{};

        Unit(size_t input_count, bool has_activation) : weight(random_weight(input_count)), has_activation(has_activation) {}
        explicit Unit(float_t value) : value(value) {}

        ~Unit() = default;
    };

    using Layer = std::vector<Unit>;

} // namespace fa
