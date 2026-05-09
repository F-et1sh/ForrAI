#pragma once
#include <iostream>
#include <array>
#include <optional>

namespace fa {
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using container_t    = std::vector<_Ty, _Alloc>;
    using neuron_value_t = float;

    struct ApplicationContext {
    public:
        constexpr static uint8_t picture_width  = 28;
        constexpr static uint8_t picture_height = 28;

        std::array<float, picture_width * picture_height> canvas_pixels{};
        std::array<float, 10>                             current_predictions{};

        bool is_canvas_dirty{};

        ApplicationContext()  = default;
        ~ApplicationContext() = default;
    };

    static std::size_t classify(const std::array<neuron_value_t, 10>& predictions) {
        std::size_t    result = 0;
        neuron_value_t max    = predictions[0];

        for (std::size_t i = 1; i < predictions.size(); i++) {
            if (predictions[i] > max) {
                max    = predictions[i];
                result = i;
            }
        }
        return result;
    }

    template <template <typename...> class Container = std::vector, typename Value = uint8_t>
    static void print_image(const Container<Value>& image) {
        for (size_t i = 0; i < image.size(); i++) {
            std::cerr << (image[i] > 0.5 ? "X" : " ");
            if (i > 0 && i % 28 == 0) {
                std::cerr << std::endl;
            }
        }
        std::cerr << std::endl;
    }

#define FA_CONCATENATE_DIRECT(x, y) x##y
#define FA_CONCATENATE(x, y) FA_CONCATENATE_DIRECT(x, y)
#define FA_VARIABLE_ON_LINE(str) FA_CONCATENATE(str, __LINE__)
} // namespace fa
