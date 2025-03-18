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

#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP
#include <vector>

#include "my.hpp"


class GameMap {
protected:
    size_t level;
    size_t width;
    size_t height;
    std::vector<std::vector<Cell>> map;
public:

    GameMap(size_t level, size_t width, size_t height) : level(level), width(width), height(height) {}

    Cell* getCell(int x, int y) {
        if (y >= 0 && y < height && x >= 0 && x < width)
            return &map[y][x];
        return nullptr;
    }

    void reset() {
        for (size_t y = 0; y < height; ++y)
            for (size_t x = 0; x < width; ++x)
                map[y][x] = Cell(x, y);
    }

    void chargeMap(const std::string& filepath)
    {
        level = 1;
        (void)map;
    }
};



#endif //GAMEMAP_HPP
