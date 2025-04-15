/*
**  _                                              _      ___    ___  
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / / 
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/ 
**          __/ |     on 18/03/25.                                          
**         |___/ 
*/

#ifndef CORE_HPP
#define CORE_HPP
#include <memory>

#include "../games/IGame.hpp"
#include "../graphicals/IGraphics.hpp"
#include <dlfcn.h>


class Core {
private:
    std::unique_ptr<Arcade::IGame> _game;
    std::unique_ptr<Arcade::IGraphics> _graphics;
    void* _gameHandle = nullptr;
    void* _graphicsHandle = nullptr;

    void loadGame(const std::string& libPath)
    {
        _gameHandle = dlopen(libPath.c_str(), RTLD_LAZY);
        if (!_gameHandle) throw std::runtime_error(dlerror());

        void *createGame = dlsym(_gameHandle, "createGame");
        if (!createGame) {
            dlclose(_gameHandle);
            throw std::runtime_error("Could not find symbol 'createGame' in " + libPath);
        }

        const auto gameFactory = reinterpret_cast<Arcade::IGame*(*)()>(createGame);
        if (gameFactory != nullptr)
            _game.reset(gameFactory());
        else
            throw std::runtime_error("Failed to cast createGame symbol.");
    }

    void loadGraphics(const std::string& libPath)
    {
        _graphicsHandle = dlopen(libPath.c_str(), RTLD_LAZY);
        if (!_graphicsHandle) throw std::runtime_error(dlerror());

        void *createGraphics = dlsym(_graphicsHandle, "createGraphics");
        if (!createGraphics) {
            dlclose(_graphicsHandle);
            throw std::runtime_error("Could not find symbol 'createGraphics' in " + libPath);
        }

        const auto graphicsFactory = reinterpret_cast<Arcade::IGraphics*(*)()>(createGraphics);
        if (graphicsFactory != nullptr)
            _graphics.reset(graphicsFactory());
        else
            throw std::runtime_error("Failed to cast createGraphics symbol.");
    }
public:
    Core(const std::string& gamePath,  const std::string& graphPath){
        //(void)gamePath;
        loadGame(gamePath);
        //(void)graphPath;
        //std::cout << "game is loaded" << std::endl;
        loadGraphics(graphPath);
        //std::cout << "graphics is loaded" << std::endl;
    }

    ~Core() {
        _game.reset();
        _graphics.reset();
        if (_gameHandle)     dlclose(_gameHandle);
        if (_graphicsHandle) dlclose(_graphicsHandle);
    }

    void changeLib(const std::string& lib) {
        if (lib == "graphics") {
            if (_graphicsHandle) {
                _graphics.reset();
                dlclose(_graphicsHandle);
            }
            _gameHandle = nullptr;
            const std::string nextGraphicsPath = getNextGraphics();
            std::cout << nextGraphicsPath << std::endl;
            loadGraphics(nextGraphicsPath);
        }
        if (lib == "games") {
            if (_gameHandle) {
                _game.reset();
                dlclose(_gameHandle);
            }
            _gameHandle = nullptr;
            const std::string nextGamesPath = getNextGame();
            loadGame(nextGamesPath);
        }
    }

    void run()
    {
        if (!_game)     throw std::runtime_error("Could not load Game");
        if (!_graphics) throw std::runtime_error("Could not load Graphics library");

        _game->initMap();
        while (true) {
            auto user_input = Arcade::Input::NONE;
            user_input = _graphics->getInput();
            if (user_input == Arcade::Input::ESCAPE) break;
            if (user_input == Arcade::Input::SWITCH_LIB) changeLib("graphics");
            if (user_input == Arcade::Input::SWITCH_GAME) changeLib("game");
            _game->update(user_input);
            _graphics->draw(_game->getMap());
        }
    }
};



#endif //CORE_HPP
