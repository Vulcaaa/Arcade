/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** ADisplayModule
*/

/// @file ADisplayModule.cpp
/// @brief Implementation of the ADisplayModule class, an abstract base class for display modules in
/// the Arcade project

#include "ADisplayModule.hpp"
#include <cmath>
#include <filesystem>
#include <array>

/// @brief Valid image formats for sprites
static const std::array<std::string_view, 3> VALID_FORMATS = {".png", ".jpg", ".jpeg"};

arcade::ADisplayModule::ADisplayModule(std::string name, RenderContext context)
    : _name(name),
      _title(""),
      _context(context),
      _fullScreen(false),
      _deltaTime(0),
      _lastTime(std::chrono::high_resolution_clock::now())
{
}

arcade::Key arcade::ADisplayModule::getKey()
{
    if (_keyEvent.empty()) return Key::UNKNOWN;

    Key lastKey = _keyEvent.front();
    _keyEvent.pop();

    return lastKey;
}

arcade::MouseEvent arcade::ADisplayModule::getMouseEvent()
{
    if (_mouseEvent.empty()) return MouseEvent{{0, 0}, Key::UNKNOWN};

    MouseEvent lastMouse = _mouseEvent.front();
    _mouseEvent.pop();

    return lastMouse;
}

// --- Getters ---

arcade::ModuleType arcade::ADisplayModule::getType() const { return ModuleType::DISPLAY; }
const std::string& arcade::ADisplayModule::getName() const { return _name; }

void arcade::ADisplayModule::updateDeltaTime()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    _deltaTime = std::chrono::duration<float>(currentTime - _lastTime).count();
    _lastTime = currentTime;
}

float arcade::ADisplayModule::getDeltaTime() const { return _deltaTime; }

int arcade::ADisplayModule::getWidth() const { return _context.width; }
int arcade::ADisplayModule::getHeight() const { return _context.height; }

// --- Setters ---

void arcade::ADisplayModule::setTitle(const std::string& title) { _title = title; }
void arcade::ADisplayModule::setWindowSize(int width, int height)
{
    _context.width = width;
    _context.height = height;
}
void arcade::ADisplayModule::setFullscreen(bool fullscreen) { _fullScreen = fullscreen; }
void arcade::ADisplayModule::setRenderContext(const RenderContext& context) { _context = context; }

bool arcade::ADisplayModule::isValidFormat(const std::string& path)
{
    std::filesystem::path p(path);

    if (std::filesystem::exists(p) && std::filesystem::is_regular_file(p)) {
        std::string ext = p.extension().string();
        return std::find(VALID_FORMATS.begin(), VALID_FORMATS.end(), std::string_view(ext)) !=
               VALID_FORMATS.end();
    }
    return false;
}

void arcade::ADisplayModule::applyAnchor(int& x, int& y, int width, int height, Anchor anchor) const
{
    switch (anchor) {
        case Anchor::TOP_LEFT:      break;
        case Anchor::TOP_CENTER:    x -= width / 2; break;
        case Anchor::TOP_RIGHT:     x -= width; break;
        case Anchor::MIDDLE_LEFT:   y -= height / 2; break;
        case Anchor::CENTER:        x -= width / 2; y -= height / 2; break;
        case Anchor::MIDDLE_RIGHT:  x -= width; y -= height / 2; break;
        case Anchor::BOTTOM_LEFT:   y -= height; break;
        case Anchor::BOTTOM_CENTER: x -= width / 2; y -= height; break;
        case Anchor::BOTTOM_RIGHT:  x -= width; y -= height; break;
    }
}

void arcade::ADisplayModule::applyScaling(int& x, int& y, int& width, int& height) const
{
    auto [scale, offsetX, offsetY] = computeViewport();

    float x0 = x * scale;
    float y0 = y * scale;
    float x1 = (x + width) * scale;
    float y1 = (y + height) * scale;

    x = static_cast<int>(std::floor(x0)) + offsetX;
    y = static_cast<int>(std::floor(y0)) + offsetY;
    width = static_cast<int>(std::ceil(x1)) - static_cast<int>(std::floor(x0));
    height = static_cast<int>(std::ceil(y1)) - static_cast<int>(std::floor(y0));
}