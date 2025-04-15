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

#ifndef MY_HPP
#define MY_HPP

#include <memory>
#include <fstream>

namespace Arcade
{
    enum class Input {
        UP, DOWN, LEFT, RIGHT, ENTER,
        BACK, SWITCH_GAME, SWITCH_LIB,
        RESTART, EXIT, MENU, NONE, ESCAPE
    };
}

std::string getNextGame();
std::string getNextGraphics();

int random(int a, int b);

#endif //MY_HPP
