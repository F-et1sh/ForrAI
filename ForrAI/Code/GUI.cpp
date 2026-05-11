#include "GUI.hpp"

#include <algorithm>
#include <cmath>

#include <Windows.h>
#include <shobjidl.h>

fa::GUI::GUI(ApplicationContext& context) : m_Context(context) {
    m_Window.create(sf::VideoMode({ 640, 480 }), "ForrAI");
    m_Window.setVerticalSyncEnabled(true);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::SFML::Init(m_Window);

    ImGuiIO& io    = ImGui::GetIO();
    io.IniFilename = nullptr;

    if (std::filesystem::exists(m_Context.font_path)) {
        ImFontConfig font_config{};
        font_config.FontLoaderFlags = ImGuiFreeTypeLoaderFlags_NoHinting |
                                      ImGuiFreeTypeLoaderFlags_MonoHinting |
                                      ImGuiFreeTypeLoaderFlags_Bold;

        io.Fonts->SetFontLoader(ImGuiFreeType::GetFontLoader());

        ImFont* font = io.Fonts->AddFontFromFileTTF(m_Context.font_path.string().c_str(),
                                                    36.0f,
                                                    &font_config,
                                                    io.Fonts->GetGlyphRangesDefault());
        if (font) {
            io.FontDefault     = font;
            io.FontGlobalScale = 0.5f;

            if (!ImGui::SFML::UpdateFontTexture()) {
                reportError(m_Context, "Failed to setup font. Failed to update font texture for ImGui::SFML.\nUsing default font");
            }
        }
    }
    else {
        reportError(m_Context, "Failed to setup font. File does not exist.\nUsing default font\nPath : " + m_Context.font_path.string());
    }

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

    // begin background window
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                                 ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("##ui", nullptr, flags);
    }

    this->DrawMainMenuBar();
    this->DrawCanvas();
    this->DrawPredictions();

    if (!m_Context.error_message.empty() && m_Context.error_displaying_time > 0.0f) {
        ImGuiIO& io = ImGui::GetIO();

        float moving_offset = std::clamp(m_Context.error_displaying_time * 15, 2.0f, 25.0f);

        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f,
                                       io.DisplaySize.y - 40.0f - moving_offset + 2.0f),
                                ImGuiCond_Always,
                                ImVec2(0.5f, 0.5f));

        ImGuiWindowFlags error_flags = ImGuiWindowFlags_NoTitleBar |
                                       ImGuiWindowFlags_NoScrollbar |
                                       ImGuiWindowFlags_NoCollapse |
                                       ImGuiWindowFlags_NoFocusOnAppearing |
                                       ImGuiWindowFlags_NoNav |
                                       ImGuiWindowFlags_NoInputs |
                                       ImGuiWindowFlags_Tooltip;

        float alpha = m_Context.error_displaying_time;
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);

        ImGui::Begin("##ErrorMessage", nullptr, error_flags);
        ImGui::TextColored(ImVec4(0.9f, 0.22f, 0.2f, 1.0f), "ERROR : %s", m_Context.error_message.c_str());
        ImGui::End();

        ImGui::PopStyleVar();

        constexpr static float window_collapsing_speed = 1.5f;

        m_Context.error_displaying_time = std::max(0.0f, m_Context.error_displaying_time - window_collapsing_speed * io.DeltaTime);
    }
    else {
        m_Context.error_message.clear();
    }

    ImGui::End();

    m_Window.clear();
    ImGui::SFML::Render(m_Window);
    m_Window.display();
}

