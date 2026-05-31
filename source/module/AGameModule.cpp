/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** AGameModule
*/

#include "AGameModule.hpp"

arcade::AGameModule::AGameModule(std::string name, GameState initialState, RenderContext context)
    : _name(name), _state(initialState), _context(context), _score(0)
{}

const std::vector<arcade::Entity>& arcade::AGameModule::getEntities() const { return _entities; }

const std::vector<arcade::Text>& arcade::AGameModule::getUIText() const { return _uiText; }

arcade::RenderContext arcade::AGameModule::getRenderContext() const { return _context; }

arcade::ModuleType arcade::AGameModule::getType() const { return ModuleType::GAME; }

const std::string& arcade::AGameModule::getName() const { return _name; }

arcade::GameState arcade::AGameModule::getState() const { return _state; }

int arcade::AGameModule::getScore() const { return _score; }

void arcade::AGameModule::stop() {}

void arcade::AGameModule::pause()
{
    if (_state == GameState::RUNNING) _state = GameState::PAUSED;
}

void arcade::AGameModule::resume()
{
    if (_state == GameState::PAUSED) _state = GameState::RUNNING;
}

void arcade::AGameModule::init() { restart(); }

void arcade::AGameModule::appendCommonUITexts()
{
    float midX = static_cast<float>(_context.width) / 2;
    float midY = static_cast<float>(_context.height) / 2;

    // Score display at top left
    std::string scoreStr = "Score: " + std::to_string(_score);
    _uiText.push_back(Text(
        scoreStr,
        1,
        0,
        Anchor::TOP_LEFT,
        Colors::WHITE,
        20
    ));

    // Game title centered at the top
    _uiText.push_back(Text(
        _name,
        midX,
        0,
        Anchor::TOP_CENTER,
        Colors::WHITE,
        20
    ));

    // Bottom left key summary
    _uiText.push_back(Text(
        "[R] Restart  [P] Pause  [M] Menu",
        1,
        static_cast<float>(_context.height),
        Anchor::BOTTOM_LEFT,
        Colors::WHITE,
        20
    ));

    // If paused, add a large "PAUSED" text in the center
    if (_state == GameState::PAUSED)
        _uiText.push_back(Text(
            "PAUSED",
            midX,
            midY,
            Anchor::CENTER,
            Colors::YELLOW,
            30
        ));

    // If game over, add a large "GAME OVER" text in the center
    if (_state == GameState::GAME_OVER)
        _uiText.push_back(Text(
            "GAME OVER",
            midX,
            midY,
            Anchor::CENTER,
            Colors::RED,
            30
        ));

    if (_state == GameState::WON)
        _uiText.push_back(Text(
            "YOU WIN!",
            midX,
            midY,
            Anchor::CENTER,
            Colors::GREEN,
            30
        ));
}

std::string arcade::AGameModule::getOrientedSprite(const std::string& key, Orientation orientation) const
{
    auto it = _orientedSprites.find({key, orientation});
    if (it != _orientedSprites.end())
        return it->second;
    return "";
}

void arcade::AGameModule::setOrientedSprite(const std::string& key, Orientation orientation, const std::string& spritePath)
{
    _orientedSprites[{key, orientation}] = spritePath;
}
