/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** ALLEGRO5Display
*/

/// @file ALLEGRO5Display.hpp
/// @brief ALLEGRO5 implementation of the IDisplayModule interface

#pragma once

#include "ADisplayModule.hpp"

#include <map>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/color.h>
#include <allegro5/drawing.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keycodes.h>

namespace arcade {
    /// @brief ALLEGRO5 implementation of the IDisplayModule interface
    class ALLEGRO5 : public ADisplayModule {
        public:
        /// @brief Constructor for the ALLEGRO5 display module
        ALLEGRO5();
        /// @brief Destructor for the ALLEGRO5 display module
        ~ALLEGRO5();

        /// @brief Initialize the ALLEGRO5 display module
        /// @throw std::runtime_error if initialization fails
        void init() override;

        /// @brief Stop and cleanup the ALLEGRO5 display module
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
        /// @brief Get a sprite from the cache or load a new one
        /// @param path The path to the sprite file
        /// @return A pointer to the loaded sprite
        ALLEGRO_BITMAP* getSprite(const std::string& path);

        /// @brief Try to open a font from the specified path
        /// @param fontPath The path to the font file
        /// @param size The size of the font
        /// @return A pointer to the loaded font, or nullptr if failed
        ALLEGRO_FONT* tryOpenFont(const std::string& fontPath, int size);

        /// @brief Get a fallback font if the specified font cannot be loaded
        /// @param size The size of the font
        /// @return A pointer to the fallback font
        ALLEGRO_FONT* getFallbackFont(int size);

        /// @brief Get a font from the cache or load a new one
        /// @param path The path to the font file
        /// @param size The size of the font
        /// @return A pointer to the loaded font
        ALLEGRO_FONT* getFont(const std::string& path, int size);

        /// @brief Compute the viewport for the display
        /// @return The computed viewport containing scale and offsets for rendering
        Viewport computeViewport() const override;

        /// @brief Handle mouse click events and convert them to game input
        void mouseClickHandler(const ALLEGRO_EVENT& event);

        /// @brief Stop and cleanup loaded fonts
        void stopFont();

        /// @brief Stop and cleanup loaded sprites
        void stopSprites();

        /// @brief The ALLEGRO display
        ALLEGRO_DISPLAY* _win;

        /// @brief The ALLEGRO event queue
        ALLEGRO_EVENT_QUEUE *_queue;

        /// @brief Cache for loaded fonts, keyed by (path, size)
        std::map<std::pair<std::string, int>, ALLEGRO_FONT*> _fontCache;

        /// @brief Cache for loaded sprites, keyed by file path
        std::map<std::string, ALLEGRO_BITMAP*> _spriteCache;
    };
}  // namespace arcade
