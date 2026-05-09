#include "GUI.hpp"

#include <algorithm>
#include <cmath>

fa::GUI::GUI(ApplicationContext& context) : m_Context(context) {
    m_Window.create(sf::VideoMode({ 640, 480 }), "ForrAI");
    m_Window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(m_Window);

    m_CanvasTexture.resize(sf::Vector2u(fa::ApplicationContext::canvas_width, fa::ApplicationContext::canvas_height));
    m_CanvasTexture.setSmooth(false);

    m_TexturePixels.resize(fa::ApplicationContext::canvas_width * fa::ApplicationContext::canvas_height * 4);
    std::fill(m_TexturePixels.begin(), m_TexturePixels.end(), 0);

    this->updateCanvasTexture();

    m_LastBrushCanvasPosition = ImVec2(-1.0f, -1.0f);
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

    this->DrawMainMenuBar();
    this->DrawCanvas();

    m_Window.clear();
    ImGui::SFML::Render(m_Window);
    m_Window.display();
}

void fa::GUI::DrawMainMenuBar() {
    if (!ImGui::BeginMainMenuBar()) return;

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save")) {
        }

        if (ImGui::MenuItem("Load")) {
        }

        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void fa::GUI::DrawCanvas() {
    constexpr static float            canvas_scale        = 15.0f;
    constexpr static ImGuiWindowFlags static_window_flags = ImGuiWindowFlags_NoMove |
                                                            ImGuiWindowFlags_NoResize |
                                                            ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f));
    ImGui::SetNextWindowSize(ImVec2(450.0f, 460.0f));

    ImGui::Begin("Canvas", nullptr, static_window_flags);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 canvas_screen_position = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size            = ImVec2(fa::ApplicationContext::canvas_width * canvas_scale,
                                           fa::ApplicationContext::canvas_height * canvas_scale);

    // draw the canvas
    ImGui::Image(m_CanvasTexture, sf::Vector2f(canvas_size.x, canvas_size.y));

    // draw the grid
    constexpr ImU32 grid_color = IM_COL32(30, 30, 30, 255);

    for (int x = 0; x <= fa::ApplicationContext::canvas_width; x++) {
        float line_x = canvas_screen_position.x + static_cast<float>(x) * canvas_scale;

        draw_list->AddLine(ImVec2(line_x, canvas_screen_position.y),
                           ImVec2(line_x, canvas_screen_position.y + canvas_size.y),
                           grid_color);
    }

    for (int y = 0; y <= fa::ApplicationContext::canvas_height; y++) {
        float line_y = canvas_screen_position.y + static_cast<float>(y) * canvas_scale;

        draw_list->AddLine(ImVec2(canvas_screen_position.x, line_y),
                           ImVec2(canvas_screen_position.x + canvas_size.x, line_y),
                           grid_color);
    }

    // mouse
    ImVec2 mouse_position = ImGui::GetMousePos();

    bool is_mouse_inside_canvas = mouse_position.x >= canvas_screen_position.x &&
                                  mouse_position.x < canvas_screen_position.x + canvas_size.x &&
                                  mouse_position.y >= canvas_screen_position.y &&
                                  mouse_position.y < canvas_screen_position.y + canvas_size.y;

    // brush resize
    if (is_mouse_inside_canvas) {
        float mouse_wheel = ImGui::GetIO().MouseWheel;

        m_Context.brush_size -= static_cast<int>(mouse_wheel);
        m_Context.brush_size = std::clamp(m_Context.brush_size,
                                          fa::ApplicationContext::brush_min_size,
                                          fa::ApplicationContext::brush_max_size);
    }

    // brush preview
    if (is_mouse_inside_canvas) {
        float visual_radius = static_cast<float>(m_Context.brush_size) * canvas_scale;
        draw_list->AddCircle(mouse_position, visual_radius, IM_COL32(255, 255, 0, 180), 16, 2.0f);
    }

    // drawing
    bool is_drawing = ImGui::IsMouseDown(ImGuiMouseButton_Left) ||
                      ImGui::IsMouseDown(ImGuiMouseButton_Right);

    if (is_mouse_inside_canvas && is_drawing) {
        int canvas_x = static_cast<int>((mouse_position.x - canvas_screen_position.x) / canvas_scale);
        int canvas_y = static_cast<int>((mouse_position.y - canvas_screen_position.y) / canvas_scale);

        float target_value = ImGui::IsMouseDown(ImGuiMouseButton_Left) ? 1.0f : 0.0f;

        if (m_LastBrushCanvasPosition.x < 0.0f || m_LastBrushCanvasPosition.y < 0.0f)
            this->applyBrush(canvas_x, canvas_y, target_value);
        else {
            float delta_x = static_cast<float>(canvas_x) - m_LastBrushCanvasPosition.x;
            float delta_y = static_cast<float>(canvas_y) - m_LastBrushCanvasPosition.y;

            float distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);

            int interpolation_steps = std::max(1, static_cast<int>(distance));

            for (int step = 0; step <= interpolation_steps; step++) {

                float interpolation_factor = static_cast<float>(step) / static_cast<float>(interpolation_steps);

                int interpolated_x = static_cast<int>(std::lerp(m_LastBrushCanvasPosition.x, static_cast<float>(canvas_x), interpolation_factor));
                int interpolated_y = static_cast<int>(std::lerp(m_LastBrushCanvasPosition.y, static_cast<float>(canvas_y), interpolation_factor));

                this->applyBrush(interpolated_x, interpolated_y, target_value);
            }
        }

        m_LastBrushCanvasPosition = ImVec2(static_cast<float>(canvas_x), static_cast<float>(canvas_y));
    }
    else
        m_LastBrushCanvasPosition = ImVec2(-1.0f, -1.0f);

    // texture upload
    if (m_Context.is_canvas_dirty) this->updateCanvasTexture();

    ImGui::End();
}

