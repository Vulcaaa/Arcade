/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** SnakeGame
*/

/// @file SnakeGame.hpp
/// @brief Implementation of the Snake game logic

#pragma once

#include <deque>

#include "AGameModule.hpp"

namespace arcade {
    /// @brief Snake game implementation
    class SnakeGame : public AGameModule {
        public:
        /// @brief Constructor for the SnakeGame class
        SnakeGame();

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
        /// @brief Time since the last snake movement, used to control movement speed
        float _timeSinceLastMove;
        /// @brief Interval between snake movements (in seconds)
        float _moveInterval;

        /// @brief Current direction of the snake's movement
        Orientation _currentDir;
        /// @brief Next direction to turn the snake, set by input and applied on the next move
        Orientation _nextDir;

        /// @brief Deque representing the snake's body segments, with the head at the front
        std::deque<Position> _snakeBody;
        /// @brief Vector of fruit positions currently on the board
        std::vector<Position> _fruits;

        /// @brief Move the snake in the current direction, handle collisions and fruit consumption
        void moveSnake();

        /// @brief Spawn a fruit at a random position not occupied
        void spawnFruit();

        /// @brief Update the snake body and head entities
        void updateSnakeEntities();

        /// @brief Update the fruit entities
        void updateFruitEntities();

        /// @brief Update the wall entities
        void updateWallEntities();

        /// @brief Update the floor entities
        void updateFloorEntities();

        /// @brief Set the oriented sprites for snake
        void setSnakeOrientedSprites();
    };
}  // namespace arcade
