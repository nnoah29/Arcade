/*
**  _                                              _      ___    ___  
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / / 
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/ 
**          __/ |     on 17/03/25.                                          
**         |___/ 
*/

#ifndef IGAME_HPP
#define IGAME_HPP
#include <string>
#include <vector>

#include "../../includes/gameMap.hpp"
#include  "../../includes/my.hpp"


namespace Arcade {

    class IGame {
    protected:
        std::unique_ptr<GameMap> map;
        std::string name;
        bool playing = false;
        bool gameWon = false;
        bool gameOver = false;
        size_t score = 0, level = 0;
        size_t mapHeight = 0, mapWidth = 0;
    public:
        virtual ~IGame() = default;
        virtual void reset() = 0;
        virtual void initMap() = 0;
        virtual void update(Input userInput) = 0;
        virtual GameMap getMap() const = 0;
        virtual bool isGameOver() const = 0;
        virtual int getScore() const = 0;
        virtual std::string getName() const = 0;
    };

} // namespace Arcade

#endif //IGAME_HPP
