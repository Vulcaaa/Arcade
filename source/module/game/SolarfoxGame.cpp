/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SolarfoxGame
*/

#include "SolarfoxGame.hpp"
#include "AGameModule.hpp"
#include "IDisplayModule.hpp"
#include <algorithm>

const std::string GAMENAME = "Solarfox";
const float MOVE_INTERVAL = 0.0167f;
const float SPACESHIP_SPEED = 5.0f;
const float BULLET_SPEED = 17.0f;
const float ENEMY_FIRE_RATE = 3.0f;
const float ENEMY_BULLET_SPEED = 7.0f;

const int CELL_SIZE = 60;
const int START_X = 200;
const int START_Y = 100;
const int END_X = 600;
const int END_Y = 500;

arcade::Solarfox::Solarfox()
    : AGameModule(GAMENAME, GameState::RUNNING, RenderContext(800, 600, CoordinateSpace::PIXEL))
{
    setSolarfoxOrientedSprites();
}

void arcade::Solarfox::setSolarfoxOrientedSprites()
{
    setOrientedSprite("ship", Orientation::UP, "sprite/solarfox/ship/spaceshipTop.png");
    setOrientedSprite("ship", Orientation::RIGHT, "sprite/solarfox/ship/spaceshipRight.png");
    setOrientedSprite("ship", Orientation::DOWN, "sprite/solarfox/ship/spaceshipDown.png");
    setOrientedSprite("ship", Orientation::LEFT, "sprite/solarfox/ship/spaceshipLeft.png");
    setOrientedSprite("ship", Orientation::NONE, "sprite/solarfox/ship/spaceshipRight.png");

    setOrientedSprite("enemy", Orientation::UP, "sprite/solarfox/enemy/enemyTop.png");
    setOrientedSprite("enemy", Orientation::DOWN, "sprite/solarfox/enemy/enemyDown.png");
    setOrientedSprite("enemy", Orientation::RIGHT, "sprite/solarfox/enemy/enemyRight.png");
    setOrientedSprite("enemy", Orientation::LEFT, "sprite/solarfox/enemy/enemyLeft.png");

    setOrientedSprite("background", Orientation::NONE, "sprite/solarfox/solarfox_bg.png");
    setOrientedSprite("bullet", Orientation::NONE, "sprite/solarfox/bullet.png");
    setOrientedSprite("fireball", Orientation::NONE, "sprite/solarfox/fireball.png");
    setOrientedSprite("powerup", Orientation::NONE, "sprite/solarfox/diamond.png");
}

