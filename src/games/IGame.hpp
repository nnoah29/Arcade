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

    enum Input {
        UP, DOWN, LEFT, RIGHT, ENTER,
        BACK, SWITCH_GAME, SWITCH_LIB,
        RESTART, EXIT, MENU, NONE,
    };

    class IGame {
        protected:
            arcade::GameMap map;
            std::string name;
            bool playing;
            bool gameWon;
            bool gameOver;
            size_t score;
            size_t level;
            size_t mapHeight;
            size_t mapWidth;
        public:
            IGame(size_t level = 0, size_t width = 30, size_t height = 20)
                : map(level, width, height)
                , name("")
                , playing(true)
                , gameWon(false)
                , gameOver(false)
                , score(0)
                , level(1)
                , mapHeight(height)
                , mapWidth(width)
            {}
            virtual ~IGame() = default;
            virtual void reset() = 0;
            virtual void initMap() = 0;
            virtual void update(Input userInput) = 0;
            virtual arcade::GameMap getMap() const = 0;
            virtual bool isGameOver() const = 0;
            virtual int getScore() const = 0;
            virtual std::string getName() const = 0;
    };

} // namespace Arcade

#endif //IGAME_HPP