/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** LIBCACADisplay
*/

#include "ALLEGRO5Display.hpp"
#include "IDisplayModule.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <allegro5/events.h>
#include <allegro5/keycodes.h>
#include <unordered_map>

static const std::string DISPLAY_NAME = "Allegro5";

/// @brief Mapping of Allegro key codes to arcade::Key values
static const std::unordered_map<int, arcade::Key> KEY_BINDINGS = {
    {ALLEGRO_KEY_UP, arcade::Key::UP},
    {ALLEGRO_KEY_DOWN, arcade::Key::DOWN},
    {ALLEGRO_KEY_LEFT, arcade::Key::LEFT},
    {ALLEGRO_KEY_RIGHT, arcade::Key::RIGHT},
    {ALLEGRO_KEY_SPACE, arcade::Key::SPACE},
    {ALLEGRO_KEY_ENTER, arcade::Key::ENTER},

    {ALLEGRO_KEY_ESCAPE, arcade::Key::EXIT},
    {ALLEGRO_KEY_BACKSPACE, arcade::Key::BACKSPACE},

    {ALLEGRO_KEY_0, arcade::Key::NUM_0},
    {ALLEGRO_KEY_1, arcade::Key::PREV_GAME},
    {ALLEGRO_KEY_2, arcade::Key::NEXT_GAME},
    {ALLEGRO_KEY_3, arcade::Key::PREV_GRAPHICS},
    {ALLEGRO_KEY_4, arcade::Key::NEXT_GRAPHICS},
    {ALLEGRO_KEY_5, arcade::Key::NUM_5},
    {ALLEGRO_KEY_6, arcade::Key::NUM_6},
    {ALLEGRO_KEY_7, arcade::Key::NUM_7},
    {ALLEGRO_KEY_8, arcade::Key::NUM_8},
    {ALLEGRO_KEY_9, arcade::Key::NUM_9},

    {ALLEGRO_KEY_A, arcade::Key::A},
    {ALLEGRO_KEY_B, arcade::Key::B},
    {ALLEGRO_KEY_C, arcade::Key::C},
    {ALLEGRO_KEY_D, arcade::Key::D},
    {ALLEGRO_KEY_E, arcade::Key::E},
    {ALLEGRO_KEY_F, arcade::Key::F},
    {ALLEGRO_KEY_G, arcade::Key::G},
    {ALLEGRO_KEY_H, arcade::Key::H},
    {ALLEGRO_KEY_I, arcade::Key::I},
    {ALLEGRO_KEY_J, arcade::Key::J},
    {ALLEGRO_KEY_K, arcade::Key::K},
    {ALLEGRO_KEY_L, arcade::Key::L},
    {ALLEGRO_KEY_M, arcade::Key::MENU},
    {ALLEGRO_KEY_N, arcade::Key::N},
    {ALLEGRO_KEY_O, arcade::Key::O},
    {ALLEGRO_KEY_P, arcade::Key::P},
    {ALLEGRO_KEY_Q, arcade::Key::Q},
    {ALLEGRO_KEY_R, arcade::Key::RESTART},
    {ALLEGRO_KEY_S, arcade::Key::S},
    {ALLEGRO_KEY_T, arcade::Key::T},
    {ALLEGRO_KEY_U, arcade::Key::U},
    {ALLEGRO_KEY_V, arcade::Key::V},
    {ALLEGRO_KEY_W, arcade::Key::W},
    {ALLEGRO_KEY_X, arcade::Key::X},
    {ALLEGRO_KEY_Y, arcade::Key::Y},
    {ALLEGRO_KEY_Z, arcade::Key::Z}
};

/// @brief Mapping of Allegro mouse button codes to arcade::Key values
static const std::unordered_map<int, arcade::Key> MOUSE_BINDINGS = {
    {1, arcade::Key::MOUSE_LEFT},
    {2, arcade::Key::MOUSE_RIGHT},
    {3, arcade::Key::MOUSE_MIDDLE},
};

