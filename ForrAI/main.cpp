#include "RegressionModel.hpp"

int main() {
    // build the model
    fa::RegressionModel model{ 28 * 28, 10, 3 };

    // training loop
    const fa::Dataset dataset_train = fa::load_dataset("mnist_train.txt");
    std::cerr << "Training model..." << std::endl;
    std::cerr << "Dataset size : " << dataset_train.size() << std::endl;

    for (size_t epoch = 0; epoch < 10; epoch++) {
        for (size_t i = 0; i < dataset_train.size(); i++) {
            const fa::Sample& sample       = dataset_train[i];
            fa::float_t       prediction   = model.forward(sample.image);
            fa::float_t       loss         = static_cast<fa::float_t>(0.5 * std::pow(sample.target - prediction, 2));
            fa::float_t       error_signal = prediction - sample.target;
            model.backward(error_signal);
            model.step(static_cast<fa::float_t>(0.001));

            // don't print too often, this is slow
            if (i % 1000 == 0) {
                std::cerr << "Epoch : " << epoch << " Sample : " << i << " Loss : " << loss << std::endl;
            }
        }
    }

    // check model accuracy on testset
    const fa::Dataset dataset_test = fa::load_dataset("mnist_test.txt");
    std::cerr << "Testing model..." << std::endl;
    std::cerr << "Dataset size : " << dataset_test.size() << std::endl;
    size_t correct_ctr = 0;
    for (const fa::Sample& sample : dataset_test) {
        fa::float_t prediction = model.forward(sample.image);
        correct_ctr += fa::classify(prediction) == sample.target ? 1 : 0;
    }
    fa::float_t accuracy = static_cast<fa::float_t>(correct_ctr) / dataset_test.size();
    std::cerr << "Accuracy : " << accuracy << std::endl;

    // show testset samples and predictions
    for (const fa::Sample& sample : dataset_test) {
        fa::float_t prediction = model.forward(sample.image);
        fa::print_image(sample.image);
        std::cerr << "Predicted: " << fa::classify(prediction) << " (" << prediction << ") Target: " << sample.target << std::endl;
        std::cerr << "Press ENTER to see next sample..." << std::endl;
        (void) std::getchar();
    }

    return 0;
}
