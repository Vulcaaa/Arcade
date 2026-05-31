/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** IMenuModule
*/

/// @file IMenuModule.hpp
/// @brief An interface for a class that manages the menu of the arcade

#ifndef IMENU_MODULE_HPP
#define IMENU_MODULE_HPP

#include <string>
#include <vector>

namespace arcade {
    /// @brief An interface for a class that manages the menu of the arcade
    class IMenuModule {
        public:
            virtual ~IMenuModule() = default;

            /// @brief Sets the paths of the available display libraries
            /// @param paths The paths of the available display libraries
            virtual void setDisplayPaths(const std::vector<std::string>& paths) = 0;
            /// @brief Sets the paths of the available game libraries
            /// @param paths The paths of the available game libraries
            virtual void setGamePaths(const std::vector<std::string>& paths) = 0;

            /// @brief Set the index of the currently loaded graphic library.
            /// @param idx index of the display library currently loaded.
            virtual void setDisplayIdx(const int idx) = 0;

            /// @brief Gets the path of the selected game library
            /// @return The path of the selected game library
            virtual const std::string& getSelectedGame() const = 0;
            /// @brief Gets the path of the selected graphic library
            /// @return The path of the selected graphic library
            virtual const std::string& getSelectedGraphic() const = 0;

            /// @brief Sets the player's name entered in the menu.
            /// @param name The player's name to set.
            virtual void setPlayerName(const std::string& name) = 0;
            /// @brief Gets the player's name entered in the menu.
            /// @return A constant reference to the player's name.
            virtual const std::string& getPlayerName() const = 0;
    };
}

#endif /* !IMENU_MODULE_HPP */