arcade::ALLEGRO5::ALLEGRO5() : ADisplayModule(DISPLAY_NAME), _win(nullptr), _queue(nullptr) {}

arcade::ALLEGRO5::~ALLEGRO5() { stop(); }

void arcade::ALLEGRO5::init()
{
    if (!al_init()) throw std::runtime_error("Failed to initialize Allegro");

    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    _queue = al_create_event_queue();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    _win = al_create_display(_context.width, _context.height);
    if (!_win) throw std::runtime_error("Failed to create Allegro display");

    ALLEGRO_MONITOR_INFO info;
    if (al_get_monitor_info(0, &info)) {
        int monitorW = info.x2 - info.x1;
        int monitorH = info.y2 - info.y1;
        int posX = info.x1 + (monitorW - _context.width) / 2;
        int posY = info.y1 + (monitorH - _context.height) / 2;
        al_set_window_position(_win, posX, posY);
    }

    al_set_window_title(_win, "Arcade");

    al_install_keyboard();
    al_install_mouse();

    al_register_event_source(_queue, al_get_keyboard_event_source());
    al_register_event_source(_queue, al_get_mouse_event_source());
    al_register_event_source(_queue, al_get_display_event_source(_win));
}

void arcade::ALLEGRO5::stopFont()
{
    for (auto& pair : _fontCache) {
        if (pair.second) al_destroy_font(pair.second);
    }
    _fontCache.clear();
}

void arcade::ALLEGRO5::stopSprites()
{
    for (auto& pair : _spriteCache) {
        if (pair.second) al_destroy_bitmap(pair.second);
    }
    _spriteCache.clear();
}

void arcade::ALLEGRO5::stop()
{
    stopFont();

    stopSprites();

    if (_queue) {
        al_destroy_event_queue(_queue);
        _queue = nullptr;
    }
    if (_win) {
        al_destroy_display(_win);
        _win = nullptr;
    }
}

void arcade::ALLEGRO5::clear() { al_clear_to_color(al_map_rgb(0, 0, 0)); }

void arcade::ALLEGRO5::display() { al_flip_display(); }

ALLEGRO_FONT* arcade::ALLEGRO5::tryOpenFont(const std::string& fontPath, int size)
{
    ALLEGRO_FONT* font = al_load_ttf_font(fontPath.c_str(), size, 0);
    return font;
}

ALLEGRO_FONT* arcade::ALLEGRO5::getFallbackFont(int size)
{
    std::vector<std::string> fallbackFonts = {
        "font/defaultFont.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/Library/Fonts/Arial.ttf",
        "C:\\Windows\\Fonts\\Arial.ttf"
    };

    for (const auto& path : fallbackFonts) {
        ALLEGRO_FONT* font = tryOpenFont(path, size);
        if (font) return font;
    }
    return nullptr;
}

ALLEGRO_FONT* arcade::ALLEGRO5::getFont(const std::string& path, int size)
{
    auto key = std::make_pair(path, size);

    auto it = _fontCache.find(key);
    if (it != _fontCache.end()) return it->second;

    ALLEGRO_FONT* font = tryOpenFont(path, size);

    if (!font) font = getFallbackFont(size);

    if (!font)
        throw std::runtime_error("Failed to load font: " + path);

    _fontCache[key] = font;
    return font;
}

void arcade::ALLEGRO5::drawText(const Text& text)
{
    int x = text.pos.x;
    int y = text.pos.y;
    ALLEGRO_FONT* font = getFont(text.fontPath, text.fontSize);

    if (!font) return;

    int width = al_get_text_width(font, text.content.c_str());
    int height = al_get_font_line_height(font);

    applyAnchor(x, y, width, height, text.anchor);
    applyScaling(x, y, width, height);
    al_draw_text(
        font,
        al_map_rgb(text.color.r, text.color.g, text.color.b),
        x, y,
        ALLEGRO_ALIGN_LEFT,
        text.content.c_str()
    );
}

