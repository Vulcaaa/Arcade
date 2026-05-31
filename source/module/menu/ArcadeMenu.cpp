/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** ArcadeMenu
*/

/// @file ArcadeMenu.cpp
/// @brief Implementation of the ArcadeMenu class, which provides a menu interface for selecting
/// games and graphics libraries in the arcade application.

#include "ArcadeMenu.hpp"

#include "IDisplayModule.hpp"
#include <cstddef>
#include <sstream>
#include <fstream>
#include <algorithm>

static const std::string GAMENAME = "ArcadeMenu";
static const std::string EMPTY = "";

arcade::ArcadeMenu::ArcadeMenu()
    : AGameModule(GAMENAME, GameState::MENU, RenderContext(800, 600, CoordinateSpace::PIXEL)),
      _currentSection(CurrentChoice::ONGRAPHIC),
      _validatedSection(CurrentChoice::ONGRAPHIC),
      _hasValidatedSelection(false),
      _displayIdx(0),
      _gameIdx(0),
      _needsUpdate(true),
      _playerName("Guest"),
      _isInPlayerBox(false)
{
    setMenuOrientedSprites();
}

void arcade::ArcadeMenu::setMenuOrientedSprites()
{
    setOrientedSprite("arrow", Orientation::LEFT, "sprite/menu/arrowLeft.png");
    setOrientedSprite("arrow", Orientation::RIGHT, "sprite/menu/arrowRight.png");
}

void arcade::ArcadeMenu::setDisplayPaths(const std::vector<std::string>& paths) {
    _displayPaths = paths;
    _needsUpdate = true;
}

void arcade::ArcadeMenu::setGamePaths(const std::vector<std::string>& paths) {
    _gamePaths = paths;
    _needsUpdate = true;
}

void arcade::ArcadeMenu::setDisplayIdx(const int idx)
{
    _currentDisplayIdx = idx;
    _needsUpdate = true;
}

void arcade::ArcadeMenu::restart()
{
    _state = GameState::RUNNING;
    _gameIdx = 0;
    _displayIdx = 0;
    _currentSection = CurrentChoice::ONGRAPHIC;
    _validatedSection = CurrentChoice::ONGRAPHIC;
    _hasValidatedSelection = false;
    _needsUpdate = true;
}

void arcade::ArcadeMenu::validateSelection()
{
    _validatedSection = _currentSection;
    _hasValidatedSelection = true;
    _state = GameState::WON;
}

void arcade::ArcadeMenu::handleInput(Key key)
{
    _needsUpdate = true;
    if (_isInPlayerBox) {
        handlePlayerNameInput(key);
        return;
    }
    switch (key) {
    case Key::LEFT: leftKey(); break;
    case Key::RIGHT: rightKey(); break;
    case Key::UP:
    case Key::DOWN: switchChoice(key); break;
    case Key::ENTER: validateSelection();
        break;
    case Key::N: _isInPlayerBox = true;
        break;
    default: break;
    }
}

void arcade::ArcadeMenu::handleInput(MouseEvent mouseEvent)
{
    if (mouseEvent.button == Key::UNKNOWN) return;

    if (mouseEvent.button == Key::MOUSE_LEFT) {

        // Check if click is in player name box
        if (!_isInPlayerBox) {
            int boxX = 240;
            int boxY = 550;
            if ((mouseEvent.pos.x >= boxX && mouseEvent.pos.x <= boxX + 325) &&
                (mouseEvent.pos.y >= boxY && mouseEvent.pos.y <= boxY + 25))
                    _isInPlayerBox = true;
        }

        // Check if click is within graphics arrows
        if (_currentSection == CurrentChoice::ONGRAPHIC && !_displayPaths.empty()) {
            int graphicX =  75;
            int graphicY =  189;
            if (mouseEvent.pos.y >= graphicY && mouseEvent.pos.y <= graphicY + 30) {
                if (mouseEvent.pos.x >= graphicX && mouseEvent.pos.x <= graphicX + 21)
                    handleInput(Key::LEFT);
                else if (mouseEvent.pos.x >= 350 && mouseEvent.pos.x <= 375)
                    handleInput(Key::RIGHT);
            }
        }
        // Check if click is within game arrows
        else if (_currentSection == CurrentChoice::ONGAME && !_gamePaths.empty()) {
            int gameX = 75;
            int gameY = 340;
            if (mouseEvent.pos.y >= gameY && mouseEvent.pos.y <= gameY + 30) {
                if (mouseEvent.pos.x >= gameX && mouseEvent.pos.x <= gameX + 21)
                    handleInput(Key::LEFT);
                else if (mouseEvent.pos.x >= 350 && mouseEvent.pos.x <= 375)
                    handleInput(Key::RIGHT);
            }
        }
    }
}

