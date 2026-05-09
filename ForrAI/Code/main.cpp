#include "GUI.hpp"
#include "BusinessLogic.hpp"

//template <typename T>
//void check_accuracy(fa::Perceptron& perceptron, T& dataset) {
//    std::size_t correct_count{};
//    double      accuracy{};
//
//    for (std::size_t i = 0; i < dataset.test_images.size(); i++) {
//        const auto& image = dataset.test_images[i];
//        const auto& label = dataset.test_labels[i];
//
//        const auto  predictions = perceptron.forward(image);
//        std::size_t answer      = fa::classify(predictions);
//
//        correct_count += answer == label ? 1 : 0;
//    }
//
//    accuracy = static_cast<double>(correct_count) / dataset.test_images.size();
//    std::cerr << "Accuracy : " << accuracy << std::endl;
//}
//
//template <typename T>
//void show_accuracy_picture_by_picture(fa::Perceptron& perceptron, T& dataset) {
//    for (std::size_t i = 0; i < dataset.test_images.size(); i++) {
//        const auto& image = dataset.test_images[i];
//        const auto& label = dataset.test_labels[i];
//
//        const auto  predictions = perceptron.forward(image);
//        std::size_t answer      = fa::classify(predictions);
//
//        fa::print_image(image);
//
//        std::cerr << "Predicted : " << answer << "\nTarget : " << static_cast<std::size_t>(label) << std::endl;
//
//        (void) std::getchar();
//    }
//}

int main() {
    fa::ApplicationContext context{};

    fa::GUI                gui{ context };
    fa::BusinessLogic      business_logic{ context };

    while (gui.IsWindowOpen()) {
        business_logic.Update();
        gui.Update();
    }

    return 0;
}
