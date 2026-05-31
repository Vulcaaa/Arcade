/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** IGameModule
*/

/// @file IGameModule.hpp
/// @brief Interface for game libraries in the Arcade project

#pragma once

#include <string>
#include <vector>

#include "IDisplayModule.hpp"
#include "IModule.hpp"

namespace arcade {

    /// @enum GameState
    /// @brief Current state of the game
    enum class GameState {
        MENU,       ///< Game is in menu/not started
        RUNNING,    ///< Game is currently running
        PAUSED,     ///< Game is paused
        GAME_OVER,  ///< Game has ended (loss condition)
        WON         ///< Game has ended (win condition)
    };

    /// @class IGameModule
    /// @brief Interface for game libraries
    ///
    /// This interface must be implemented by all game libraries (Snake, Pacman, etc.)
    /// The core program will use this interface to run the game logic without knowing
    /// which specific game is being played.
    ///
    /// Game libraries must NOT contain any rendering code or reference to specific
    /// graphics libraries. They should only manage game state and provide entities
    /// to be rendered by the display module.
    class IGameModule : public IModule {
        public:
        /// @brief Virtual destructor
        virtual ~IGameModule() = default;

        /// @brief Initialize the game
        ///
        /// Sets up the initial game state, loads resources, and prepares for gameplay.
        /// This method should be called once before starting the game.
        ///
        /// @throw ModuleException if initialization fails
        virtual void init() = 0;

        /// @brief Stop and cleanup the game
        ///
        /// Releases all game resources and resets state.
        /// This method should be called once when exiting the game.
        virtual void stop() = 0;

        /// @brief Get the name of this game
        /// @return Name of the game (e.g., "Snake", "Pacman", "Nibbler")
        virtual const std::string& getName() const = 0;

        /// @brief Update game logic
        ///
        /// This method is called once per frame to update the game state.
        /// It should process physics, AI, collisions, and any time-based events.
        ///
        /// @param deltaTime Time elapsed since last update (in seconds)
        virtual void update(float deltaTime) = 0;

        /// @brief Handle keyboard input
        ///
        /// Process a key event to update game state accordingly.
        /// The core program will call this method when a key is pressed.
        ///
        /// @param key The key that was pressed
        virtual void handleInput(Key key) = 0;

        /// @brief Handle mouse input
        ///
        /// Process a mouse event to update game state accordingly.
        /// The core program will call this method when a mouse event occurs.
        ///
        /// @param mouseEvent The mouse event data (position and button)
        virtual void handleInput(MouseEvent mouseEvent) = 0;

        /// @brief Get all entities to be rendered
        ///
        /// Returns a reference to all game entities (player, enemies, items, tiles, etc.)
        /// that should be displayed on screen. The display module will iterate over
        /// this list and render each entity.
        ///
        /// @return Const reference to vector of entities to render
        virtual const std::vector<Entity>& getEntities() const = 0;

        /// @brief Get UI text elements to be rendered
        ///
        /// Returns text elements for score, lives, messages, etc.
        /// These are rendered separately from entities to ensure proper layering.
        ///
        /// @return Const reference to vector of text elements to render
        virtual const std::vector<Text>& getUIText() const = 0;

        /// @brief Get the current game state
        /// @return Current state (menu, running, paused, game over, won)
        virtual GameState getState() const = 0;

        /// @brief Get the current score
        /// @return Player's current score
        virtual int getScore() const = 0;

        /// @brief Restart the game
        ///
        /// Resets the game to its initial state, clearing score and repositioning entities.
        virtual void restart() = 0;

        /// @brief Pause the game
        ///
        /// Pauses game logic updates. Can be resumed with resume().
        virtual void pause() = 0;

        /// @brief Resume the game
        ///
        /// Resumes game logic updates after being paused.
        virtual void resume() = 0;

        /// @brief Get the render context for this game
        ///
        /// Defines the logical rendering space and coordinate system used by
        /// entities and text returned by the game.
        ///
        /// @return Logical render context
        virtual RenderContext getRenderContext() const = 0;
    };
}  // namespace arcade
