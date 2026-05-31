/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** AGameModule
*/

/// @file AGameModule.hpp
/// @brief Abstract base class for game modules

#pragma once

#include "IGameModule.hpp"
#include <map>
#include <string>
#include <utility>

namespace arcade {

    /// @enum Orientation
    /// @brief Defines orientations for dynamic sprites
    enum class Orientation { UP, DOWN, LEFT, RIGHT, NONE };

    /// @class AGameModule
    /// @brief Abstract implementation of IGameModule that provides getType()
    ///
    /// This class implements the getType() method returning ModuleType::GAME.
    /// All concrete game modules (Snake, Pacman, etc.) should inherit from
    /// this class instead of IGameModule directly.
    class AGameModule : public IGameModule {
        public:
        /// @brief Virtual destructor
        AGameModule(std::string name = "Unnamed Game", GameState initialState = GameState::MENU, RenderContext context = RenderContext(800, 600, CoordinateSpace::PIXEL));
        virtual ~AGameModule() = default;

        /// @brief get all entities to be rendered
        /// @return Const reference to vector of entities to render
        const std::vector<Entity>& getEntities() const override;

        /// @brief get all UI text elements to be rendered
        /// @return Const reference to vector of text elements to render
        const std::vector<Text>& getUIText() const override;

        /// @brief Get the render context of the current game (window size, space coordinate)
        /// @return RenderContext containing rendering parameters for this game
        RenderContext getRenderContext() const override;

        /// @brief Get the type of this module
        /// @return ModuleType::GAME
        ModuleType getType() const override;

        /// @brief Get the name of this game
        /// @return Name of the game (e.g., "Snake", "Pacman", "Nibbler")
        const std::string& getName() const override;

        /// @brief Get the current game state
        /// @return Current state (menu, running, paused, game over, won)
        GameState getState() const override;

        /// @brief Get the current score
        /// @return Player's current score
        int getScore() const override;

        /// @brief Stop the game and clean up resources
        void stop() override;

        /// @brief Pause the game
        void pause() override;

        /// @brief Resume the game
        void resume() override;

        /// @brief Initialize the game state
        void init() override;

        protected:
        /// @brief Append common UI texts (controls) to the given vector
        void appendCommonUITexts();

        /// @brief Register a sprite for a specific orientation
        /// @param key The base key of the entity (e.g., "head")
        /// @param orientation The orientation (e.g., UP, DOWN)
        /// @param spritePath The path to the specific sprite
        void setOrientedSprite(const std::string& key, Orientation orientation, const std::string& spritePath);

        /// @brief Get the registered sprite for a specific orientation
        /// @param key The base key of the entity
        /// @param orientation The orientation
        /// @return The path to the correctly oriented sprite, or empty if not found
        std::string getOrientedSprite(const std::string& key, Orientation orientation) const;

        /// @brief Map storing oriented sprites
        std::map<std::pair<std::string, Orientation>, std::string> _orientedSprites;

        /// @brief A vector of entities to be rendered for the menu visuals.
        std::vector<Entity> _entities;
        /// @brief A vector of text elements to be rendered for the menu UI.
        std::vector<Text> _uiText;

        /// @brief The name of the game module
        std::string _name;
        /// @brief The current state of the game (menu, running, paused, game over, won)
        GameState _state;

        /// @brief The rendering context for this game (window size, coordinate space)
        RenderContext _context;

        /// @brief The player's current score
        int _score;

        /// @brief Time tracking for game updates
        float _timeSinceLastMove;

        /// @brief Interval between game updates (e.g., snake movement speed)
        float _moveInterval;
    };
}  // namespace arcade
