/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** arcade_menu
*/

/// @file ArcadeMenu.hpp
/// @brief Declaration of the ArcadeMenu class, which provides a menu interface for selecting
/// games and graphics libraries in the arcade application.

#ifndef ARCADE_MENU_HPP
#define ARCADE_MENU_HPP

#include "AGameModule.hpp"
#include "IMenuModule.hpp"

namespace arcade {
    /// @brief The ArcadeMenu class implements the IMenuModule interface and provides a menu for
    /// selecting games and graphics libraries.
    ///
    /// @implements IMenuModule
    /// @implements AGameModule
    ///
    /// @note The menu allows users to navigate through available options and make selections using
    /// keyboard input.
    class ArcadeMenu : public IMenuModule, public AGameModule {
        public:
        /// @brief Constructs the ArcadeMenu with default settings and initializes the menu state.
        ArcadeMenu();
        ~ArcadeMenu() = default;

        // Setters

        /// @brief Sets the paths for the available graphic libraries.
        /// @param paths The vector of graphic library paths.
        void setDisplayPaths(const std::vector<std::string>& paths) override;
        /// @brief Sets the paths for the available game libraries.
        /// @param paths The vector of game library paths.
        void setGamePaths(const std::vector<std::string>& paths) override;
        /// @brief Set the index of the currently loaded graphic library.
        /// @param idx index of the display library currently loaded.
        void setDisplayIdx(const int idx) override;
        /// @brief Sets the player's name entered in the menu.
        /// @param name The player's name to set.
        void setPlayerName(const std::string& name) override;

        /// @brief Restarts the menu, resetting selections and state.
        /// @note This function resets the menu to its initial state, clearing any user selections.
        void restart() override;
        /// @brief Handles keyboard input to navigate the menu and make selections.
        /// @param key The key that was pressed by the user.
        /// @note This function processes arrow keys for navigation and left/right keys for
        /// switching between graphic and game selection.
        void handleInput(Key key) override;
        /// @brief Handles mouse input.
        /// @param mouseEvent The mouse event that occurred.
        void handleInput(MouseEvent mouseEvent) override;
        /// @brief Updates the menu state.
        /// @param elapsedTime The time elapsed since the last update.
        void update(float elapsedTime) override;

        // Getters

        /// @brief Gets the entities to be rendered for the menu.
        /// @return A constant reference to a vector of entities representing the menu visuals.
        const std::vector<Entity>& getEntities() const override;
        /// @brief Gets the UI text elements to be rendered for the menu.
        /// @return A constant reference to a vector of text elements representing the menu UI.
        const std::vector<Text>& getUIText() const override;
        /// @brief Gets the currently selected game.
        /// @return A constant reference to the selected game's name.
        const std::string& getSelectedGame() const override;
        /// @brief Gets the currently selected graphic library.
        /// @return A constant reference to the selected graphic library's name.
        const std::string& getSelectedGraphic() const override;
        /// @brief Gets the player's name entered in the menu.
        /// @return A constant reference to the player's name.
        const std::string& getPlayerName() const override;

        /// @brief Gets the top 5 players and their scores from the saves.csv file.
        /// @param gameName The name of the game to filter by.
        /// @return A vector of pairs containing the player's name and their score.
        std::vector<std::pair<std::string, int>> getTopScores(const std::string& gameName = "") const;

        private:

        /// @brief Initializes the oriented sprites for the menu.
        void setMenuOrientedSprites();

        /// @brief Checks if the window has been resized and sets a flag to update the menu visuals if needed.
        void isResized();

        /// @brief Current menu section being navigated (graphics or games)
        enum class CurrentChoice { ONGRAPHIC, ONGAME };

        /// @brief Handles the up key input to navigate upwards in the menu.
        void leftKey();
        /// @brief Handles the down key input to navigate downwards in the menu.
        void rightKey();

        /// @brief Handles left/right key input to switch between graphic and game selection
        /// sections.
        /// @param key The key that was pressed (LEFT or RIGHT)
        void switchChoice(const Key& key);

        /// @brief Validates the current selection and updates the menu state accordingly.
        void validateSelection();

        /// @brief A vector of file paths for available display modules.
        std::vector<std::string> _displayPaths;
        /// @brief A vector of file paths for available game modules.
        std::vector<std::string> _gamePaths;

        /// @brief The current section of the menu that the user is navigating (graphics or games).
        CurrentChoice _currentSection;
        /// @brief The section of the menu that the user has validated (graphics or games).
        CurrentChoice _validatedSection;
        /// @brief Flag indicating whether the user has made a selection in the menu.
        bool _hasValidatedSelection;

        /// @brief The index of the currently selected graphic library in the menu.
        int _displayIdx;
        /// @brief The index of the currently selected game library in the menu.
        int _gameIdx;
        /// @brief The index of the currently loaded graphic library.
        int _currentDisplayIdx;

        /// @brief Flag to ensure we don't update uselessly unless input or window changes
        bool _needsUpdate;

        /// @brief The player's name entered in the menu.
        std::string _playerName;

        /// @brief Flag indicating whether the player is currently entering their name in the menu.
        bool _isInPlayerBox;

        /// @brief Handles input for entering the player's name in the menu.
        /// @param key The key that was pressed by the user while entering their name.
        void handlePlayerNameInput(Key key);

        /// @brief Creates a save file for the player
        void createPlayerSave();

        /// @brief draw the title of the menu at the top of the screen
        /// @param midX The x-coordinate of the middle of the screen, used for centering the title
        void drawTitle(float midX);

        /// @brief draw the sections for graphics and game selection in the menu
        /// @param choiceX The x-coordinate of the graphic bar separating the sections
        /// @param graphicY The y-coordinate of the graphic column separating the sections
        void drawGraphicsSection(float choiceX, float graphicY);

        /// @brief draw the sections for graphics and game selection in the menu
        /// @param choiceX The x-coordinate of the game bar separating the sections
        /// @param gameY The y-coordinate of the game column separating the sections
        void drawGamesSection(float choiceX, float gameY);

        /// @brief draw the box for entering the player's name in the menu
        /// @param midX The x-coordinate of the middle of the screen, used for centering the name box
        void drawPlayerNameBox(float midX);

        /// @brief draw the scoreboard with the top 5 players and their scores
        /// @param boardX The x-coordinate of the left edge of the scoreboard
        /// @param boardY The y-coordinate of the top edge of the scoreboard
        void drawScoreBoard(float boardX, float boardY);
    };
}  // namespace arcade

#endif /* !ARCADE_MENU_HPP */
