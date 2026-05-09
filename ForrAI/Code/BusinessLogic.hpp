#pragma once
#include "Perceptron.hpp"
#include "Serializer.hpp"

namespace fa {
    class BusinessLogic {
    public:
        BusinessLogic(ApplicationContext& context);
        ~BusinessLogic() = default;

        void Update();

    private:
        ApplicationContext& m_Context;

        fa::Perceptron m_Perceptron{
            ApplicationContext::canvas_width * ApplicationContext::canvas_height, // 784
            { 512, 256, 128 }                                                     // 784 --> 512 --> 256 --> 128 --> 10
        };
    };
} // namespace fa