void arcade::Solarfox::update(float elapsedTime)
{
    if (_state == GameState::RUNNING) {
        _timeSinceLastMove += elapsedTime;
        if (_timeSinceLastMove >= _moveInterval) {
            updatePosition();
            detectCollisions();
            _timeSinceLastMove = 0.0;
        }
    }

    _entities.clear();
    _uiText.clear();
    // Game background
    Entity background;
    background.pos = {160, 60};
    background.size = {480, 480};
    background.spritePath = getOrientedSprite("background", Orientation::NONE);
    _entities.push_back(background);

    // Update UI texts
    appendCommonUITexts();

    // Bordures de la zone walkable
    Entity borderTop, borderBottom, borderLeft, borderRight;

    borderTop.pos = {START_X, START_Y};
    borderTop.size = {400, 2};
    borderTop.color = Colors::RED;

    borderBottom.pos = {START_X, END_Y};
    borderBottom.size = {400, 2};
    borderBottom.color = Colors::RED;

    borderLeft.pos = {START_X, START_Y};
    borderLeft.size = {2, 400};
    borderLeft.color = Colors::RED;

    borderRight.pos = {END_X, START_Y};
    borderRight.size = {2, 400};
    borderRight.color = Colors::RED;

    _entities.push_back(borderTop);
    _entities.push_back(borderBottom);
    _entities.push_back(borderLeft);
    _entities.push_back(borderRight);

    Entity borderTop2, borderBottom2, borderLeft2, borderRight2;

    borderTop2.pos = {START_X - 40, START_Y - 40};
    borderTop2.size = {480, 1};
    borderTop2.color = Colors::GREEN;

    borderBottom2.pos = {START_X - 40, END_Y + 40};
    borderBottom2.size = {480, 1};
    borderBottom2.color = Colors::GREEN;

    borderLeft2.pos = {START_X - 40, START_Y - 40};
    borderLeft2.size = {1, 480};
    borderLeft2.color = Colors::GREEN;

    borderRight2.pos = {END_X + 40, START_Y - 40};
    borderRight2.size = {1, 480};
    borderRight2.color = Colors::GREEN;

    _entities.push_back(borderTop2);
    _entities.push_back(borderBottom2);
    _entities.push_back(borderLeft2);
    _entities.push_back(borderRight2);

    for (const auto& enemy : _enemies) {
        Entity enemyEntity;
        enemyEntity.pos = enemy.pos;
        enemyEntity.size = {40, 40};
        enemyEntity.anchor = enemy.anchor;
        enemyEntity.character = 'E';
        enemyEntity.color = Colors::RED;
        enemyEntity.spritePath = enemy.spritePath;
        _entities.push_back(enemyEntity);
    }

    for (const auto& bullet : _bullets) {
        Entity bulletEntity;
        bulletEntity.pos = bullet.pos;
        bulletEntity.anchor = Anchor::CENTER;
        bulletEntity.size = { 25,25};
        bulletEntity.character = '*';
        bulletEntity.color = Colors::YELLOW;
        bulletEntity.spritePath = getOrientedSprite("bullet", Orientation::NONE);
        _entities.push_back(bulletEntity);
    }

    for (const auto& eb : _enemyBullets) {
        Entity ebEntity;
        ebEntity.pos = eb.pos;
        ebEntity.anchor = Anchor::CENTER;
        ebEntity.size = {20, 20};
        ebEntity.character = 'o';
        ebEntity.color = Colors::MAGENTA;
        ebEntity.spritePath = getOrientedSprite("fireball", Orientation::NONE);
        _entities.push_back(ebEntity);
    }

    for (const auto& powerup : _powerups) {
        if (powerup.collected) continue;

        Entity powerupEntity;
        powerupEntity.pos = powerup.pos;
        powerupEntity.size = {30, 30};
        powerupEntity.character = 'P';
        powerupEntity.color = Colors::BLUE;
        powerupEntity.spritePath = getOrientedSprite("powerup", Orientation::NONE);
        _entities.push_back(powerupEntity);
    }

    _ship.pos = _shipPosition;
    _ship.anchor = Anchor::CENTER;
    _ship.size = {55, 55};
    _ship.character = _shipChar;
    _ship.color = Colors::CYAN;
    _ship.backgroundColor = Colors::BLACK;
    _ship.spritePath = getOrientedSprite("ship", _shipDirection);
    _entities.push_back(_ship);
}

void arcade::Solarfox::updatePosition()
{
    // Move spaceship
    switch (_shipDirection) {
    case Orientation::UP:    _shipPosition.y -= _shipSpeed; break;
    case Orientation::DOWN:  _shipPosition.y += _shipSpeed; break;
    case Orientation::LEFT:  _shipPosition.x -= _shipSpeed; break;
    case Orientation::RIGHT: _shipPosition.x += _shipSpeed; break;
    default: break;
    }

    // Move player bullets
    for (auto& bullet : _bullets) {
        switch (bullet.dir) {
        case Orientation::UP:     bullet.pos.y -= BULLET_SPEED; break;
        case Orientation::DOWN:   bullet.pos.y += BULLET_SPEED; break;
        case Orientation::LEFT:   bullet.pos.x -= BULLET_SPEED; break;
        case Orientation::RIGHT:  bullet.pos.x += BULLET_SPEED; break;
        default: break;
        }
        bullet.distanceTraveled += BULLET_SPEED;
    }

    // Move enemies
    if (_shipDirection != Orientation::NONE) {
        for (auto& enemy : _enemies) {
            switch (enemy.dir) {
            case Orientation::UP:     enemy.pos.y -= SPACESHIP_SPEED / 2; break;
            case Orientation::DOWN:   enemy.pos.y += SPACESHIP_SPEED / 2; break;
            case Orientation::LEFT:   enemy.pos.x -= SPACESHIP_SPEED / 2; break;
            case Orientation::RIGHT:  enemy.pos.x += SPACESHIP_SPEED / 2; break;
            default: break;
            }
        }

        // Enemy shooting
        for (auto& enemy : _enemies) {
            enemy.timeSinceLastShot += _moveInterval;
            if (enemy.timeSinceLastShot >= _enemyFireRate) {
                Bullet eb;
                eb.pos = enemy.pos;
                eb.dir = enemy.shotDir;
                _enemyBullets.push_back(eb);

                std::uniform_real_distribution<float> distFireOffset(0.0f, _enemyFireRate / 2.0f);
                enemy.timeSinceLastShot = distFireOffset(_rng);
            }
        }
    }

    // Move enemy bullets
    for (auto& eb : _enemyBullets) {
        switch (eb.dir) {
            case Orientation::UP:    eb.pos.y -= _bulletSpeed; break;
            case Orientation::DOWN:  eb.pos.y += _bulletSpeed; break;
            case Orientation::LEFT:  eb.pos.x -= _bulletSpeed; break;
            case Orientation::RIGHT: eb.pos.x += _bulletSpeed; break;
            default: break;
        }
    }

    // Remove enemy bullets that are out of bounds
    _enemyBullets.erase(
        std::remove_if(_enemyBullets.begin(), _enemyBullets.end(), [](const Bullet& eb) {
            return eb.pos.x < START_X || eb.pos.x > END_X ||
                eb.pos.y < START_Y || eb.pos.y > END_Y;
        }),
        _enemyBullets.end()
    );


    if (_enemies[0].pos.x >= END_X) _enemies[0].dir = Orientation::LEFT;
    else if (_enemies[0].pos.x <= START_X + 30) _enemies[0].dir = Orientation::RIGHT;

    if (_enemies[1].pos.y >= END_Y) _enemies[1].dir = Orientation::UP;
    else if (_enemies[1].pos.y <= START_Y + 30) _enemies[1].dir = Orientation::DOWN;

    if (_enemies[2].pos.x >= END_X) _enemies[2].dir = Orientation::LEFT;
    else if (_enemies[2].pos.x <= START_X + 30) _enemies[2].dir = Orientation::RIGHT;

    if (_enemies[3].pos.y >= END_Y) _enemies[3].dir = Orientation::UP;
    else if (_enemies[3].pos.y <= START_Y + 30) _enemies[3].dir = Orientation::DOWN;

    // Player bullet range checker
    _bullets.erase(
    std::remove_if(_bullets.begin(), _bullets.end(), [](const Bullet& b) {
        return b.distanceTraveled >= 2 * CELL_SIZE;
    }),
    _bullets.end());

    // Keep spaceship within bounds (200,100) to (400,400)
    if (_shipPosition.x < START_X || _shipPosition.x > END_X ||
        _shipPosition.y < START_Y || _shipPosition.y > END_Y) {
        _state = GameState::GAME_OVER;
        return;
    }
}

