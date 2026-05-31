/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** IDisplayModule
*/

/// @file IDisplayModule.hpp
/// @brief Interface for graphics libraries in the Arcade project

#pragma once

#include <cstdint>
#include <string>

#include "IModule.hpp"

namespace arcade {

    /// @enum Key
    /// @brief Enumeration of all possible key inputs
    enum class Key {
        UNKNOWN = 0,
        // Arrows
        UP,
        DOWN,
        LEFT,
        RIGHT,
        // Actions
        SPACE,
        ENTER,
        ESCAPE,
        BACKSPACE,
        // Numbers
        NUM_0,
        NUM_1,
        NUM_2,
        NUM_3,
        NUM_4,
        NUM_5,
        NUM_6,
        NUM_7,
        NUM_8,
        NUM_9,
        // Letters A-Z
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        // Mouse buttons
        MOUSE_LEFT,
        MOUSE_RIGHT,
        MOUSE_MIDDLE,
        // Meta keys for Arcade control
        NEXT_GRAPHICS,  ///< Switch to next graphics library
        PREV_GRAPHICS,  ///< Switch to previous graphics library
        NEXT_GAME,      ///< Switch to next game
        PREV_GAME,      ///< Switch to previous game
        RESTART,        ///< Restart current game
        MENU,           ///< Return to menu
        EXIT,           ///< Exit the program
    };

    /// @struct Color
    /// @brief RGBA color representation
    struct Color {
        uint8_t r, g, b, a;

        /// @brief Construct a color
        /// @param red Red component (default: 255)
        /// @param green Green component (default: 255)
        /// @param blue Blue component (default: 255)
        /// @param alpha Alpha component (default: 255)
        Color(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255)
            : r(red), g(green), b(blue), a(alpha)
        {
        }
    };

    /// @namespace arcade::Colors
    /// @brief Predefined common colors
    namespace Colors {
        const Color BLACK(0, 0, 0);
        const Color WHITE(255, 255, 255);
        const Color RED(255, 0, 0);
        const Color GREEN(0, 255, 0);
        const Color BLUE(0, 0, 255);
        const Color YELLOW(255, 255, 0);
        const Color CYAN(0, 255, 255);
        const Color MAGENTA(255, 0, 255);
    }  // namespace Colors

    /// @struct Position
    /// @brief 2D position in screen space
    struct Position {
        float x, y;

        /// @brief Construct a position
        /// @param posX X coordinate (default: 0.0)
        /// @param posY Y coordinate (default: 0.0)

        Position(float posX = 0.0f, float posY = 0.0f) : x(posX), y(posY) {}
    };

    /// @enum Anchor
    /// @brief Anchor point for positioning rectangle, entities and text
    enum class Anchor {
        TOP_LEFT,       ///< Anchor at top-left corner (default)
        TOP_CENTER,     ///< Anchor at top-center
        TOP_RIGHT,      ///< Anchor at top-right corner
        MIDDLE_LEFT,    ///< Anchor at middle-left
        CENTER,         ///< Anchor at center
        MIDDLE_RIGHT,   ///< Anchor at middle-right
        BOTTOM_LEFT,    ///< Anchor at bottom-left corner
        BOTTOM_CENTER,  ///< Anchor at bottom-center
        BOTTOM_RIGHT    ///< Anchor at bottom-right corner
    };

    /// @struct Rect
    /// @brief Rectangle definition for sprites and shapes
    struct Rect {
        int x;          ///< X coordinate of anchor point
        int y;          ///< Y coordinate of anchor point
        int width;      ///< Width of rectangle
        int height;     ///< Height of rectangle
        Anchor anchor;  ///< Anchor point for positioning (default: TOP_LEFT)

        /// @brief Construct a rectangle
        /// @param posX X coordinate (default: 0)
        /// @param posY Y coordinate (default: 0)
        /// @param w Width (default: 0)
        /// @param h Height (default: 0)
        /// @param a Anchor point (default: TOP_LEFT)
        Rect(int posX = 0, int posY = 0, int w = 0, int h = 0, Anchor a = Anchor::TOP_LEFT)
            : x(posX), y(posY), width(w), height(h), anchor(a)
        {}
    };