void arcade::ArcadeMenu::leftKey()
{
    if (_currentSection == CurrentChoice::ONGRAPHIC && !_displayPaths.empty())
        _displayIdx = (_displayIdx - 1 + _displayPaths.size()) % _displayPaths.size();
    else if (_currentSection == CurrentChoice::ONGAME && !_gamePaths.empty())
        _gameIdx = (_gameIdx - 1 + _gamePaths.size()) % _gamePaths.size();
}

void arcade::ArcadeMenu::rightKey()
{
    if (_currentSection == CurrentChoice::ONGRAPHIC && !_displayPaths.empty()) {
        _displayIdx = (_displayIdx + 1) % _displayPaths.size();
    } else if (_currentSection == CurrentChoice::ONGAME && !_gamePaths.empty())
        _gameIdx = (_gameIdx + 1) % _gamePaths.size();
}

void arcade::ArcadeMenu::switchChoice(const Key& key)
{
    if (key == Key::UP || key == Key::DOWN) {
        if (_currentSection == CurrentChoice::ONGRAPHIC) {
            _currentSection = CurrentChoice::ONGAME;
            _displayIdx = _currentDisplayIdx;
        } else {
            _currentSection = CurrentChoice::ONGRAPHIC;
        }
    }
}

void arcade::ArcadeMenu::isResized()
{
    static float lastWidth = -1;
    static float lastHeight = -1;

    if (lastWidth != _context.width || lastHeight != _context.height) {
        lastWidth = _context.width;
        lastHeight = _context.height;
        _needsUpdate = true;
    }
}

void arcade::ArcadeMenu::drawTitle(float midX)
{
    _uiText.push_back(Text(
        "Arcade",
        midX,
        65,
        Anchor::CENTER,
        Colors::YELLOW,
        40
    ));
}

void arcade::ArcadeMenu::drawGraphicsSection(float choiceX, float graphicY)
{
    _uiText.push_back(Text(
        "Graphics",
        choiceX + 35,
        graphicY,
        Anchor::MIDDLE_LEFT,
        Colors::CYAN,
        30
    ));

    if (_displayPaths.empty()) return;

    int currentGraphicY = graphicY + 50;

    Entity arrowLeft;
    arrowLeft.color = Colors::RED;
    arrowLeft.spritePath = getOrientedSprite("arrow", Orientation::LEFT);
    arrowLeft.anchor = Anchor::CENTER;
    arrowLeft.character = '<';
    arrowLeft.size = {65, 65};

    Entity arrowRight;
    arrowRight.color = Colors::RED;
    arrowRight.spritePath = getOrientedSprite("arrow", Orientation::RIGHT);
    arrowRight.anchor = Anchor::CENTER;
    arrowRight.character = '>';
    arrowRight.size = {65, 65};

    Color color = Colors::GREEN;
    if (_currentSection == CurrentChoice::ONGRAPHIC && !_isInPlayerBox) {
        color = Colors::RED;
        arrowLeft.pos = {choiceX + 20, static_cast<float>(currentGraphicY)};
        arrowRight.pos = {choiceX + 300, static_cast<float>(currentGraphicY)};
        _entities.push_back(arrowLeft);
        _entities.push_back(arrowRight);
    }

    _uiText.push_back(Text(
        _displayPaths[_displayIdx],
        choiceX + 40,
        currentGraphicY,
        Anchor::MIDDLE_LEFT,
        color,
        10
    ));
}

void arcade::ArcadeMenu::drawGamesSection(float choiceX, float gameY)
{
    _uiText.push_back(Text(
        "Games",
        choiceX + 35,
        gameY,
        Anchor::MIDDLE_LEFT,
        Colors::CYAN,
        30
    ));

    if (_gamePaths.empty()) return;

    int currentGameY = gameY + 50;

    Entity arrowLeft;
    arrowLeft.color = Colors::RED;
    arrowLeft.spritePath = getOrientedSprite("arrow", Orientation::LEFT);
    arrowLeft.character = '<';
    arrowLeft.anchor = Anchor::CENTER;
    arrowLeft.size = {65, 65};

    Entity arrowRight;
    arrowRight.color = Colors::RED;
    arrowRight.spritePath = getOrientedSprite("arrow", Orientation::RIGHT);
    arrowRight.character = '>';
    arrowRight.anchor = Anchor::CENTER;
    arrowRight.size = {65, 65};

    Color color = Colors::WHITE;
    if (_currentSection == CurrentChoice::ONGAME && !_isInPlayerBox) {
        color = Colors::RED;
        arrowLeft.pos = {choiceX + 20, static_cast<float>(currentGameY)};
        arrowRight.pos = {choiceX + 300, static_cast<float>(currentGameY)};
        _entities.push_back(arrowLeft);
        _entities.push_back(arrowRight);
    }

    _uiText.push_back(Text(
        _gamePaths[_gameIdx],
        choiceX + 40,
        currentGameY,
        Anchor::MIDDLE_LEFT,
        color,
        10
    ));
}

