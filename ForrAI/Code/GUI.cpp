#include "GUI.hpp"

fa::GUI::GUI(ApplicationContext& context) : m_Context(context) {
    m_Window.create(sf::VideoMode({ 640, 480 }), "ForrAI");
    m_Window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(m_Window);
}

fa::GUI::~GUI() {
    ImGui::SFML::Shutdown();
}

void fa::GUI::Update() {
    while (const auto event = m_Window.pollEvent()) {
        ImGui::SFML::ProcessEvent(m_Window, *event);

        if (event->is<sf::Event::Closed>()) {
            m_Window.close();
        }
    }

    ImGui::SFML::Update(m_Window, m_DeltaClock.restart());

    // ...

    m_Window.clear();
    ImGui::SFML::Render(m_Window);
    m_Window.display();
}
