#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
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

        bool IsWindowOpen() const noexcept { return m_Window.isOpen(); }

    private:
        void DrawMainMenuBar();
        void DrawCanvas();

        void updateCanvasTexture();
        void applyBrush(int center_x, int center_y, float target_value);
        
        bool isPixelInsideCanvas(int pixel_x, int pixel_y) const;

    private:
        ApplicationContext& m_Context;

        ImVec2 m_LastBrushCanvasPosition{};

        sf::Clock m_DeltaClock{};

        std::vector<std::uint8_t> m_TexturePixels{};
        sf::Texture               m_CanvasTexture{};
        sf::Sprite                m_CanvasSprite{ m_CanvasTexture };

        sf::RenderWindow m_Window{};
    };
} // namespace fa
