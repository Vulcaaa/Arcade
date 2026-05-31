/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** SnakeGame
*/

#include "SnakeGame.hpp"

#include <random>

const std::string GAMENAME = "Snake";

arcade::SnakeGame::SnakeGame() : AGameModule(GAMENAME, GameState::RUNNING, RenderContext(12, 11, CoordinateSpace::GRID))
{
    setSnakeOrientedSprites();
}

void arcade::SnakeGame::setSnakeOrientedSprites()
{
    setOrientedSprite("head", Orientation::UP, "sprite/snake/head/snakeHeadTop.png");
    setOrientedSprite("head", Orientation::DOWN, "sprite/snake/head/snakeHeadDown.png");
    setOrientedSprite("head", Orientation::LEFT, "sprite/snake/head/snakeHeadLeft.png");
    setOrientedSprite("head", Orientation::RIGHT, "sprite/snake/head/snakeHeadRight.png");
    setOrientedSprite("head", Orientation::NONE, "sprite/snake/head/snakeHeadRight.png");

    setOrientedSprite("body", Orientation::UP, "sprite/snake/body/snakeBodyY.png");
    setOrientedSprite("body", Orientation::DOWN, "sprite/snake/body/snakeBodyY.png");
    setOrientedSprite("body", Orientation::LEFT, "sprite/snake/body/snakeBodyX.png");
    setOrientedSprite("body", Orientation::RIGHT, "sprite/snake/body/snakeBodyX.png");
    setOrientedSprite("body", Orientation::NONE, "sprite/snake/body/snakeBodyX.png");

    setOrientedSprite("tail", Orientation::UP, "sprite/snake/tail/snakeTailTop.png");
    setOrientedSprite("tail", Orientation::DOWN, "sprite/snake/tail/snakeTailDown.png");
    setOrientedSprite("tail", Orientation::LEFT, "sprite/snake/tail/snakeTailLeft.png");
    setOrientedSprite("tail", Orientation::RIGHT, "sprite/snake/tail/snakeTailRight.png");
    setOrientedSprite("tail", Orientation::NONE, "sprite/snake/tail/snakeTailRight.png");

    setOrientedSprite("angleTopLeft", Orientation::NONE, "sprite/snake/angle/snakeAngleTopLeft.png");
    setOrientedSprite("angleTopRight", Orientation::NONE, "sprite/snake/angle/snakeAngleTopRight.png");
    setOrientedSprite("angleDownLeft", Orientation::NONE, "sprite/snake/angle/snakeAngleDownLeft.png");
    setOrientedSprite("angleDownRight", Orientation::NONE, "sprite/snake/angle/snakeAngleDownRight.png");

    setOrientedSprite("fruit", Orientation::NONE, "sprite/snake/fruit.png");
    setOrientedSprite("wall", Orientation::NONE, "sprite/snake/wall.png");
    setOrientedSprite("floor", Orientation::NONE, "sprite/snake/floor.png");
}

void arcade::SnakeGame::update(float elapsedTime)
{
    if (_state == GameState::RUNNING) {
        _timeSinceLastMove += elapsedTime;
        if (_timeSinceLastMove >= _moveInterval) {
            moveSnake();
            _timeSinceLastMove = 0.0;
        }
    }

    _uiText.clear();
    _entities.clear();

    updateFloorEntities();
    updateSnakeEntities();
    updateFruitEntities();
    updateWallEntities();

    // Update UI texts
    appendCommonUITexts();
}

