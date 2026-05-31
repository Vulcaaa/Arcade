/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** NCURSESDisplay
*/

/// @file NCURSESDisplay.cpp
/// @brief NCURSES implementation

#include "NCURSESDisplay.hpp"

#include <unordered_map>

/// @brief Name of the NCURSES display module
static const std::string DISPLAY_NAME = "NCURSES";

/// @brief Mapping of NCURSES key codes to arcade::Key values
static const std::unordered_map<int, arcade::Key> KEY_BINDINGS = {
    {'a', arcade::Key::A},
    {'A', arcade::Key::A},
    {'b', arcade::Key::B},
    {'B', arcade::Key::B},
    {'c', arcade::Key::C},
    {'C', arcade::Key::C},
    {'d', arcade::Key::D},
    {'D', arcade::Key::D},
    {'e', arcade::Key::E},
    {'E', arcade::Key::E},
    {'f', arcade::Key::F},
    {'F', arcade::Key::F},
    {'g', arcade::Key::G},
    {'G', arcade::Key::G},
    {'h', arcade::Key::H},
    {'H', arcade::Key::H},
    {'i', arcade::Key::I},
    {'I', arcade::Key::I},
    {'j', arcade::Key::J},
    {'J', arcade::Key::J},
    {'k', arcade::Key::K},
    {'K', arcade::Key::K},
    {'l', arcade::Key::L},
    {'L', arcade::Key::L},
    {'m', arcade::Key::MENU},
    {'M', arcade::Key::MENU},
    {'n', arcade::Key::N},
    {'N', arcade::Key::N},
    {'o', arcade::Key::O},
    {'O', arcade::Key::O},
    {'p', arcade::Key::P},
    {'P', arcade::Key::P},
    {'q', arcade::Key::Q},
    {'Q', arcade::Key::Q},
    {'r', arcade::Key::RESTART},
    {'R', arcade::Key::RESTART},
    {'s', arcade::Key::S},
    {'S', arcade::Key::S},
    {'t', arcade::Key::T},
    {'T', arcade::Key::T},
    {'u', arcade::Key::U},
    {'U', arcade::Key::U},
    {'v', arcade::Key::V},
    {'V', arcade::Key::V},
    {'w', arcade::Key::W},
    {'W', arcade::Key::W},
    {'x', arcade::Key::X},
    {'X', arcade::Key::X},
    {'y', arcade::Key::Y},
    {'Y', arcade::Key::Y},
    {'z', arcade::Key::Z},
    {'Z', arcade::Key::Z},
    {'0', arcade::Key::NUM_0},
    {(int)(-15456), arcade::Key::NUM_0},
    {'1', arcade::Key::PREV_GAME},
    {'&', arcade::Key::PREV_GAME},
    {'2', arcade::Key::NEXT_GAME},
    {(int)(-15447), arcade::Key::NEXT_GAME},
    {'3', arcade::Key::PREV_GRAPHICS},
    {'"', arcade::Key::PREV_GRAPHICS},
    {'4', arcade::Key::NEXT_GRAPHICS},
    {'\'', arcade::Key::NEXT_GRAPHICS},
    {'5', arcade::Key::NUM_5},
    {'(', arcade::Key::NUM_5},
    {'6', arcade::Key::NUM_6},
    {'-', arcade::Key::NUM_6},
    {'7', arcade::Key::NUM_7},
    {(int)(-15448), arcade::Key::NUM_7},
    {'8', arcade::Key::NUM_8},
    {'_', arcade::Key::NUM_8},
    {'9', arcade::Key::NUM_9},
    {(int)(-15449), arcade::Key::NUM_9},
    {' ', arcade::Key::SPACE},
    {KEY_ENTER, arcade::Key::ENTER},
    {10, arcade::Key::ENTER},
    {'\n', arcade::Key::ENTER},
    {13, arcade::Key::ENTER},
    {27, arcade::Key::EXIT},
    {KEY_BACKSPACE, arcade::Key::BACKSPACE},
    {KEY_UP, arcade::Key::UP},
    {KEY_DOWN, arcade::Key::DOWN},
    {KEY_LEFT, arcade::Key::LEFT},
    {KEY_RIGHT, arcade::Key::RIGHT}
};