void fa::GUI::updateCanvasTexture() {
    if (!m_Context.is_canvas_dirty) return;

    constexpr static std::uint8_t alpha = 255;

    for (std::size_t index = 0; index < m_Context.canvas_pixels.size(); index++) {
        float value = m_Context.canvas_pixels[index];

        std::uint8_t color = static_cast<std::uint8_t>(value * 255.0f);

        std::size_t texture_index = index * 4;

        m_TexturePixels[texture_index + 0] = color;
        m_TexturePixels[texture_index + 1] = color;
        m_TexturePixels[texture_index + 2] = color;
        m_TexturePixels[texture_index + 3] = alpha;
    }

    m_CanvasTexture.update(m_TexturePixels.data());

    m_Context.is_canvas_dirty = false;
}

void fa::GUI::applyBrush(int center_x, int center_y, float target_value) {
    int   brush_radius         = m_Context.brush_size;
    float brush_radius_squared = static_cast<float>(brush_radius * brush_radius);

    for (int offset_y = -brush_radius; offset_y <= brush_radius; offset_y++) {
        for (int offset_x = -brush_radius; offset_x <= brush_radius; offset_x++) {

            int pixel_x = center_x + offset_x;
            int pixel_y = center_y + offset_y;

            if (!this->isPixelInsideCanvas(pixel_x, pixel_y)) continue;

            float distance_squared = static_cast<float>(offset_x * offset_x + offset_y * offset_y);
            if (distance_squared > brush_radius_squared) continue;

            float normalized_distance_squared = distance_squared / brush_radius_squared;

            float opacity = 1.0f - normalized_distance_squared;
            opacity *= opacity;

            std::size_t pixel_index = pixel_y * fa::ApplicationContext::canvas_width + pixel_x;

            auto& pixel = m_Context.canvas_pixels[pixel_index];
            pixel       = std::lerp(pixel, target_value, opacity);
        }
    }

    m_Context.is_canvas_dirty = true;
}

bool fa::GUI::isPixelInsideCanvas(int x, int y) const {
    return x >= 0 && x < fa::ApplicationContext::canvas_width &&
           y >= 0 && y < fa::ApplicationContext::canvas_height;
}
