/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** DLLoader
*/

/// @file DLLoader.hpp
/// @brief A class for loading and managing shared libraries

#include <dlfcn.h>

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "interface/IDLLoader.hpp"

#ifndef DLLOADER_HPP
#define DLLOADER_HPP

namespace arcade {

    /// @brief A class for loading and managing shared libraries
    /// @tparam T The type of the module to load
    template <typename T>
    class DLLoader : public IDLLoader<T> {
        public:
        /// @brief Constructor that loads the shared library at the given filepath
        /// @param filepath The path to the shared library to load
        DLLoader(const std::string& filepath)
            : _handle(dlopen(filepath.c_str(), RTLD_LAZY | RTLD_NODELETE), [](void* handle) {
                  if (handle) dlclose(handle);
              })
        {
            if (!_handle) throw DLLoaderException(dlerror());
        }

        /// @brief Get an instance of the loaded module
        /// @param entryPointName The name of the entry point function to call (default is
        /// "entryPoint")
        /// @return A pointer to the instance of the loaded module, or nullptr if there was an error
        T* getInstance(const std::string& entryPointName = "entryPoint") override
        {
            if (!_handle) return nullptr;

            typedef T* (*EntryPointFunc)();

            EntryPointFunc createModule =
                (EntryPointFunc)dlsym(_handle.get(), entryPointName.c_str());

            if (createModule == nullptr) throw DLLoaderException(dlerror());

            return createModule();
        }

        /// @brief Exception class for DLLoader errors
        class DLLoaderException : public std::exception {
            public:
            /// @brief Constructor that takes an error message
            /// @param message The error message describing the exception
            DLLoaderException(const std::string& message) : _message(message) {}
            /// @brief Override of the what() function to return the error message
            /// @return The error message as a C-style string
            const char* what() const noexcept override { return _message.c_str(); }

            private:
            /// @brief The error message describing the exception
            std::string _message;
        };

        private:
        /// @brief A unique pointer that manages the handle to the loaded shared library, ensuring
        /// it is properly closed when the DLLoader is destroyed
        std::unique_ptr<void, void (*)(void*)> _handle;
    };
}  // namespace arcade

#endif /* !DLLOADER_HPP */
