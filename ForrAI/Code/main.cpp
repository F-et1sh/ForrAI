#include "mnist/mnist_reader.hpp"

#include "Perceptron.hpp"
#include "Serializer.hpp"

int main() {
    {
        FA_SCOPE_TIMER("ForrAI-training-time")

        fa::Perceptron perceptron(28 * 28, { 512, 256, 128 }); // total : 5 layers and 1 690 neurons

        // Load MNIST data
        auto dataset = mnist::read_dataset<std::vector, std::vector, std::uint8_t, std::uint8_t>("Files/mnist_archive");

        for (size_t epoch = 0; epoch < 10; epoch++) {
            FA_SCOPE_TIMER("Epoch " + std::to_string(epoch))

            auto zipped = std::views::zip(dataset.training_images, dataset.training_labels);

            static std::mt19937 gen{ std::random_device{}() };
            std::ranges::shuffle(zipped, gen);

            fa::neuron_value_t current_learning_rate = static_cast<fa::neuron_value_t>(0.069f * std::exp(-0.1f * epoch));

            for (std::size_t i = 0; i < dataset.training_images.size(); i++) {
                const auto& image = dataset.training_images[i];
                const auto& label = dataset.training_labels[i];

                const auto                         predictions = perceptron.forward(image);
                std::array<fa::neuron_value_t, 10> error_singal{};

                for (std::size_t j = 0; j < predictions.size(); j++) {
                    float target    = (j == label) ? 1.0f : 0.0f;
                    error_singal[j] = predictions[j] - target;
                }

                perceptron.backward(error_singal);

                if (i > 0 && i % 32 == 0) {
                    perceptron.step(current_learning_rate / 32.0);
                }
            }

            if (epoch > 0 && epoch % 10 == 0) {
                fa::Serializer        serializer{ perceptron };
                std::filesystem::path path = "ForrAI-epoch_" + std::to_string(epoch) + ".bin";
                serializer.Write(path);
            }
        }

        std::size_t correct_count{};
        double      accuracy{};

        for (std::size_t i = 0; i < dataset.test_images.size(); i++) {
            const auto& image = dataset.test_images[i];
            const auto& label = dataset.test_labels[i];

            const auto  predictions = perceptron.forward(image);
            std::size_t answer      = fa::classify(predictions);

            correct_count += answer == label ? 1 : 0;
        }

        accuracy = static_cast<double>(correct_count) / dataset.test_images.size();
        std::cerr << "Accuracy : " << accuracy << std::endl;

        fa::Serializer        serializer{ perceptron };
        std::filesystem::path path = "ForrAI-final.bin";
        serializer.Write(path);
    }

    std::system("pause");

    return 0;
}
