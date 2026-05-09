#pragma once
#include "Perceptron.hpp"
#include "misc.hpp"

namespace fa {
    class BusinessLogic {
    public:
        BusinessLogic(ApplicationContext& context) : m_Context(context) {}
        ~BusinessLogic() = default;

        void Update();

    private:
        ApplicationContext& m_Context;

        fa::Perceptron m_Perceptron{
            ApplicationContext::picture_width * ApplicationContext::picture_height, // 784
            { 512, 256, 128 }                                                       // 784 --> 512 --> 256 --> 128 --> 10
        };
    };
} // namespace fa