void arcade::ArcadeMenu::drawPlayerNameBox(float midX)
{
    _uiText.push_back(Text(
        "Press N to enter your name",
        midX,
        static_cast<float>(_context.height - 80),
        Anchor::CENTER,
        Colors::YELLOW,
        15
    ));

    Entity playerNameBoxTop;
    playerNameBoxTop.pos = {midX, static_cast<float>(_context.height - 50)};
    playerNameBoxTop.anchor = Anchor::CENTER;
    playerNameBoxTop.size = {320, 1};
    playerNameBoxTop.character = '-';
    playerNameBoxTop.spritePath = "sprite/menu/playerNameBox.png";
    _entities.push_back(playerNameBoxTop);

    Entity playerNameBoxBottom;
    playerNameBoxBottom.pos = {midX, static_cast<float>(_context.height - 30)};
    playerNameBoxBottom.anchor = Anchor::CENTER;
    playerNameBoxBottom.size = {320, 1};
    playerNameBoxBottom.character = '-';
    playerNameBoxBottom.spritePath = "sprite/menu/playerNameBox.png";
    _entities.push_back(playerNameBoxBottom);

    Entity playerNameBoxLeft;
    playerNameBoxLeft.pos = {midX - 160, static_cast<float>(_context.height - 40)};
    playerNameBoxLeft.anchor = Anchor::CENTER;
    playerNameBoxLeft.size = {1, 20};
    playerNameBoxLeft.character = '|';
    playerNameBoxLeft.spritePath = "sprite/menu/playerNameBox.png";
    _entities.push_back(playerNameBoxLeft);

    Entity playerNameBoxRight;
    playerNameBoxRight.pos = {midX + 160, static_cast<float>(_context.height - 40)};
    playerNameBoxRight.anchor = Anchor::CENTER;
    playerNameBoxRight.size = {1, 20};
    playerNameBoxRight.character = '|';
    playerNameBoxRight.spritePath = "sprite/menu/playerNameBox.png";
    _entities.push_back(playerNameBoxRight);

    Color nameColor = _isInPlayerBox ? Colors::YELLOW : Colors::WHITE;
    _uiText.push_back(Text(
        _playerName,
        midX,
        static_cast<float>(_context.height - 40),
        Anchor::CENTER,
        nameColor,
        12
    ));
}

void arcade::ArcadeMenu::drawScoreBoard(float boardX, float boardY)
{
    std::string gameName = "Scores";

    if (!_gamePaths.empty() && _gameIdx >= 0 && _gameIdx < static_cast<int>(_gamePaths.size())) {
        std::string path = _gamePaths[_gameIdx];
        size_t startOffset = path.find_last_of('/');
        startOffset = (startOffset == std::string::npos) ? 0 : startOffset + 1;

        if (path.compare(startOffset, 7, "arcade_") == 0) startOffset += 7;

        size_t endOffset = path.find_last_of('.');

        if (endOffset != std::string::npos && endOffset > startOffset)
            gameName = path.substr(startOffset, endOffset - startOffset);
        else gameName = path.substr(startOffset);

        if (!gameName.empty()) gameName[0] = std::toupper(gameName[0]);
    }

    _uiText.push_back(Text(
        gameName + " Scores",
        boardX,
        boardY,
        Anchor::MIDDLE_LEFT,
        Color(231, 22, 242),
        20
    ));

    auto topScores = getTopScores(gameName);

    if (topScores.empty()) {
        _uiText.push_back(Text(
            "No scores yet",
            boardX,
            boardY + 30,
            Anchor::MIDDLE_LEFT,
            Colors::WHITE,
            20
        ));
        return;
    }

    float currentY = boardY + 30;
    for (const auto& [player, score] : topScores) {
        _uiText.push_back(Text(
            player + ": " + std::to_string(score),
            boardX,
            currentY,
            Anchor::TOP_LEFT,
            Colors::WHITE,
            20
        ));
        currentY += 30;
    }
}