void arcade::SnakeGame::updateSnakeEntities()
{
    // Helper to calculate orientation from one body part to another
    auto getOr = [](const Position& from, const Position& to) {
        if (from.x > to.x) return Orientation::RIGHT;
        if (from.x < to.x) return Orientation::LEFT;
        if (from.y > to.y) return Orientation::DOWN;
        if (from.y < to.y) return Orientation::UP;
        return Orientation::NONE;
    };

    // Update snake body / head entities
    for (size_t i = 0; i < _snakeBody.size(); ++i) {
        Entity part;
        part.pos = _snakeBody[i];

        if (i == 0) {
            part.character = 'O';
            part.color = Colors::YELLOW;
            part.spritePath = getOrientedSprite("head", _currentDir);
        } else if (i == _snakeBody.size() - 1) {
            part.character = '.';
            part.color = Colors::GREEN;
            Orientation tailDir = getOr(_snakeBody[i-1], _snakeBody[i]);
            part.spritePath = getOrientedSprite("tail", tailDir);
        } else {
            part.character = 'o';
            part.color = Colors::GREEN;
            
            Orientation dir1 = getOr(_snakeBody[i-1], _snakeBody[i]);
            Orientation dir2 = getOr(_snakeBody[i+1], _snakeBody[i]);
            
            bool isAngle = false;
            std::string angleSprite = "";

            if ((dir1 == Orientation::UP && dir2 == Orientation::LEFT) ||
                (dir1 == Orientation::LEFT && dir2 == Orientation::UP)) {
                isAngle = true;
                angleSprite = getOrientedSprite("angleTopLeft", Orientation::NONE);
            } else if ((dir1 == Orientation::UP && dir2 == Orientation::RIGHT) ||
                       (dir1 == Orientation::RIGHT && dir2 == Orientation::UP)) {
                isAngle = true;
                angleSprite = getOrientedSprite("angleTopRight", Orientation::NONE);
            } else if ((dir1 == Orientation::DOWN && dir2 == Orientation::LEFT) ||
                       (dir1 == Orientation::LEFT && dir2 == Orientation::DOWN)) {
                isAngle = true;
                angleSprite = getOrientedSprite("angleDownLeft", Orientation::NONE);
            } else if ((dir1 == Orientation::DOWN && dir2 == Orientation::RIGHT) ||
                       (dir1 == Orientation::RIGHT && dir2 == Orientation::DOWN)) {
                isAngle = true;
                angleSprite = getOrientedSprite("angleDownRight", Orientation::NONE);
            }

            if (isAngle) part.spritePath = angleSprite;
            else part.spritePath = getOrientedSprite("body", dir1);
        }
        _entities.push_back(part);
    }
}

void arcade::SnakeGame::updateFruitEntities()
{
    // Update fruit entities
    for (const auto& fruitPos : _fruits) {
        Entity fruit;
        fruit.pos = fruitPos;
        fruit.character = '#';
        fruit.color = Colors::RED;
        fruit.spritePath = getOrientedSprite("fruit", Orientation::NONE);
        _entities.push_back(fruit);
    }
}

void arcade::SnakeGame::updateFloorEntities()
{
    Color floorColor = {246, 236, 215};
    // Update floor entities (Inside the walls)
    for (int y = 1; y < _context.height - 1; y++) {
        for (int x = 1; x < _context.width - 1; x++) {
            Entity floor;
            floor.pos = {static_cast<float>(x), static_cast<float>(y)};
            floor.character = ' ';
            floor.color = floorColor;
            floor.spritePath = getOrientedSprite("floor", Orientation::NONE);
            _entities.push_back(floor);
        }
    }
}

void arcade::SnakeGame::updateWallEntities()
{
    Color wallColor = {92, 64, 51};
    // Update wall entities (Top and Bottom)
    for (int x = 0; x < _context.width; x++) {
        Entity wallTop;
        wallTop.pos = {static_cast<float>(x), 0};
        wallTop.character = '-';
        wallTop.color = wallColor;
        wallTop.spritePath = getOrientedSprite("wall", Orientation::NONE);
        _entities.push_back(wallTop);

        Entity wallBottom;
        wallBottom.pos = {static_cast<float>(x), static_cast<float>(_context.height - 1)};
        wallBottom.character = '-';
        wallBottom.color = wallColor;
        wallBottom.spritePath = getOrientedSprite("wall", Orientation::NONE);
        _entities.push_back(wallBottom);
    }

    // Update wall entities (Left and Right)
    for (int y = 0; y < _context.height; y++) {
        Entity wallLeft;
        wallLeft.pos = {0, static_cast<float>(y)};
        wallLeft.character = '|';
        wallLeft.color = wallColor;
        wallLeft.spritePath = getOrientedSprite("wall", Orientation::NONE);
        _entities.push_back(wallLeft);

        Entity wallRight;
        wallRight.pos = {static_cast<float>(_context.width - 1), static_cast<float>(y)};
        wallRight.character = '|';
        wallRight.color = wallColor;
        wallRight.spritePath = getOrientedSprite("wall", Orientation::NONE);
        _entities.push_back(wallRight);
    }
}

void arcade::SnakeGame::handleInput(Key key)
{
    if (key == Key::P) {
        if (_state == GameState::RUNNING)
            pause();
        else if (_state == GameState::PAUSED)
            resume();
        return;
    }

    if (key == Key::UP || key == Key::Z)
        if (_currentDir != Orientation::DOWN) _nextDir = Orientation::UP;
    if (key == Key::DOWN || key == Key::S)
        if (_currentDir != Orientation::UP) _nextDir = Orientation::DOWN;
    if (key == Key::LEFT || key == Key::Q)
        if (_currentDir != Orientation::RIGHT) _nextDir = Orientation::LEFT;
    if (key == Key::RIGHT || key == Key::D)
        if (_currentDir != Orientation::LEFT) _nextDir = Orientation::RIGHT;
}