    /// @enum CoordinateSpace
    /// @brief Coordinate space used by a game for rendering
    enum class CoordinateSpace {
        GRID,  ///< Logical grid coordinates (tile/cell based)
        PIXEL  ///< Logical pixel coordinates
    };

    /// @struct RenderContext
    /// @brief Logical render space description provided by a game
    ///
    /// @note How display modules transform game coordinates to screen coordinates
    /// (scaling, centering, aspect ratio handling) is implementation-defined.
    /// Each display module decides its own transformation strategy.
    struct RenderContext {
        int width;              ///< Logical width of the game world
        int height;             ///< Logical height of the game world
        CoordinateSpace space;  ///< Coordinate system used by game entities/text

        RenderContext(int w = 0, int h = 0, CoordinateSpace s = CoordinateSpace::GRID)
            : width(w), height(h), space(s)
        {}
    };

    /// @struct Entity
    /// @brief Displayable game entity (character, sprite, or tile)
    ///
    /// This structure supports both terminal-based (ASCII) and graphical rendering.
    /// - Terminal libraries use the character field
    /// - Graphical libraries use the spritePath field
    ///
    /// @note Entity::size <= 0 means "auto":
    /// - For ASCII rendering: 1x1 character cell
    /// - For graphical rendering: native sprite dimensions
    ///
    /// @note Entity::backgroundColor:
    /// - Terminal displays: render as cell background color
    /// - Graphical displays: should ignore this field
    ///
    /// @note Supported sprite formats: PNG, JPG
    /// Each sprite is a single complete image file (no sprite sheets/atlases).
    struct Entity {
        Position pos;            ///< Position on screen
        Anchor anchor;           ///< Anchor point for positioning (default: TOP_LEFT)
        Position size;           ///< Logical entity size (width/height), <= 0 means auto
        char character;          ///< ASCII character (for terminal-based display)
        Color color;             ///< Entity foreground color
        Color backgroundColor;   ///< Entity background color (terminal only)
        std::string spritePath;  ///< Path to sprite image file (PNG or JPG)

        /// @brief Default constructor
        Entity()
            : anchor(Anchor::TOP_LEFT),
              size(0.0f, 0.0f),
              character(' '),
              color(Colors::WHITE),
              backgroundColor(Colors::BLACK)
        {}
    };

    /// @struct Text
    /// @brief Text to be displayed on screen
    struct Text {
        std::string content;   ///< Text content to display
        Position pos;          ///< Position on screen
        Anchor anchor;         ///< Anchor point for positioning (default: TOP_LEFT)
        Color color;           ///< Text color
        int fontSize;          ///< Font size (in points or equivalent)
        std::string fontPath;  ///< Path to custom font file (empty = system default)

        /// @brief Construct a text object
        /// @param text Text content (default: empty string)
        /// @param x X coordinate (default: 0)
        /// @param y Y coordinate (default: 0)
        /// @param textColor Text color (default: white)
        /// @param size Font size (default: 12)
        /// @param font Font path (default: empty = system default)
        /// @param a Anchor point (default: TOP_LEFT)
        Text(const std::string& text = "", float x = 0, float y = 0, Anchor a = Anchor::TOP_LEFT,
             const Color& textColor = Colors::WHITE, int size = 12, const std::string& font = "")
            : content(text), pos(x, y), anchor(a), color(textColor), fontSize(size), fontPath(font)
        {}
    };

    /// @struct MouseEvent
    /// @brief Mouse input event data
    struct MouseEvent {
        Position pos;  ///< Mouse position
        Key button;    ///< Mouse button that was clicked (MOUSE_LEFT, MOUSE_RIGHT, MOUSE_MIDDLE, or
                       ///< UNKNOWN)

        MouseEvent() : button(Key::UNKNOWN) {}
        MouseEvent(const Position& position, Key btn) : pos(position), button(btn) {}
    };

