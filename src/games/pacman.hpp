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

#ifndef PACMAN_HPP
#define PACMAN_HPP
#include <iostream>

#include "AGame.hpp"
#include "../../includes/my.hpp"

#define FILEPATH_LEVEL_1 "no define"
namespace Arcade
{

    enum class GhostState {
        HUNTER,      // Poursuite
        SCATTER,     // Dispersion random
        FEARFUL,     // Mode effrayé (fuire bigPacman)
        EATEN        // Retour à la maison
    };

    class Ghost {
    private:
        size_t x, y;
        size_t homeX, homeY;
        GhostState state_ = GhostState::SCATTER;
        EntityType lastCellEntity = EntityType::EMPTY;

        void move(size_t new_x, size_t new_y, Cell *new_cell, GameMap *map) {
            map->getCell(x, y)->entity = lastCellEntity;
            lastCellEntity = new_cell->entity;
            new_cell->entity = EntityType::ENEMY;
            x = new_x, y = new_y;
        }
    public:
        Ghost(size_t x, size_t y) : x(0), y(0), homeX(x), homeY(y) {
            x = homeX, y = homeY;
        }

        void move_random(GameMap *map)
        {
            while (true) {
                const int dir = random(1, 4);
                size_t new_x = x, new_y = y;
                switch (dir) {
                    case 1:    new_y--; break;
                    case 2:    new_x++; break;
                    case 3:    new_x--; break;
                    case 4:    new_y++; break;
                    default: ;
                }
                Cell *new_cell = map->getCell(new_x, new_y);
                if (!new_cell) continue;
                if (new_cell->entity == Arcade::EntityType::WALL) continue;
                if (new_cell->entity == Arcade::EntityType::ENEMY) continue;
                move(new_x, new_y, new_cell, map);
                break;
            }
        }

        void move_chasse(GameMap *map, size_t player_x, size_t player_y)
        {
            const int dx = (player_x > x) ? 1 : (player_x < x) ? -1 : 0;
            const int dy = (player_y > y) ? 1 : (player_y < y) ? -1 : 0;

            bool move_in_x = (std::rand() % 2 == 0);

            for (int i = 0; i < 2; ++i) {
                if (move_in_x && dx != 0) {
                    const size_t new_x = x + dx;
                    Cell *new_cell = map->getCell(new_x, y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL) {
                        move(new_x, y, new_cell, map);
                        return;
                    }
                }
                if (!move_in_x && dy != 0) {
                    const size_t new_y = y + dy;
                    Cell *new_cell = map->getCell(x, new_y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL) {
                        move(x, new_y, new_cell, map);
                        return;
                    }
                }
                move_in_x = !move_in_x;
            }
        }

        void move_afraid(GameMap *map, size_t player_x, size_t player_y)
        {
            const int dx = (player_x > x) ? -1 : (player_x < x) ? 1 : 0;
            const int dy = (player_y > y) ? -1 : (player_y < y) ? 1 : 0;

            bool move_in_x = (std::rand() % 2 == 0);

            for (int i = 0; i < 2; ++i) {
                if (move_in_x && dx != 0) {
                    const size_t new_x = x + dx;
                    Cell *new_cell = map->getCell(new_x, y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL) {
                        move(new_x, y, new_cell, map);
                        return;
                    }
                }
                if (!move_in_x && dy != 0) {
                    const size_t new_y = y + dy;
                    Cell *new_cell = map->getCell(x, new_y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL) {
                        move(x, new_y, new_cell, map);
                        return;
                    }
                }
                move_in_x = !move_in_x;
            }
            move_random(map);
        }

        void setState(GhostState state) {
            state_ = state;
        };

        void setHome(size_t _x, size_t _y) {
            homeX = _x, homeY = _y;
            x = _x, y = _y;
        }

        void move_(GameMap *map, size_t player_x, size_t player_y)
        {
            if (state_ == GhostState::HUNTER)
                move_chasse(map, player_x, player_y);
            if (state_ == GhostState::SCATTER)
                move_random(map);
            if (state_ == GhostState::FEARFUL)
                move_afraid(map, player_x, player_y);
        }
    };
    class pacman final : public AGame {
    private:
        size_t x, y;
        bool bigPacman;
        size_t lives = 3;
        Arcade::Input lastInput = Arcade::Input::LEFT;
        Ghost ghosts[4] = {Ghost(47895, 78956), Ghost(47895, 78956), Ghost(47895, 78956), Ghost(47895, 78956)};

        void move(size_t new_x, size_t new_y, Cell *new_cell) {
            map->getCell(x, y)->entity = EntityType::EMPTY;
            x = new_x, y = new_y;
            new_cell->entity = EntityType::PLAYER;
        }
    public:
        pacman(): AGame() {
            name = "Pacman";
        };
    public:
        void reset() override{
            return;
        };

        void initMap() override
        {
            map->chargeMap(FILEPATH_LEVEL_1);
            size_t ghostIndex = 0;
            bool pacmanPlaced = false;

            for (size_t i = 0; i < map->getHeight(); ++i) {
                for (size_t j = 0; j < map->getWidth(); ++j) {
                    const Arcade::Cell* cell = map->getCell(j, i);
                    if (!cell) continue;
                    if (cell->entity == Arcade::EntityType::PLAYER && !pacmanPlaced) {
                        x = j;
                        y = i;
                        pacmanPlaced = true;
                    }
                    if (cell->entity == Arcade::EntityType::ENEMY && ghostIndex < 4) {
                        ghosts[ghostIndex].setHome(j, i);
                        ghostIndex++;
                    }
                }
            }
            if (!pacmanPlaced)
                std::cerr << "Erreur: Pac-Man n'a pas été placé sur la carte !" << std::endl;
            if (ghostIndex < 4)
                std::cerr << "Avertissement: Seulement " << ghostIndex << " fantômes placés sur 4 !" << std::endl;
        }


        void move_pacman(Arcade::Input userInput)
        {
            size_t new_x = x, new_y = y;

            if (userInput == Arcade::Input::NONE)
                userInput = lastInput;
            switch (userInput) {
                case Arcade::Input::UP:    new_y--; break;
                case Arcade::Input::DOWN:  new_y++; break;
                case Arcade::Input::LEFT:  new_x--; break;
                case Arcade::Input::RIGHT: new_x++; break;
                default: return;
            }

            Cell *new_cell = map->getCell(new_x, new_y);
            if (!new_cell) return;
            if (new_cell->entity == Arcade::EntityType::WALL) return;
            lastInput = userInput;

            if (new_cell->entity == Arcade::EntityType::ENEMY) {
                if (bigPacman)
                    score += 200;
                else
                    lives--;
            }
            if (new_cell->entity == Arcade::EntityType::BONUS)
                score += 10;
            if (new_cell->entity == Arcade::EntityType::BIG_BONUS) {
                score += 10;
                bigPacman = true;
            }
            move(new_x, new_y, new_cell);
        }

        void update(Arcade::Input userInput) override
        {
            move_pacman(userInput);
            if (lives <= 0)
                gameOver = true;

            std::cout << "Updating Pacman" << std::endl;
            (void)userInput;
        };

    };
}// Acarde

#endif //PACMAN_HPP
