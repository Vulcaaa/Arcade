/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** SolarfoxGame
*/

/// @file SolarfoxGame.hpp
/// @brief Implementation of the Solarfox game logic

#pragma once

#include "AGameModule.hpp"
#include "IDisplayModule.hpp"
#include <array>
#include <vector>
#include <random>

namespace arcade {
    /// @brief Solarfox game implementation
    class Solarfox : public AGameModule {
        public:
        /// @brief Constructor for the Solarfox class
        Solarfox();

        /// @brief Update the game state
        /// @param elapsedTime Time elapsed since last update
        void update(float elapsedTime) override;

        /// @brief Handle input events
        /// @param key The key that was pressed
        void handleInput(Key key) override;

        /// @brief Handle mouse input
        /// @param mouseEvent The mouse event
        void handleInput(MouseEvent mouseEvent) override;

        /// @brief Restart the game
        void restart() override;

        private:

        /// @brief Struct representing a bullet fired by the player or enemies
        struct Bullet {
            /// @brief Position of the bullet on the screen
            Position pos;
            /// @brief Direction the bullet is moving (UP, DOWN, LEFT, RIGHT)
            Orientation dir;
            /// @brief Distance traveled by the bullet
            float distanceTraveled;
        };

        /// @brief Struct representing a powerup that can be collected by the player
        struct Powerup {
            /// @brief Position of the powerup on the screen
            Position pos;
            /// @brief If the powerup has been collected by the player
            bool collected;
        };

        /// @brief Struct representing an enemy ship in the game
        struct Enemy {
            /// @brief Position of the enemy on the screen
            Position pos;
            /// @brief Current direction of the enemy's movement
            Orientation dir;
            /// @brief Direction the enemy is shooting
            Orientation shotDir;
            /// @brief Time since the enemy last fired a bullet
            float timeSinceLastShot;
            /// @brief Sprite path for the enemy's visual representation
            std::string spritePath;
            /// @brief Anchor point for rendering the enemy sprite
            Anchor anchor;
        };

        /// @brief Update the position of entities
        void updatePosition();
        /// @brief Handle the direction of the spaceship
        /// @param key The key that was pressed
        void handleSpaceshipDirection(Key key);
        /// @brief Initialize the powerups for the level
        void initPowerups();
        /// @brief Initialize the enemies for the level
        void initEnemies();
        /// @brief Detect collisions between entities
        void detectCollisions();
        /// @brief Progress to the next level
        void nextLevel();
        /// @brief Set the oriented sprites for the solarfox
        void setSolarfoxOrientedSprites();
        /// @brief Create a new bullet based on the spaceship's current position and direction
        void createNewBullet();

        /// @brief The main spaceship entity
        Entity _ship;
        /// @brief Character representing the ship
        char _shipChar;
        /// @brief Position of the ship
        Position _shipPosition;
        /// @brief Direction the ship is facing
        Orientation _shipDirection;
        /// @brief Speed of the ship
        float _shipSpeed;

        /// @brief Current level
        int _level;
        /// @brief Bullets fired by the player
        std::vector<Bullet> _bullets;
        /// @brief Speed of the bullets
        float _bulletSpeed;
        /// @brief Powerups scattered in the map
        std::vector<Powerup> _powerups;
        /// @brief Array of 4 enemies guarding the edges
        std::array<Enemy, 4> _enemies;
        /// @brief Bullets fired by enemies
        std::vector<Bullet> _enemyBullets;
        /// @brief Rate of fire for enemies
        float _enemyFireRate;
        /// @brief Random number generator
        std::mt19937 _rng;

        /// @brief Time elapsed since the last move
        float _timeSinceLastMove;
        /// @brief Interval between moves
        float _moveInterval;
    };
}  // namespace arcade
