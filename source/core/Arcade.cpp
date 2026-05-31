/*
** EPITECH PROJECT, 2026
** G-OOP-400-MPL-4-1-arcade-5
** File description:
** Arcade
*/

/// @file Arcade.cpp
/// @brief Implementation of the Arcade class, which manages the main game loop, module loading, and
/// state transitions for the arcade application.

#include "Arcade.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <thread>

#include "IMenuModule.hpp"

arcade::Arcade::Arcade(int ac, char* av[])
    : _graphicLoader(nullptr),
      _gameLoader(nullptr),
      _graphicInstance(nullptr),
      _gameInstance(nullptr),
      _displayIdx(-1),
      _gameIdx(-1),
      _gameState(GameState::MENU),
      _playerName("Guest"),
      _isRunning(true)
{
    if (ac != 2) {
        printUsage(av[0]);
        throw std::invalid_argument("Invalid number of arguments.");
    }

    // Load every lib
    loadLibrairies("lib", av[1]);

    // Start the menu
    menuManager();

    // Load the initial graphic library
    try {
        _graphicLoader = std::make_unique<DLLoader<IDisplayModule>>(av[1]);
        _graphicInstance = _graphicLoader->getInstance();
        if (_graphicInstance->getType() != ModuleType::DISPLAY)
            throw std::invalid_argument(std::string(av[1]) + ": Not a display module.");
        _graphicInstance->setRenderContext(_gameInstance->getRenderContext());
        _graphicInstance->init();
    } catch (const DLLoader<IDisplayModule>::DLLoaderException& e) {
        throw std::invalid_argument("Failed to load graphic library.");
    }
}

void arcade::Arcade::loadLibrairies(const std::string& loadPath, const std::string& initGraphicPath)
{
    if (!std::filesystem::exists(loadPath))
        throw std::runtime_error(loadPath + ": Directory not found.");

    for (const auto& file : std::filesystem::directory_iterator(loadPath)) {
        if (file.is_regular_file() && file.path().extension() == ".so") {
            try {
                DLLoader<IModule> modLoader(file.path().string());
                IModule* modInstance = modLoader.getInstance();

                if (modInstance->getType() == ModuleType::DISPLAY)
                    _displayPaths.push_back(file.path().string());
                else if (modInstance->getType() == ModuleType::GAME) {
                    if (modInstance->getName() != "ArcadeMenu")
                        _gamePaths.push_back(file.path().string());
                    else
                        _menuPath = file.path().string();
                }
                delete modInstance;
            } catch (const DLLoader<IModule>::DLLoaderException& e) {
                std::cerr << file.path().string() << ": " << e.what() << std::endl;
                continue;
            }
        }
    }
    setDisplayIdx(initGraphicPath);
}

void arcade::Arcade::menuManager()
{
    // Stop and clean up the game
    if (_gameInstance != nullptr) {
        _gameInstance->stop();
        delete _gameInstance;
        _gameInstance = nullptr;
    }

    // Reset the loader (which destroys the old instance)
    _gameLoader.reset();

    // Start a new menu
    if (_menuPath.empty()) throw std::runtime_error("ArcadeMenu.so not found in lib directory.");

    _gameLoader = std::make_unique<DLLoader<IGameModule>>(_menuPath);
    _gameInstance = _gameLoader->getInstance();
    _gameInstance->init();

    IMenuModule* menu = dynamic_cast<IMenuModule*>(_gameInstance);
    if (menu) {
        menu->setDisplayPaths(_displayPaths);
        menu->setGamePaths(_gamePaths);
        menu->setDisplayIdx(_displayIdx);
        menu->setPlayerName(_playerName);
    }

    _gameState = GameState::MENU;
}

