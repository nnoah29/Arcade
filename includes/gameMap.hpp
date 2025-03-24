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

namespace arcade {

    class GameMap {
        private:
            size_t level;
            size_t width;
            size_t height;
            std::vector<std::vector<Cell>> cells;
        public:
        
            GameMap(size_t level, size_t width, size_t height) : level(level), width(width), height(height) {
                cells.resize(height);
                for (auto& row : cells) {
                    row.resize(width);
                }
            }
        
            Cell* getCell(int x, int y) {
                if (y >= 0 && y < static_cast<int>(height) && x >= 0 && x < static_cast<int>(width))
                    return &cells[y][x];
                return nullptr;
            }
        
            void reset() {
                for (size_t y = 0; y < height; ++y)
                    for (size_t x = 0; x < width; ++x)
                        cells[y][x] = Cell(x, y);
            }
        
            void chargeMap(const std::string& filepath)
            {
                (void)filepath;
                level = 1;
            }

            std::vector<std::vector<Cell>>& getCells() { return cells; }
            const std::vector<std::vector<Cell>>& getCells() const { return cells; }
            
        };
}


#endif //GAMEMAP_HPP