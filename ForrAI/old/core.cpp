#include "core.hpp"

fa::Dataset fa::load_dataset(const std::filesystem::path& relative_path) {
    Dataset dataset{};

    std::ifstream file(relative_path);
    if (!file) {
        std::cerr << "ERROR : Could not read dataset" << std::endl;
        return dataset;
    }

    std::string temp{};
    char        c{};
    bool        is_label = true;

    while (file.get(c)) {
        temp += c;

        if (c != ',' && c != ';') continue;

        int val = std::stoi(temp);
        temp.clear();

        if (is_label) {
            dataset.push_back(Sample());
            dataset.back().target = static_cast<float_t>(val);
            is_label              = false;
        }
        else {
            dataset.back().image.push_back(val / 255.0f);
        }

        if (c == ';') is_label = true;
    }

    return dataset;
}

void fa::print_image(const std::vector<float_t>& image) {
    for (size_t i = 0; i < image.size(); i++) {
        std::cerr << (image[i] > 0.5 ? "X" : " ");
        if (i > 0 && i % 28 == 0) {
            std::cerr << std::endl;
        }
    }
    std::cerr << std::endl;
}

std::vector<float_t> fa::random_weight(size_t elements_count) {
    std::vector<float_t> weight{};
    weight.reserve(elements_count);
    for (size_t i = 0; i < elements_count; i++) {
        weight.push_back(static_cast<float_t>((0.01 * std::rand()) / RAND_MAX));
    }
    return weight;
}

float_t fa::classify(float_t prediction) {
    return prediction > 0.5 ? 1.0f : 0.0f;
}