void arcade::Solarfox::detectCollisions()
{
    for (auto& bullet : _bullets) {
        for (auto& powerup : _powerups) {
            if (powerup.collected) continue;

            // Simple collision check (bullet is a point, powerup is a 30x30 box)
            if (bullet.pos.x >= powerup.pos.x - 2 && bullet.pos.x <= powerup.pos.x + 30 &&
                bullet.pos.y >= powerup.pos.y - 2 && bullet.pos.y <= powerup.pos.y + 30) {
                powerup.collected = true;
                _score += 10;
            }
        }
    }

    // Ship vs enemy bullets
    for (const auto& eb : _enemyBullets) {
        const float SHIP_HALF = 25.0f;
        if (eb.pos.x >= _shipPosition.x - SHIP_HALF && eb.pos.x <= _shipPosition.x + SHIP_HALF &&
            eb.pos.y >= _shipPosition.y - SHIP_HALF && eb.pos.y <= _shipPosition.y + SHIP_HALF) {
            _state = GameState::GAME_OVER;
            return;
        }
    }

    // Player bullets vs enemy bullets
    for (auto& bullet : _bullets) {
        _enemyBullets.erase(
            std::remove_if(_enemyBullets.begin(), _enemyBullets.end(), [&](const Bullet& eb) {
                return std::abs(bullet.pos.x - eb.pos.x) <= 8.0f &&
                    std::abs(bullet.pos.y - eb.pos.y) <= 8.0f;
            }),
            _enemyBullets.end()
        );
    }

    // Next level checker
    bool allCollected = std::all_of(_powerups.begin(), _powerups.end(),
        [](const Powerup& p) { return p.collected; });
    if (allCollected && !_powerups.empty())
        nextLevel();
}

void arcade::Solarfox::nextLevel()
{
    _level++;
    _bullets.clear();
    _enemyBullets.clear();
    _shipChar = '>';
    _shipDirection = Orientation::NONE;
    _shipPosition = {static_cast<float>(_context.width / 2.0f),
                     static_cast<float>(_context.height / 2.0f)};
    _shipDirection = Orientation::NONE;

    _shipSpeed     = SPACESHIP_SPEED + (_level - 1) * 0.2f;
    _bulletSpeed   = ENEMY_BULLET_SPEED + (_level - 1) * 0.08f;
    _enemyFireRate = std::max(0.8f, ENEMY_FIRE_RATE - (_level - 1) * 0.15f);

    initPowerups();
    initEnemies();
}