void fa::GUI::DrawMainMenuBar() {
    if (!ImGui::BeginMainMenuBar()) return;

    if (ImGui::BeginMenu("File")) {

        //if (ImGui::MenuItem("Save")) {
        //}

        if (ImGui::MenuItem("Load")) {

            auto path = this->openFile();

            if (path.has_value()) {
                m_Context.path_to_file = path.value();
                m_Context.is_reading   = true;
            }
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("About")) {
        ImGui::Text("Forr-AI 1.0v\nMay 2026\n© 2026 Farrakh Fattakhov.");
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void fa::GUI::DrawCanvas() {
    float available_width  = ImGui::GetContentRegionAvail().x;
    float available_height = ImGui::GetContentRegionAvail().y;

    float scale_x = available_width / static_cast<float>(fa::ApplicationContext::canvas_width);
    float scale_y = available_height / static_cast<float>(fa::ApplicationContext::canvas_height);

    float pixel_size = std::floor(std::min(scale_x, scale_y));

    constexpr static ImGuiWindowFlags static_window_flags = ImGuiWindowFlags_NoMove |
                                                            ImGuiWindowFlags_NoCollapse |
                                                            ImGuiWindowFlags_NoScrollbar |
                                                            ImGuiWindowFlags_NoScrollWithMouse;

    ImVec2 display_size    = ImGui::GetIO().DisplaySize;
    float  menu_bar_height = ImGui::GetFrameHeight();
    float  output_width    = display_size.x * 0.3f;
    float  canvas_width    = display_size.x - output_width;
    float  content_height  = display_size.y - menu_bar_height;

    ImGui::SetNextWindowPos(ImVec2(0.0f, menu_bar_height));
    ImGui::SetNextWindowSize(ImVec2(canvas_width, content_height));

    ImGui::Begin("Canvas", nullptr, static_window_flags);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 canvas_screen_position = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size            = ImVec2(fa::ApplicationContext::canvas_width * pixel_size,
                                           fa::ApplicationContext::canvas_height * pixel_size);

    // draw the canvas
    ImGui::Image(m_CanvasTexture, sf::Vector2f(canvas_size.x, canvas_size.y));

    // draw the grid
    constexpr ImU32 grid_color = IM_COL32(30, 30, 30, 255);

    for (int x = 0; x <= fa::ApplicationContext::canvas_width; x++) {
        float line_x = canvas_screen_position.x + static_cast<float>(x) * pixel_size;

        draw_list->AddLine(ImVec2(line_x, canvas_screen_position.y),
                           ImVec2(line_x, canvas_screen_position.y + canvas_size.y),
                           grid_color);
    }

    for (int y = 0; y <= fa::ApplicationContext::canvas_height; y++) {
        float line_y = canvas_screen_position.y + static_cast<float>(y) * pixel_size;

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

    if (!ImGui::IsWindowHovered()) {
        // texture upload
        this->updateCanvasTexture();

        ImGui::End();
        return;
    }

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
        float visual_radius = static_cast<float>(m_Context.brush_size) * pixel_size;
        draw_list->AddCircle(mouse_position, visual_radius, IM_COL32(255, 255, 0, 180), 16, 2.0f);
    }

    // drawing by brush
    bool is_drawing = ImGui::IsMouseDown(ImGuiMouseButton_Left) ||
                      ImGui::IsMouseDown(ImGuiMouseButton_Right);

    if (is_mouse_inside_canvas && is_drawing) {
        int canvas_x = static_cast<int>((mouse_position.x - canvas_screen_position.x) / pixel_size);
        int canvas_y = static_cast<int>((mouse_position.y - canvas_screen_position.y) / pixel_size);

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
    this->updateCanvasTexture();

    ImGui::End();
}

void fa::GUI::DrawPredictions() {
    constexpr static ImGuiWindowFlags static_window_flags = ImGuiWindowFlags_NoMove |
                                                            ImGuiWindowFlags_NoCollapse |
                                                            ImGuiWindowFlags_NoScrollbar;

    ImVec2 display_size    = ImGui::GetIO().DisplaySize;
    float  menu_bar_height = ImGui::GetFrameHeight();
    float  output_width    = display_size.x * 0.3f;
    float  canvas_width    = display_size.x - output_width;
    float  content_height  = display_size.y - menu_bar_height;

    ImGui::SetNextWindowPos(ImVec2(canvas_width, menu_bar_height));
    ImGui::SetNextWindowSize(ImVec2(output_width, content_height));

    ImGui::Begin("Output", nullptr, static_window_flags);

    int answer = (int) fa::classify(m_Context.current_predictions);

    ImGui::SetCursorPosY(40);
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Predicted :");

    ImGui::SetWindowFontScale(4.0f);
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 20);
    ImGui::Text("%d", answer);
    ImGui::SetWindowFontScale(1.0f);

    ImGui::Separator();
    ImGui::Spacing();

    for (int i = 0; i < 10; i++) {
        float prediction = (float) m_Context.current_predictions[i];

        ImGui::Text("%d:", i);
        ImGui::SameLine();

        if (i == answer)
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.8f, 0.2f, 1.0f));
        else
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));

        ImGui::ProgressBar(prediction, ImVec2(-1, 15), "");
        ImGui::PopStyleColor();
    }

    if (ImGui::Button("Clear All", ImVec2(-1, 30))) {
        std::ranges::fill(m_Context.canvas_pixels, 0.0);
        m_IsCanvasTextureDirty    = true;
        m_Context.is_canvas_dirty = true;
    }

    ImGui::End();
}

void fa::GUI::updateCanvasTexture() {
    if (!m_IsCanvasTextureDirty) return;

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

    m_IsCanvasTextureDirty = false;
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

    m_IsCanvasTextureDirty    = true;
    m_Context.is_canvas_dirty = true;
}

std::optional<std::filesystem::path> fa::GUI::openFile() const {
    HRESULT f_sys_hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(f_sys_hr)) return std::nullopt;

    IFileOpenDialog* f_file_system{};
    f_sys_hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_file_system));
    if (FAILED(f_sys_hr)) {
        CoUninitialize();
        return std::nullopt;
    }

    f_sys_hr = f_file_system->Show(NULL);
    if (FAILED(f_sys_hr)) {
        f_file_system->Release();
        CoUninitialize();
        return std::nullopt;
    }

    IShellItem* f_files{};
    f_sys_hr = f_file_system->GetResult(&f_files);
    if (FAILED(f_sys_hr)) {
        f_file_system->Release();
        CoUninitialize();
        return std::nullopt;
    }

    PWSTR f_path{};
    f_sys_hr = f_files->GetDisplayName(SIGDN_FILESYSPATH, &f_path);
    if (FAILED(f_sys_hr)) {
        f_files->Release();
        f_file_system->Release();
        CoUninitialize();
        return std::nullopt;
    }

    std::filesystem::path path = f_path;

    CoTaskMemFree(f_path);
    f_files->Release();
    f_file_system->Release();
    CoUninitialize();

    return path;
}

bool fa::GUI::isPixelInsideCanvas(int x, int y) const {
    return x >= 0 && x < fa::ApplicationContext::canvas_width &&
           y >= 0 && y < fa::ApplicationContext::canvas_height;
}
