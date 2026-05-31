/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** Arcade
*/

/// @file Arcade.hpp
/// @brief Declaration of the Arcade class, which manages the main game loop, module loading, and
/// state transitions for the arcade application.

#ifndef ARCADE_HPP
#define ARCADE_HPP

#include <chrono>

#include "DLLoader.hpp"
#include "IDisplayModule.hpp"
#include "IGameModule.hpp"

namespace arcade {
    /// @brief The Arcade class is responsible for managing the main game loop, loading game and
    /// display modules, handling user input, and managing state transitions between the menu and
    /// games. It uses dynamic loading to support multiple game and display modules at runtime.
    class Arcade {
        public:
        /// @brief Constructs the Arcade application, loads the initial graphic library, and starts
        /// the menu
        /// @param ac The argument count (should be 2)
        /// @param av The argument vector (should contain the path to the initial graphic library)
        /// @throws std::invalid_argument if the number of arguments is incorrect or if the graphic
        /// library fails to load
        Arcade(int ac, char* av[]);
        ~Arcade() = default;

        /// @brief Runs the main game loop
        /// @return The exit status of the program
        int run();

        private:
        /// @brief Prints the usage of the program
        /// @param programName The name of the program (typically argv[0])
        void printUsage(const std::string& programName) const;
        /// @brief Loads the libraries from the specified path
        /// @param loadPath The path from which to load the libraries
        /// @param graphicPath The path of the initially loaded graphic library
        void loadLibrairies(const std::string& loadPath, const std::string& graphicPath);

        /// @brief Processes the inputs from the user
        /// @param key The key that was pressed
        /// @return True if the input was processed, false otherwise
        bool inputsManager();

        /// @brief Key processing function.
        bool processKeys();

        /// @brief Mouse events processing function.
        bool processMouseEvents();

        /// @brief Loads a game module from the specified path and starts it
        /// @param selectedGame The path of the selected game module
        void loadGameFromPath(const std::string& selectedGame);
        /// @brief Loads a display module from the specified path and switches to it
        /// @param selectedGraphic The path of the selected display module
        void loadDisplayFromPath(const std::string& selectedGraphic);

        /// @brief Handles the menu selection and transitions to the selected game or graphic
        /// library
        void handleMenuSelection();
        /// @brief Start the arcade menu
        void menuManager();
        /// @brief Handles state transitions based on the current game state and user input
        /// @param key The key that was pressed
        void handleStateTransitions();

        /// @brief Renders the current frame by drawing all entities and UI text
        void renderFrame();
        /// @brief Cleans up resources by stopping and deleting the game and display instances
        void cleanupResources();

        /// @brief Sets the display index based on the initially loaded graphic library
        void setDisplayIdx(const std::string& initGraphicPath);

        /// @brief Manages the switching of keys based on the current game state
        /// @param key The key detected by a DisplayModule
        void moduleSwitcher(Key key);

        /// @brief Saves the current session info to a csv file
        void saveScoreToFile();

        /// @brief A unique pointer that manages the loading of the display module, ensuring proper
        /// cleanup when the Arcade instance is destroyed
        std::unique_ptr<DLLoader<IDisplayModule>> _graphicLoader;
        /// @brief A unique pointer that manages the loading of the game module, ensuring proper
        /// cleanup when the Arcade instance is destroyed
        std::unique_ptr<DLLoader<IGameModule>> _gameLoader;

        /// @brief A pointer to the currently active display module instance
        IDisplayModule* _graphicInstance;
        /// @brief A pointer to the currently active game module instance
        IGameModule* _gameInstance;

        /// @brief Vectors that store the file paths of available display modules
        std::vector<std::string> _displayPaths;
        /// @brief Vector that stores the file paths of available game modules
        std::vector<std::string> _gamePaths;
        /// @brief The file path of the menu module
        std::string _menuPath;

        /// @brief The index of the currently selected display module in the menu
        int _displayIdx;
        /// @brief The index of the currently selected game module in the menu
        int _gameIdx;

        /// @brief The current state of the game (menu, running, paused, game over, won)
        GameState _gameState;
        /// @brief The player's name
        std::string _playerName;
        /// @brief Flag indicating whether the arcade application is currently running
        bool _isRunning;
    };
}  // namespace arcade

#endif /* !ARCADE_HPP */
