#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "misc.hpp"

namespace fa {
    class GUI {
    public:
        GUI(ApplicationContext& context);
        ~GUI();

        void Update();

    public:
        bool IsWindowOpen() const noexcept { return m_Window.isOpen(); }

    private:
        ApplicationContext& m_Context;

        sf::Clock        m_DeltaClock{};
        sf::RenderWindow m_Window{};
    };
} // namespace fa
