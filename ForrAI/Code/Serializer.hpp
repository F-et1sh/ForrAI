#pragma once
#include <fstream>
#include <filesystem>

#include "Perceptron.hpp"
#include "misc.hpp"

namespace fa {
    class Serializer {
    public:
        Serializer(Perceptron& perceptron) : m_Perceptron(perceptron) {}
        ~Serializer() = default;

        void Write(const std::filesystem::path& path, ApplicationContext& context) {
            std::ofstream file(path, std::ios::binary);
            if (!file.good()) {
                reportError(context, std::string("Failed to serialize perceptron. Failed to open the file\nPath : ") + path.string());
                return;
            }

            write_variable(file, m_Perceptron.GetLayerValues());
            write_variable(file, m_Perceptron.GetLayerBiases());
            write_variable(file, m_Perceptron.GetLayerGradBiases());
            write_variable(file, m_Perceptron.GetLayerVelocityBiases());
            write_variable(file, m_Perceptron.GetLayerDeltas());
            write_variable(file, m_Perceptron.GetLayerWeights());
            write_variable(file, m_Perceptron.GetLayerGradWeights());
            write_variable(file, m_Perceptron.GetLayerVelocityWeights());
            write_variable(file, m_Perceptron.GetLayersTopology());
        }

        void Read(const std::filesystem::path& path, ApplicationContext& context) {
            std::ifstream file(path, std::ios::binary);
            if (!file.good()) {
                reportError(context, std::string("Failed to deserialize perceptron. Failed to open the file\nPath : ") + path.string());
                return;
            }
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            fa::container_t<fa::neuron_value_t> layer_values{};
            fa::container_t<fa::neuron_value_t> layer_biases{};
            fa::container_t<fa::neuron_value_t> layer_grad_biases{};
            fa::container_t<fa::neuron_value_t> layer_velocity_biases{};
            fa::container_t<fa::neuron_value_t> layer_deltas{};
            fa::container_t<fa::neuron_value_t> layer_weights{};
            fa::container_t<fa::neuron_value_t> layer_grad_weights{};
            fa::container_t<fa::neuron_value_t> layer_velocity_weights{};

            std::vector<fa::Perceptron::Layer> layers_topology{};

            try {
                read_variable(file, layer_values);
                read_variable(file, layer_biases);
                read_variable(file, layer_grad_biases);
                read_variable(file, layer_velocity_biases);
                read_variable(file, layer_deltas);
                read_variable(file, layer_weights);
                read_variable(file, layer_grad_weights);
                read_variable(file, layer_velocity_weights);
                read_variable(file, layers_topology);
            }
            catch (const std::exception& e) {
                reportError(context, std::string("Failed to deserialize perceptron. ") + e.what());
            }

            m_Perceptron = fa::Perceptron{ layer_values,
                                           layer_biases,
                                           layer_grad_biases,
                                           layer_velocity_biases,
                                           layer_deltas,
                                           layer_weights,
                                           layer_grad_weights,
                                           layer_velocity_weights,
                                           layers_topology };
        }

    private:
        template <template <typename...> class Container, typename Element>
        static void write_variable(std::ofstream& file, const Container<Element>& vec) {
            std::size_t size = vec.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size));
            file.write(reinterpret_cast<const char*>(vec.data()), size * sizeof(Element));
        }

        template <template <typename...> class Container, typename Element>
        static void read_variable(std::ifstream& file, Container<Element>& vec) {
            std::size_t size{};
            if (!file.read(reinterpret_cast<char*>(&size), sizeof(size))) return;

            if (size > 100'000'000) throw std::runtime_error("Too much data\nMost likely wrong format");

            vec.resize(size);
            if (size > 0)
                file.read(reinterpret_cast<char*>(vec.data()), size * sizeof(Element));
        }

    private:
        Perceptron& m_Perceptron;
    };
} // namespace fa