void arcade::Solarfox::handleSpaceshipDirection(Key key)
{
    if (_state != GameState::RUNNING) return;

    if ((key == Key::UP || key == Key::Z) && _shipDirection != Orientation::DOWN) {
        _shipDirection = Orientation::UP;
        _shipChar = '^';
    } else if ((key == Key::DOWN || key == Key::S) && _shipDirection != Orientation::UP) {
        _shipDirection = Orientation::DOWN;
        _shipChar = 'v';
    } else if ((key == Key::LEFT || key == Key::Q) && _shipDirection != Orientation::RIGHT && _shipDirection != Orientation::NONE) {
        _shipDirection = Orientation::LEFT;
        _shipChar = '<';
    } else if ((key == Key::RIGHT || key == Key::D) && _shipDirection != Orientation::LEFT) {
        _shipDirection = Orientation::RIGHT;
        _shipChar = '>';
    }
}

void arcade::Solarfox::createNewBullet()
{
    Bullet newBullet;
    newBullet.pos = _shipPosition;
    newBullet.dir = _shipDirection;
    _bullets.push_back(newBullet);
}

void arcade::Solarfox::handleInput(Key key)
{
    if (key == Key::P) {
        if (_state == GameState::RUNNING)
            pause();
        else if (_state == GameState::PAUSED)
            resume();
        return;
    } else if (key == Key::SPACE)
        createNewBullet();
    else if (key == Key::C)
        _shipSpeed += 1.0f;

    handleSpaceshipDirection(key);
}

void arcade::Solarfox::handleInput(MouseEvent mouseEvent)
{
    if (mouseEvent.button == Key::MOUSE_LEFT)
        createNewBullet();
}

void arcade::Solarfox::initPowerups()
{
    _powerups.clear();

    std::vector<Position> cells;
    for (int y = START_Y; y < END_Y; y += CELL_SIZE)
        for (int x = START_X; x < END_X; x += CELL_SIZE)
            cells.push_back({static_cast<float>(x), static_cast<float>(y)});

    std::shuffle(cells.begin(), cells.end(), _rng);

    int count = std::min(5 + (_level - 1) * 3, static_cast<int>(cells.size()));
    for (int i = 0; i < count; i++)
        _powerups.push_back({cells[i], false});
}

void arcade::Solarfox::initEnemies()
{
    std::uniform_real_distribution<float> distX(START_X + 30.0f, END_X - 30.0f);
    std::uniform_real_distribution<float> distY(START_Y + 30.0f, END_Y - 30.0f);
    std::uniform_real_distribution<float> distFire(0.0f, _enemyFireRate);

    _enemies[0] = {{distX(_rng), START_Y}, Orientation::RIGHT, Orientation::DOWN,  distFire(_rng), getOrientedSprite("enemy", Orientation::DOWN), Anchor::BOTTOM_RIGHT};
    _enemies[1] = {{END_X, distY(_rng)},   Orientation::DOWN,  Orientation::LEFT,  distFire(_rng), getOrientedSprite("enemy", Orientation::LEFT), Anchor::BOTTOM_LEFT};
    _enemies[2] = {{distX(_rng), END_Y},   Orientation::RIGHT, Orientation::UP,    distFire(_rng), getOrientedSprite("enemy", Orientation::UP), Anchor::TOP_RIGHT};
    _enemies[3] = {{START_X, distY(_rng)}, Orientation::DOWN,  Orientation::RIGHT, distFire(_rng), getOrientedSprite("enemy", Orientation::RIGHT), Anchor::BOTTOM_RIGHT};
}

void arcade::Solarfox::restart()
{
    int midX = _context.width / 2;
    int midY = _context.height / 2;

    _state = GameState::RUNNING;
    _score = 0;

    _shipChar = '>';
    _shipPosition = {static_cast<float>(midX), static_cast<float>(midY)};
    _shipDirection = Orientation::NONE;
    _bullets.clear();
    _powerups.clear();
    _enemyBullets.clear();
    _level = 1;
    _enemyFireRate = ENEMY_FIRE_RATE;
    _rng = std::mt19937(std::random_device{}());

    _timeSinceLastMove = static_cast<float>(0.0);
    _shipSpeed   = SPACESHIP_SPEED;
    _bulletSpeed = ENEMY_BULLET_SPEED;
    _moveInterval = MOVE_INTERVAL;
    initPowerups();
    initEnemies();
}

extern "C" {
    arcade::IModule* entryPoint() {
        return new arcade::Solarfox();
    }
}