void arcade::SnakeGame::handleInput(MouseEvent mouseEvent)
{
    (void)mouseEvent;
}

void arcade::SnakeGame::restart()
{
    _state = GameState::RUNNING;
    _score = 0;
    _timeSinceLastMove = static_cast<float>(0.0);
    _moveInterval = static_cast<float>(0.10);
    _currentDir = Orientation::NONE;
    _nextDir = Orientation::NONE;

    _snakeBody.clear();
    int midX = _context.width / 2;
    int midY = _context.height / 2;
    _snakeBody.push_back({static_cast<float>(midX - 1), static_cast<float>(midY)});
    _snakeBody.push_back({static_cast<float>(midX - 2), static_cast<float>(midY)});
    _snakeBody.push_back({static_cast<float>(midX - 3), static_cast<float>(midY)});
    _snakeBody.push_back({static_cast<float>(midX - 4), static_cast<float>(midY)});

    _fruits.clear();
    spawnFruit();
}

void arcade::SnakeGame::moveSnake()
{
    if (_nextDir != Orientation::NONE) {
        if (_nextDir == Orientation::UP && _currentDir != Orientation::DOWN)
            _currentDir = _nextDir;
        else if (_nextDir == Orientation::DOWN && _currentDir != Orientation::UP)
            _currentDir = _nextDir;
        else if (_nextDir == Orientation::LEFT && _currentDir != Orientation::RIGHT && _currentDir != Orientation::NONE)
            _currentDir = _nextDir;
        else if (_nextDir == Orientation::RIGHT && _currentDir != Orientation::LEFT)
            _currentDir = _nextDir;
    }

    Position nextHead = _snakeBody.front();

    switch (_currentDir) {
        case Orientation::UP:
            nextHead.y -= 1;
            break;
        case Orientation::DOWN:
            nextHead.y += 1;
            break;
        case Orientation::LEFT:
            nextHead.x -= 1;
            break;
        case Orientation::RIGHT:
            nextHead.x += 1;
            break;
        case Orientation::NONE:
            return;
    }

    /// Wall Collision (Game Over)
    if (_context.width <= 2 || _context.height <= 2
        || nextHead.x < 1 || nextHead.x >= _context.width - 1
        || nextHead.y < 1 || nextHead.y >= _context.height - 1) {
        _state = GameState::GAME_OVER;
        return;
    }

    /// Self Collision (Game Over)
    for (size_t i = 0; i < _snakeBody.size() - 1; ++i) {
        if (_snakeBody[i].x == nextHead.x && _snakeBody[i].y == nextHead.y) {
            _state = GameState::GAME_OVER;
            return;
        }
    }

    /// Add the new head
    _snakeBody.push_front(nextHead);

    /// Check if we ate a fruit
    bool ateFruit = false;
    for (auto& fruitPos : _fruits) {
        if (fruitPos.x == nextHead.x && fruitPos.y == nextHead.y) {
            _score += 10;
            _fruits.clear();
            spawnFruit();
            ateFruit = true;
            break;
        }
    }
    /// If we didn't eat a fruit, remove the tail
    if (!ateFruit) _snakeBody.pop_back();

    /// Check for win condition
    if (_snakeBody.size() >= static_cast<size_t>((_context.width - 2) * (_context.height - 2)))
        _state = GameState::WON;
}

void arcade::SnakeGame::spawnFruit()
{
    if (_context.width <= 2 || _context.height <= 2) {
        _state = GameState::GAME_OVER;
        return;
    }

    if (_snakeBody.size() >= static_cast<size_t>((_context.width - 2) * (_context.height - 2))) {
        _state = GameState::WON;
        return;
    }

    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> distX(1, _context.width - 2);
    std::uniform_int_distribution<int> distY(1, _context.height - 2);

    bool validPosition = false;
    float randX = 0;
    float randY = 0;

    while (!validPosition) {
        randX = static_cast<float>(distX(rng));
        randY = static_cast<float>(distY(rng));
        validPosition = true;

        for (const auto& part : _snakeBody) {
            if (part.x == randX && part.y == randY) {
                validPosition = false;
                break;
            }
        }

        for (const auto& fruit : _fruits) {
            if (fruit.x == randX && fruit.y == randY) {
                validPosition = false;
                break;
            }
        }
    }
    _fruits.push_back({randX, randY});
}

extern "C" {
    arcade::IModule* entryPoint() {
        return new arcade::SnakeGame();
    }
}
