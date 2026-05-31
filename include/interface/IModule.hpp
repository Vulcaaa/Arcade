/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** IModule
*/

/// @file IModule.hpp
/// @brief Base interface for all Arcade modules (games and display libraries)
///
/// @section dynamic_loading Dynamic Loading Convention
/// Each .so library MUST export the following C-style entry point function:
/// @code
/// extern "C" arcade::IModule* entryPoint() {
///     return new MyModuleImplementation();
/// }
/// @endcode
///
/// The core program will call this function to instantiate the module.
#pragma once

#include <exception>
#include <string>

namespace arcade {

    /// @enum ModuleType
    /// @brief Identifies the type of module
    enum class ModuleType {
        DISPLAY,  ///< Graphics/display library module
        GAME      ///< Game logic module
    };

    /// @class ModuleException
    /// @brief Exception class for module errors
    ///
    /// This exception can be thrown by any module during initialization,
    /// resource loading, or runtime errors.
    class ModuleException : public std::exception {
        public:
        /// @brief Construct exception with error message
        /// @param message Description of the error
        explicit ModuleException(const std::string& message) : _message(message) {}

        /// @brief Get the error message
        /// @return C-string containing the error description
        const char* what() const noexcept override { return _message.c_str(); }

        private:
        std::string _message;  ///< Error message
    };

    /// @class IModule
    /// @brief Base interface for all Arcade modules
    ///
    /// This interface provides common functionality for both game and display modules.
    /// All modules must implement getType() to identify themselves at runtime.
    class IModule {
        public:
        /// @brief Virtual destructor
        virtual ~IModule() = default;

        /// @brief Get the type of this module
        /// @return Whether this is a DISPLAY or GAME module
        virtual ModuleType getType() const = 0;

        /// @brief Get the name of this module
        /// @return Human-readable name (e.g., "SDL2", "Snake", "nCurses")
        virtual const std::string& getName() const = 0;
    };
}  // namespace arcade
