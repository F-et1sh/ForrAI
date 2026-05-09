#include "mnist/mnist_reader.hpp"

#include "Perceptron.hpp"
#include "Serializer.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

template <typename T>
void check_accuracy(fa::Perceptron& perceptron, T& dataset) {
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
}

template <typename T>
void show_accuracy_picture_by_picture(fa::Perceptron& perceptron, T& dataset) {
    for (std::size_t i = 0; i < dataset.test_images.size(); i++) {
        const auto& image = dataset.test_images[i];
        const auto& label = dataset.test_labels[i];

        const auto  predictions = perceptron.forward(image);
        std::size_t answer      = fa::classify(predictions);

        fa::print_image(image);

        std::cerr << "Predicted : " << answer << "\nTarget : " << static_cast<std::size_t>(label) << std::endl;

        (void) std::getchar();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML = <3");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    sf::Clock delta_clock{};

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, delta_clock.restart());

        // ...

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
