#pragma once
#include <iostream>
#include <array>
#include <optional>
#include <filesystem>

namespace fa {
    template <class _Ty, class _Alloc = std::allocator<_Ty>>
    using container_t    = std::vector<_Ty, _Alloc>;
    using neuron_value_t = float;

    struct ApplicationContext {
    public:
        constexpr static int brush_min_size = 2;
        constexpr static int brush_max_size = 10;
        int                  brush_size     = 2;

        std::filesystem::path font_path = "Files/Fonts/OpenSans/open_sans.ttf";

        std::array<float, 10> current_predictions{};

        std::string           error_message{}; // drawing this if not empty
        std::filesystem::path path_to_file{};  // using this to write or read if flags ( is_reading/is_writing ) are active

        constexpr static std::uint8_t                            canvas_width  = 28;
        constexpr static std::uint8_t                            canvas_height = 28;
        std::array<neuron_value_t, canvas_width * canvas_height> canvas_pixels{};

        bool is_canvas_dirty = false;
        bool is_reading      = false;
        bool is_writing      = false;

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

    template <template <typename...> class Container = std::vector, typename Value = std::uint8_t>
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
