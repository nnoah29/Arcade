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
#include <stdexcept>


class Core {
private:
    std::unique_ptr<Arcade::IGame> _game;
    std::unique_ptr<Arcade::IGraphics> _graphics;

    void loadGame(std::string libPath)
    {
        void *createGraphics = nullptr;
        void *createGame = nullptr;
        void *handle = dlopen(libPath.c_str(), RTLD_LAZY);

        if (!handle) throw std::runtime_error(dlerror());

        createGame = dlsym(handle, "createGame");
        if (createGame) {
            const auto gameFactory = reinterpret_cast<Arcade::IGame*(*)()>(createGame);
            if (gameFactory)
                _game.reset(gameFactory());
            else
                throw std::runtime_error("Failed to cast createGame symbol.");
            dlclose(handle);
            return;
        }

        createGraphics = dlsym(handle, "createGraphics");
        if (createGraphics) {
            const auto graphicsFactory = reinterpret_cast<Arcade::IGraphics*(*)()>(createGraphics);
            if (graphicsFactory)
                _graphics.reset(graphicsFactory());
            else
                throw std::runtime_error("Failed to cast createGraphics symbol.");
            dlclose(handle);
            return;
        }
        dlclose(handle);
        throw std::runtime_error("Could not find symbol '" + std::string(libPath) + "'");
    }
public:
    Core(const std::string& libPath, int magic_number){
        (void)magic_number;
        loadGame(libPath);
    }

    void run() const
    {
        if (!_game)     throw std::runtime_error("Could not load Game");
        if (!_graphics) throw std::runtime_error("Could not load Graphics library");

        while (true) {
            auto user_input = Arcade::Input::NONE;
            user_input = _graphics->getInput();
            _game->update(user_input);
            _graphics->draw(_game->getMap());

        }
    }
};



#endif //CORE_HPP