void arcade::ArcadeMenu::update([[maybe_unused]] float elapsedTime)
{
    isResized();

    if (!_needsUpdate)
        return;

    _needsUpdate = false;

    float midX = _context.width / 2.0f;
    float midY = _context.height / 2.0f;

    _uiText.clear();
    _entities.clear();

    float choiceX = (midX / 6.0f);
    float graphicY = midY - 150.0f;
    float gameY = midY;

    drawTitle(midX);
    drawGraphicsSection(choiceX, graphicY);
    drawGamesSection(choiceX, gameY);
    drawPlayerNameBox(midX);
    drawScoreBoard(midX + 50, midY - 150);
}

const std::vector<arcade::Entity>& arcade::ArcadeMenu::getEntities() const { return _entities; }

const std::vector<arcade::Text>& arcade::ArcadeMenu::getUIText() const { return _uiText; }

const std::string& arcade::ArcadeMenu::getSelectedGame() const
{
    if (!_hasValidatedSelection || _validatedSection != CurrentChoice::ONGAME) return EMPTY;
    if (_gameIdx >= 0 && _gameIdx < static_cast<int>(_gamePaths.size()))
        return _gamePaths[_gameIdx];
    return EMPTY;
}

const std::string& arcade::ArcadeMenu::getSelectedGraphic() const
{
    if (!_hasValidatedSelection || _validatedSection != CurrentChoice::ONGRAPHIC) return EMPTY;
    if (_displayIdx >= 0 && _displayIdx < static_cast<int>(_displayPaths.size()))
        return _displayPaths[_displayIdx];
    return EMPTY;
}

const std::string& arcade::ArcadeMenu::getPlayerName() const { return _playerName; }

void arcade::ArcadeMenu::setPlayerName(const std::string& name) { _playerName = name; }

void arcade::ArcadeMenu::handlePlayerNameInput(Key key)
{
    std::map<Key, char> keyNames = {
        {Key::A, 'A'}, {Key::B, 'B'}, {Key::C, 'C'}, {Key::D, 'D'}, {Key::E, 'E'},
        {Key::F, 'F'}, {Key::G, 'G'}, {Key::H, 'H'}, {Key::I, 'I'}, {Key::J, 'J'},
        {Key::K, 'K'}, {Key::L, 'L'}, {Key::M, 'M'}, {Key::N, 'N'}, {Key::O, 'O'},
        {Key::P, 'P'}, {Key::Q, 'Q'}, {Key::R, 'R'}, {Key::S, 'S'}, {Key::T, 'T'},
        {Key::U, 'U'}, {Key::V, 'V'}, {Key::W, 'W'}, {Key::X, 'X'}, {Key::Y, 'Y'},
        {Key::Z, 'Z'},
        {Key::NUM_0, '0'}, {Key::NUM_1, '1'}, {Key::NUM_2, '2'}, {Key::NUM_3, '3'},
        {Key::NUM_4, '4'}, {Key::NUM_5, '5'}, {Key::NUM_6, '6'}, {Key::NUM_7, '7'},
        {Key::NUM_8, '8'}, {Key::NUM_9, '9'}
    };

    if (_playerName == "Guest")
            _playerName = "";
    if (key == Key::BACKSPACE && !_playerName.empty())
        _playerName.pop_back();
    else if (key >= Key::A && key <= Key::Z)
        _playerName += keyNames[key];
    else if (key >= Key::NUM_0 && key <= Key::NUM_9)
        _playerName += keyNames[key];
    else if (key == Key::ENTER) {
        _isInPlayerBox = false;
        if (_playerName.empty()) _playerName = "Guest";
    }
}

std::vector<std::pair<std::string, int>> arcade::ArcadeMenu::getTopScores(const std::string& gameName) const
{
    std::vector<std::pair<std::string, int>> scores;
    std::ifstream file("saves.csv");
    std::string line;

    if (!file.is_open()) return scores;

    // Skip the header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string player;
        std::string scoreStr;
        std::string game;

        std::getline(ss, player, ',');
        std::getline(ss, scoreStr, ',');
        std::getline(ss, game, ',');

        if (!gameName.empty() && game != gameName) continue;

        scores.push_back({player, std::stoi(scoreStr)});
    }

    std::sort(scores.begin(), scores.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return a.second > b.second;
    });

    if (scores.size() > 5) scores.resize(5);

    return scores;
}

extern "C" {
    arcade::IModule* entryPoint() {
        return new arcade::ArcadeMenu();
    }
}