void arcade::Arcade::moduleSwitcher(arcade::Key key)
{
    int idx = 0;

    switch (key) {
        case Key::NEXT_GRAPHICS:
        case Key::PREV_GRAPHICS: {
            if (_displayPaths.empty()) return;
            if (key == Key::NEXT_GRAPHICS) 
                idx = (_displayIdx + 1) % _displayPaths.size();
            else
                idx = (_displayIdx - 1 + _displayPaths.size()) % _displayPaths.size();
            loadDisplayFromPath(_displayPaths[idx]);
            IMenuModule* menu = dynamic_cast<IMenuModule*>(_gameInstance);
            if (menu) menu->setDisplayIdx(_displayIdx);
            break;
        }
        case Key::NEXT_GAME:
        case Key::PREV_GAME:
            if (_gamePaths.empty() || _gameState == GameState::MENU) return;
            if (key == Key::NEXT_GAME)
                idx = (_gameIdx + 1) % _gamePaths.size();
            else
                idx = (_gameIdx - 1 + _gamePaths.size()) % _gamePaths.size();
            loadGameFromPath(_gamePaths[idx]);
            break;
        default:
            break;
    }
}

bool arcade::Arcade::processKeys()
{
    arcade::Key key;

    while ((key = _graphicInstance->getKey()) != Key::UNKNOWN) {
        if (key == Key::EXIT) {
            _isRunning = false;
            return false;
        } else if (key == Key::MENU && _gameState != GameState::MENU)
            menuManager();
        else if (key == Key::RESTART && _gameState != GameState::MENU) {
            _gameInstance->restart();
            _gameState = GameState::RUNNING;
        } else if (key == Key::PREV_GAME || key == Key::NEXT_GAME || key == Key::PREV_GRAPHICS || key == Key::NEXT_GRAPHICS)
            moduleSwitcher(key);
        else
            _gameInstance->handleInput(key);
    }
    return true;
}

bool arcade::Arcade::processMouseEvents()
{
    arcade::MouseEvent mouseEvent;
    while ((mouseEvent = _graphicInstance->getMouseEvent()).button != Key::UNKNOWN)
        _gameInstance->handleInput(mouseEvent);
    return true;
}

bool arcade::Arcade::inputsManager()
{
    _graphicInstance->pollEvents();

    return processKeys() && processMouseEvents();
}

void arcade::Arcade::loadGameFromPath(const std::string& selectedGame)
{
    try {
        // Stop old instance before destroying the old loader
        if (_gameInstance != nullptr) {
            _gameInstance->stop();
            delete _gameInstance;
            _gameInstance = nullptr;
        }

        // Create new loader and get the instance
        auto newGameLoader = std::make_unique<DLLoader<IGameModule>>(selectedGame);
        IGameModule* newGameInstance = newGameLoader->getInstance();

        // Assign new loader
        _gameLoader = std::move(newGameLoader);
        _gameInstance = newGameInstance;
        _gameInstance->init();
        _gameState = GameState::RUNNING;

        // Update the selected game index in the menu
        auto it = std::find(_gamePaths.begin(), _gamePaths.end(), selectedGame);
        if (it != _gamePaths.end())
            _gameIdx = std::distance(_gamePaths.begin(), it);

    } catch (const DLLoader<IGameModule>::DLLoaderException& e) {
        throw std::runtime_error(selectedGame + ": " + e.what());
    }
}

void arcade::Arcade::loadDisplayFromPath(const std::string& selectedGraphic)
{
    if (selectedGraphic == _displayPaths[_displayIdx]) return;
    try {
        if (_graphicInstance != nullptr) {
            delete _graphicInstance;
            _graphicInstance = nullptr;
        }

        auto newGraphicLoader = std::make_unique<DLLoader<IDisplayModule>>(selectedGraphic);
        IDisplayModule* newGraphicInstance = newGraphicLoader->getInstance();

        _graphicLoader = std::move(newGraphicLoader);
        _graphicInstance = newGraphicInstance;
        _graphicInstance->init();

        auto it = std::find(_displayPaths.begin(), _displayPaths.end(), selectedGraphic);
        if (it != _displayPaths.end())
            _displayIdx = std::distance(_displayPaths.begin(), it);
    } catch (const DLLoader<IDisplayModule>::DLLoaderException& e) {
        throw std::runtime_error(selectedGraphic + ": " + e.what());
    }
}