/// @brief Mapping of NCURSES mouse button codes to arcade::Key values
static const std::unordered_map<mmask_t, arcade::Key> MOUSE_BINDINGS = {
    {BUTTON1_PRESSED, arcade::Key::MOUSE_LEFT},
    {BUTTON3_PRESSED, arcade::Key::MOUSE_RIGHT},
    {BUTTON2_PRESSED, arcade::Key::MOUSE_MIDDLE},
};

arcade::NCURSESDisplay::NCURSESDisplay()
    : ADisplayModule(DISPLAY_NAME), _win(nullptr), _nextColorPairId(0)
{}

arcade::NCURSESDisplay::~NCURSESDisplay() { stop(); }

void arcade::NCURSESDisplay::init()
{
    // Initialize NCURSES
    initscr();

    // Check if the terminal supports colors and initialize color mode
    if (has_colors()) start_color();

    // Enable cbreak mode to allow immediate input without waiting for Enter
    cbreak();
    // Disable echoing of typed characters
    noecho();
    // Enable special keys
    keypad(stdscr, TRUE);
    // Make getch non-blocking
    nodelay(stdscr, TRUE);
    // Hide the cursor
    curs_set(0);
    // Enable mouse events
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

    _win = stdscr;
}

void arcade::NCURSESDisplay::stop()
{
    if (_win) {
        // Clear color pair cache
        _colorPairs.clear();
        _nextColorPairId = 0;

        // End NCURSES mode
        endwin();
        _win = nullptr;
    }
}

void arcade::NCURSESDisplay::clear() { wclear(_win); }

void arcade::NCURSESDisplay::display() { wrefresh(_win); }

void arcade::NCURSESDisplay::drawEntity(const Entity& entity)
{
    int x, y;
    if (_context.space == CoordinateSpace::PIXEL) {
        int winW, winH;
        getmaxyx(_win, winH, winW);
        x = static_cast<int>(entity.pos.x * winW / _context.width);
        y = static_cast<int>(entity.pos.y * winH / _context.height);
    } else {
        Viewport vp = computeViewport();
        x = static_cast<int>(entity.pos.x * vp.scale) + vp.offsetX;
        y = static_cast<int>(entity.pos.y * vp.scale) + vp.offsetY;
    }

    int width = 1;
    int height = 1;
    applyAnchor(x, y, width, height, entity.anchor);

    // Set color and draw the character
    short colorPair = getColorPair(entity.color, entity.backgroundColor);
    attron(COLOR_PAIR(colorPair));
    mvwaddch(_win, y, x, entity.character);
    attroff(COLOR_PAIR(colorPair));
}

void arcade::NCURSESDisplay::drawText(const Text& text)
{
    int x, y;
    if (_context.space == CoordinateSpace::PIXEL) {
        int winW, winH;
        getmaxyx(_win, winH, winW);
        x = static_cast<int>(text.pos.x * winW / _context.width);
        y = static_cast<int>(text.pos.y * winH / _context.height);
    } else {
        Viewport vp = computeViewport();
        x = static_cast<int>(text.pos.x * vp.scale) + vp.offsetX;
        y = static_cast<int>(text.pos.y * vp.scale) + vp.offsetY;
    }

    int textWidth = static_cast<int>(text.content.length());
    applyAnchor(x, y, textWidth, 1, text.anchor);

    // Set color and draw the text
    short colorPair = getColorPair(text.color, {0, 0, 0});
    attron(COLOR_PAIR(colorPair));
    mvwprintw(_win, y, x, "%s", text.content.c_str());
    attroff(COLOR_PAIR(colorPair));
}

