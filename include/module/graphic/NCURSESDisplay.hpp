/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** NCURSESDisplay
*/

/// @file NCURSESDisplay.hpp
/// @brief NCURSES implementation of the IDisplayModule interface

#pragma once

#include <ncurses.h>

#include <map>
#include <utility>

#include "ADisplayModule.hpp"

namespace arcade {
    /// @brief NCURSES implementation of the IDisplayModule interface
    class NCURSESDisplay : public ADisplayModule {
        public:
        NCURSESDisplay();
        ~NCURSESDisplay();

        /// @brief Initialize the NCURSES display module
        /// @throw std::runtime_error if initialization fails
        void init() override;

        /// @brief Stop and cleanup the NCURSES display module
        void stop() override;

        /// @brief Clear the screen
        void clear() override;

        /// @brief Display the current frame
        void display() override;

        /// @brief Draw an entity on the screen
        /// @param entity The entity to draw
        void drawEntity(const Entity& entity) override;

        /// @brief Draw text on the screen
        /// @param text The text to draw
        void drawText(const Text& text) override;

        /// @brief Draw a rectangle on the screen
        /// @param rect The rectangle to draw
        /// @param color The color of the rectangle
        void drawRectangle(const Rect& rect, const Color& color) override;

        /// @brief Poll for input events
        void pollEvents() override;

        /// @brief Checks if the display is open
        /// @return True if the display is open, false otherwise
        bool isOpen() const override;

        private:
        /// @brief Compute the viewport for rendering based on the current window size and render context
        /// @return The computed viewport containing scale and offsets for rendering
        Viewport computeViewport() const override;

        /// @brief Handle mouse click events and convert them to arcade::MouseEvent
        void mouseClickHandler();

        /// @brief Convert a generic arcade::Color into the closest NCURSES color index
        short getClosestNcursesColor(const Color& color) const;

        /// @brief Get or create an NCURSES color pair for the given foreground and background
        short getColorPair(const Color& fg, const Color& bg);

        /// @brief The main NCURSES window (usually stdscr)
        WINDOW* _win;

        /// @brief Cache for NCURSES color pairs to avoid redundant init_pair calls.
        /// The key is a pair of ncurses color IDs: (foreground, background).
        /// The value is the allocated ncurses color pair ID.
        std::map<std::pair<short, short>, short> _colorPairs;

        /// @brief Counter to keep track of the next available color pair ID
        short _nextColorPairId;
    };
}  // namespace arcade