    /// @class IDisplayModule
    /// @brief Interface for graphics libraries
    ///
    /// This interface must be implemented by all graphics libraries (nCurses, SDL2, SFML, etc.)
    /// The core program will use this interface to render the game state without knowing
    /// which specific graphics library is being used.
    ///
    /// Graphics libraries must NOT contain any game logic. They are purely responsible for
    /// rendering entities and handling input events.
    class IDisplayModule : public IModule {
        public:
        /// @brief Virtual destructor
        virtual ~IDisplayModule() = default;

        /// @brief Initialize the display module
        ///
        /// Opens window/terminal and prepares the graphics system for rendering.
        /// This method should be called once before any rendering operations.
        ///
        /// @throw ModuleException if initialization fails
        virtual void init() = 0;

        /// @brief Stop and cleanup the display module
        ///
        /// Closes window/terminal and releases all graphics resources.
        /// This method should be called once when shutting down.
        virtual void stop() = 0;

        /// @brief Poll and cache all pending input events
        ///
        /// Should be called once per frame. Drains the event queue and caches events
        /// for getKey() and getMouseEvent(). Implementations should handle window
        /// close events and update isOpen() state accordingly.
        ///
        /// @note This method does not block. It processes all queued events and returns
        /// immediately.
        virtual void pollEvents() = 0;

        /// @brief Get the name of this display module
        /// @return Name of the graphics library (e.g., "SFML", "SDL2", "nCurses")
        virtual const std::string& getName() const = 0;

        /// @brief Clear the screen/window
        ///
        /// Should be called at the beginning of each frame before drawing.
        virtual void clear() = 0;

        /// @brief Display/present the current frame
        ///
        /// Should be called at the end of each frame to show all rendered content.
        /// This typically performs a buffer swap or screen refresh.
        virtual void display() = 0;

        /// @brief Draw an entity at a specific position
        /// @param entity The entity to draw (contains position, color, character/sprite info)
        virtual void drawEntity(const Entity& entity) = 0;

        /// @brief Draw text on screen
        /// @param text The text structure containing content, position, color, and size
        virtual void drawText(const Text& text) = 0;

        /// @brief Draw a filled rectangle
        /// @param rect Rectangle dimensions and position
        /// @param color Fill color
        virtual void drawRectangle(const Rect& rect, const Color& color) = 0;

        /// @brief Get the last key pressed
        ///
        /// Non-blocking call that returns the last key event.
        /// Should return Key::UNKNOWN if no key was pressed since last check.
        ///
        /// @return Key code or Key::UNKNOWN if no key was pressed
        virtual Key getKey() = 0;

        /// @brief Get the last mouse event
        ///
        /// Non-blocking call that returns the last mouse click event.
        /// Returns MouseEvent with button = Key::UNKNOWN if no mouse event occurred.
        ///
        /// @return Mouse event data (position and button)
        virtual MouseEvent getMouseEvent() = 0;

        /// @brief Check if the display is still open/running
        /// @return true if window is open, false if user closed it
        virtual bool isOpen() const = 0;

        /// @brief Set the window/screen title
        /// @param title The title string
        virtual void setTitle(const std::string& title) = 0;

        /// @brief Set the window size
        /// @param width Window width in pixels (or columns for terminal)
        /// @param height Window height in pixels (or rows for terminal)
        virtual void setWindowSize(int width, int height) = 0;

        /// @brief Set fullscreen mode
        /// @param fullscreen true for fullscreen, false for windowed
        virtual void setFullscreen(bool fullscreen) = 0;

        /// @brief Provide the game's logical render context to the display module
        /// @param context Logical render context (size + coordinate space)
        virtual void setRenderContext(const RenderContext& context) = 0;

        /// @brief Get the time elapsed since the last frame
        /// @return Delta time in seconds
        virtual float getDeltaTime() const = 0;

        /// @brief Get the display width
        /// @return Display width in cells (for terminal) or pixels (for graphical)
        virtual int getWidth() const = 0;

        /// @brief Get the display height
        /// @return Display height in cells (for terminal) or pixels (for graphical)
        virtual int getHeight() const = 0;
    };
}  // namespace arcade