void arcade::NCURSESDisplay::drawRectangle(const Rect& rect, const Color& color)
{
    int x0, y0, x1, y1;
    if (_context.space == CoordinateSpace::PIXEL) {
        int winW, winH;
        getmaxyx(_win, winH, winW);
        x0 = static_cast<int>(rect.x * winW / _context.width);
        y0 = static_cast<int>(rect.y * winH / _context.height);
        x1 = static_cast<int>((rect.x + rect.width) * winW / _context.width);
        y1 = static_cast<int>((rect.y + rect.height) * winH / _context.height);
    } else {
        Viewport vp = computeViewport();
        x0 = static_cast<int>(rect.x * vp.scale) + vp.offsetX;
        y0 = static_cast<int>(rect.y * vp.scale) + vp.offsetY;
        x1 = static_cast<int>((rect.x + rect.width) * vp.scale) + vp.offsetX;
        y1 = static_cast<int>((rect.y + rect.height) * vp.scale) + vp.offsetY;
    }

    short colorPair = getColorPair(color, color);
    attron(COLOR_PAIR(colorPair));
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            mvwaddch(_win, y, x, ' ');
        }
    }
    attroff(COLOR_PAIR(colorPair));
}

void arcade::NCURSESDisplay::mouseClickHandler()
{
    MEVENT event;

    if (getmouse(&event) == OK) {
        int termW, termH;
        getmaxyx(_win, termH, termW);

        float x = (static_cast<float>(event.x) / termW) * _context.width;
        float y = (static_cast<float>(event.y) / termH) * _context.height;

        x = std::clamp(x, 0.0f, static_cast<float>(_context.width));
        y = std::clamp(y, 0.0f, static_cast<float>(_context.height));

        auto it = MOUSE_BINDINGS.find(event.bstate & (BUTTON1_PRESSED | BUTTON2_PRESSED | BUTTON3_PRESSED));
        if (it != MOUSE_BINDINGS.end())
            _mouseEvent.push(MouseEvent{{x, y}, it->second});
    }
}

void arcade::NCURSESDisplay::pollEvents()
{
    updateDeltaTime();

    int ch;
    while ((ch = wgetch(_win)) != ERR) {
        if (ch == KEY_MOUSE)
            mouseClickHandler();
        else {
            auto it = KEY_BINDINGS.find(ch);
            if (it != KEY_BINDINGS.end()) _keyEvent.push(it->second);
        }
    }
}

bool arcade::NCURSESDisplay::isOpen() const { return _win != nullptr; }

arcade::ADisplayModule::Viewport arcade::NCURSESDisplay::computeViewport() const
{
    int winW, winH;
    getmaxyx(_win, winH, winW);

    float scale = 1.0f;

    // Pixel Mode
    if (_context.space == CoordinateSpace::PIXEL) {
        float scaleX = static_cast<float>(winW) / _context.width;
        float scaleY = static_cast<float>(winH) / _context.height;
        scale = std::min(scaleX, scaleY);
    }
    // Grid Mode
    else scale = 1.0f;

    // Center the content
    int offsetX = static_cast<int>((winW - _context.width * scale) / 2.0f);
    int offsetY = static_cast<int>((winH - _context.height * scale) / 2.0f);

    return {scale, offsetX, offsetY};
}



short arcade::NCURSESDisplay::getClosestNcursesColor(const Color& color) const
{
    // Simple mapping of RGB to 8 basic colors (0-7)
    if (color.r > 128) {
        if (color.g > 128) {
            if (color.b > 128)
                return COLOR_WHITE;
            else
                return COLOR_YELLOW;
        } else {
            if (color.b > 128)
                return COLOR_MAGENTA;
            else
                return COLOR_RED;
        }
    } else {
        if (color.g > 128) {
            if (color.b > 128)
                return COLOR_CYAN;
            else
                return COLOR_GREEN;
        } else {
            if (color.b > 128)
                return COLOR_BLUE;
            else
                return COLOR_BLACK;
        }
    }
}

short arcade::NCURSESDisplay::getColorPair(const Color& fg, const Color& bg)
{
    short fgColor = getClosestNcursesColor(fg);
    short bgColor = getClosestNcursesColor(bg);

    std::pair<short, short> key = {fgColor, bgColor};
    auto it = _colorPairs.find(key);
    if (it != _colorPairs.end()) return it->second;

    if (_nextColorPairId >= COLOR_PAIRS) return 0;

    short pairId = _nextColorPairId++;
    init_pair(pairId, fgColor, bgColor);
    _colorPairs[key] = pairId;
    return pairId;
}

extern "C" {
    arcade::IModule* entryPoint() {
        return new arcade::NCURSESDisplay();
    }
}
