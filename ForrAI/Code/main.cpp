#include "mnist/mnist_reader.hpp"

#include "Perceptron.hpp"

int main() {
    fa::Perceptron perceptron(28 * 28, 10, 3);

    // Load MNIST data
    auto dataset = mnist::read_dataset<std::vector, std::vector, std::uint8_t, std::uint8_t>("Files/mnist_archive");

    for (size_t epoch = 0; epoch < 10; epoch++) {

        FA_SCOPE_TIMER("Epoch " + std::to_string(epoch))

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

            if (i % 32 == 0)
                perceptron.step(static_cast<fa::neuron_value_t>(epoch <= 5 ? 0.001 : 0.0005));
        }
    }

    std::size_t correct_count{};

    for (std::size_t i = 0; i < dataset.test_images.size(); i++) {
        const auto& image = dataset.test_images[i];
        const auto& label = dataset.test_labels[i];

        const auto  predictions = perceptron.forward(image);
        std::size_t answer      = fa::classify(predictions);

        correct_count += answer == label ? 1 : 0;

        /*fa::print_image(image);

        for (std::size_t j = 0; j < predictions.size(); j++) {
            std::cerr << j << " : " << predictions[j] << std::endl;
        }

        std::cerr << "Predicted : " << fa::classify(predictions) << ". Target : " << static_cast<std::size_t>(label) << std::endl;
        (void) std::getchar();*/
    }

    double accuracy = static_cast<double>(correct_count) / dataset.test_images.size();
    std::cerr << "Accuracy : " << accuracy << std::endl;

    return 0;
}