ALLEGRO_BITMAP* arcade::ALLEGRO5::getSprite(const std::string& path)
{
    auto it = _spriteCache.find(path);

    if (it != _spriteCache.end()) return it->second;

    if (!isValidFormat(path)) return nullptr;

    ALLEGRO_BITMAP* bitmap = al_load_bitmap(path.c_str());
    if (!bitmap)
        return nullptr;
    _spriteCache[path] = bitmap;
    return bitmap;
}

void arcade::ALLEGRO5::drawEntity(const Entity& entity)
{
    int x = entity.pos.x;
    int y = entity.pos.y;
    int width = entity.size.x > 0 ? entity.size.x : 1;
    int height = entity.size.y > 0 ? entity.size.y : 1;
    applyScaling(x, y, width, height);
    applyAnchor(x, y, width, height, entity.anchor);

    ALLEGRO_BITMAP* bitmap = nullptr;
    if (!entity.spritePath.empty())
        bitmap = getSprite(entity.spritePath);

    if (bitmap)
        al_draw_scaled_bitmap(bitmap, 0, 0,
            al_get_bitmap_width(bitmap),
            al_get_bitmap_height(bitmap),
            x, y,
            width, height,
            0
        );
    else {
        Rect rect = {x, y, width, height};
        drawRectangle(rect, entity.color);
    }
}

void arcade::ALLEGRO5::drawRectangle(const Rect& rect, const Color& color)
{
    int width = rect.width > 0 ? rect.width : 1;
    int height = rect.height > 0 ? rect.height : 1;

    al_draw_filled_rectangle(rect.x, rect.y,
        rect.x + width, rect.y + height,
        al_map_rgb(color.r, color.g, color.b)
    );
}

void arcade::ALLEGRO5::mouseClickHandler(const ALLEGRO_EVENT& event)
{
    auto [scale, offsetX, offsetY] = computeViewport();

    float x = (static_cast<float>(event.mouse.x) - offsetX) / scale;
    float y = (static_cast<float>(event.mouse.y) - offsetY) / scale;

    x = std::clamp(x, 0.0f, 800.0f);
    y = std::clamp(y, 0.0f, 600.0f);

    auto it = MOUSE_BINDINGS.find(event.mouse.button);
    if (it != MOUSE_BINDINGS.end())
        _mouseEvent.push(MouseEvent{{x, y}, it->second});
}

void arcade::ALLEGRO5::pollEvents()
{
    updateDeltaTime();
    ALLEGRO_EVENT event;

    while (al_get_next_event(_queue, &event)) {
        switch (event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE: _win = nullptr; break;
        case ALLEGRO_EVENT_KEY_DOWN: {
            auto it = KEY_BINDINGS.find(event.keyboard.keycode);
            if (it != KEY_BINDINGS.end()) _keyEvent.push(it->second);
            break;
        } case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            mouseClickHandler(event);
            break;
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
            al_acknowledge_resize(_win);
            break;
        }
    }
}

bool arcade::ALLEGRO5::isOpen() const
{
    return true;
}

arcade::ADisplayModule::Viewport arcade::ALLEGRO5::computeViewport() const
{
    int winW = al_get_display_width(_win);
    int winH = al_get_display_height(_win);

    float scaleX = static_cast<float>(winW) / _context.width;
    float scaleY = static_cast<float>(winH) / _context.height;
    float scale  = std::min(scaleX, scaleY);

    int offsetX = static_cast<int>((winW - _context.width  * scale) / 2.0f);
    int offsetY = static_cast<int>((winH - _context.height * scale) / 2.0f);

    return {scale, offsetX, offsetY};
}

extern "C" {
    arcade::IModule* entryPoint() {
        return new arcade::ALLEGRO5();
    }
}