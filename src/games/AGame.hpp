/*
**  _                                              _      ___    ___  
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / / 
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/ 
**          __/ |     on 20/03/25.                                          
**         |___/ 
*/

#ifndef AGAME_HPP
#define AGAME_HPP
#include <memory>

#include "IGame.hpp"

namespace Arcade {

class AGame : public Arcade::IGame {
public:
    AGame(): IGame() {
        map = std::make_unique<Arcade::GameMap>(1, 10, 10);
    }

    std::string getName() const override {
        return name;
    };

    Arcade::GameMap getMap() const override {
        std::cout << "sending map" << std::endl;
        return *map;
    };
    bool isGameOver() const override {
        return gameOver;
    };
    int getScore() const override {
        return static_cast<int>(score);
    };
    void reset() override = 0;
    void initMap() override = 0;
    void update(Arcade::Input userInput) override = 0;
};

} // Acarde

#endif //AGAME_HPP
