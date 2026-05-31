/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** ADisplayModule
*/

/// @file ADisplayModule.hpp
/// @brief Abstract base class for display modules in the Arcade project

#pragma once

#include <chrono>
#include <queue>
#include <string>

#include "IDisplayModule.hpp"

namespace arcade {

    /// @brief Abstract base class for display modules
    ///
    /// Provides common functionality and state management for all display modules.
    class ADisplayModule : public IDisplayModule {
        public:
        /// @brief Constructor for the ADisplayModule class
        /// @param name The name of the display module (default is "Unnamed Display")
        /// @param context The context of the current display module (default is 800x600 in PIXEL mode)
        ADisplayModule(std::string name = "Unnamed Display", RenderContext context = {800, 600, CoordinateSpace::PIXEL});
        virtual ~ADisplayModule() = default;

        /// @brief Struct containing information about the viewport for rendering
        struct Viewport {
            float scale;
            int offsetX;
            int offsetY;
        };

        // --- Getters ---

        /// @brief Gets the type of the display module
        /// @return The module type
        ModuleType getType() const override;

        /// @brief Gets the name of the display module
        /// @return The module name
        const std::string& getName() const override;

        /// @brief Gets the last key event
        /// @return The last key event
        Key getKey() override;

        /// @brief Gets the last mouse event
        /// @return The last mouse event
        MouseEvent getMouseEvent() override;

        /// @brief Gets the delta time
        /// @return The delta time
        float getDeltaTime() const override;

        /// @brief Gets the width of the display module
        /// @return The width
        int getWidth() const override;

        /// @brief Gets the height of the display module
        /// @return The height
        int getHeight() const override;

        // --- Setters ---

        /// @brief Sets the title of the display module
        /// @param title The new title
        void setTitle(const std::string& title) override;

        /// @brief Sets the size of the display module window
        /// @param width The new width
        /// @param height The new height
        void setWindowSize(int width, int height) override;

        /// @brief Sets the fullscreen mode of the display module
        /// @param fullscreen The new fullscreen mode
        void setFullscreen(bool fullscreen) override;

        /// @brief Sets the render context of the display module
        /// @param context The new render context
        void setRenderContext(const RenderContext& context) override;

        protected:
        /// @brief Check if the specified file has a valid format
        /// @param path The path to the file
        /// @return True if the format is valid, false otherwise
        bool isValidFormat(const std::string& path);

        /// @brief Compute the viewport for rendering based on the current window size and render context
        /// @return The computed viewport containing scale and offsets for rendering
        virtual Viewport computeViewport() const = 0;

        /// @brief Update the delta time, meant to be called once per frame (e.g. in pollEvents)
        void updateDeltaTime();

        /// @brief Apply the specified anchor to the given coordinates based on the entity's dimensions
        void applyAnchor(int& x, int& y, int width, int height, Anchor anchor) const;

        /// @brief Apply scaling to the given coordinates and dimensions based on the computed viewport
        void applyScaling(int& x, int& y, int& width, int& height) const;

        /// @brief The name of the display module
        std::string _name;

        /// @brief The title of the display window
        std::string _title;

        /// @brief The render context containing dimensions and coordinate space
        RenderContext _context;

        /// @brief Whether the display is in fullscreen mode
        bool _fullScreen;

        /// @brief The calculated delta time for the current frame
        float _deltaTime;

        /// @brief The last recorded time for delta time calculation
        std::chrono::high_resolution_clock::time_point _lastTime;

        /// @brief Queue to store key events
        std::queue<Key> _keyEvent;

        /// @brief Queue to store mouse events
        std::queue<MouseEvent> _mouseEvent;
    };
}  // namespace arcade