void arcade::Arcade::handleMenuSelection()
{
    IMenuModule* menu = dynamic_cast<IMenuModule*>(_gameInstance);

    if (!menu) return;

    std::string selectedGraphic = menu->getSelectedGraphic();
    std::string selectedGame = menu->getSelectedGame();

    _playerName = menu->getPlayerName();
    if (!selectedGraphic.empty()) {
        loadDisplayFromPath(selectedGraphic);
        menu->setDisplayIdx(_displayIdx);
    }
    if (!selectedGame.empty()) {
        loadGameFromPath(selectedGame);
        return;
    }
    _gameInstance->restart();
}

void arcade::Arcade::handleStateTransitions()
{
    GameState currentState = _gameInstance->getState();

    if (_gameState == GameState::MENU) {
        if (currentState == GameState::WON) handleMenuSelection();
    } else {
        if (currentState == GameState::GAME_OVER || currentState == GameState::WON) {
            if (_gameState == GameState::RUNNING) {
                _gameState = currentState;
                saveScoreToFile();
            }
        } else if (currentState == GameState::RUNNING)
            _gameState = GameState::RUNNING;
    }
}

void arcade::Arcade::saveScoreToFile()
{
    std::ofstream file("saves.csv", std::ios::app);

    if (!file.is_open()) return;
    if (file.tellp() == 0) file << "Player,Score,Game\n";
    file << _playerName << ","
         << _gameInstance->getScore() << ","
         << _gameInstance->getName() << "\n";
    file.close();
}

void arcade::Arcade::renderFrame()
{
    _graphicInstance->clear();
    _graphicInstance->setRenderContext(_gameInstance->getRenderContext());

    for (const auto& entity : _gameInstance->getEntities()) _graphicInstance->drawEntity(entity);

    for (const auto& text : _gameInstance->getUIText()) _graphicInstance->drawText(text);

    _graphicInstance->display();
}

void arcade::Arcade::cleanupResources()
{
    if (_gameInstance) {
        _gameInstance->stop();
        delete _gameInstance;
        _gameInstance = nullptr;
    }
    if (_graphicInstance) {
        _graphicInstance->stop();
        delete _graphicInstance;
        _graphicInstance = nullptr;
    }

    // Reset loaders
    _gameLoader.reset();
    _graphicLoader.reset();
}

void arcade::Arcade::setDisplayIdx(const std::string& initGraphicPath)
{
    int idx = 0;

    for (const auto& displayPath : _displayPaths) {
        if (displayPath == initGraphicPath) break;
        idx += 1;
    }
    _displayIdx = (idx < static_cast<int>(_displayPaths.size())) ? idx : -1;
}

int arcade::Arcade::run()
{
    while (_isRunning && _graphicInstance && _graphicInstance->isOpen()) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        if (!inputsManager()) break;

        _gameInstance->update(_graphicInstance->getDeltaTime());
        handleStateTransitions();
        renderFrame();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frameDuration = frameEnd - frameStart;

        // Cap at ~60 FPS (1000 / 60 ≈ 16.66 ms)
        if (frameDuration.count() < 16.66)
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(16.66 - frameDuration.count())));
    }

    cleanupResources();

    return 0;
}

void arcade::Arcade::printUsage(const std::string& programName) const
{
    std::cout << "\n"
              << "   █████╗ ██████╗  ██████╗ █████╗ ██████╗ ███████╗\n"
              << "  ██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝\n"
              << "  ███████║██████╔╝██║     ███████║██║  ██║█████╗  \n"
              << "  ██╔══██║██╔══██╗██║     ██╔══██║██║  ██║██╔══╝  \n"
              << "  ██║  ██║██║  ██║╚██████╗██║  ██║██████╔╝███████╗\n"
              << "  ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═════╝ ╚══════╝\n"
              << "\n"
              << "  \033[90mUSAGE\033[0m\n"
              << "    " << programName << " \033[33m<graphic_library>\033[0m\n"
              << "\n"
              << "  \033[90mEXAMPLES\033[0m\n"
              << "    " << programName << " \033[33m./lib/arcade_ncurses.so\033[0m\n"
              << "    " << programName << " \033[33m./lib/arcade_sdl2.so\033[0m\n"
              << "\n";
}
