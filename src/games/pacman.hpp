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
#include <chrono>
#include <iostream>

#include "AGame.hpp"
#include "../../includes/my.hpp"

#define FILEPATH_LEVEL_1 "./lib/pacman_assets/level1.map"
namespace Arcade
{
    typedef struct position {
        size_t x;
        size_t y;
    } position;

    enum class GhostState {
        HUNTER,      // Poursuite
        SCATTER,     // Dispersion random
        FEARFUL,     // Mode effrayé (fuire bigPacman)
        EATEN        // Retour à la maison
    };

    class Ghost {
    private:
        position pos;
        GhostState state_ = GhostState::SCATTER;
        EntityType lastCellEntity = EntityType::EMPTY;
        std::chrono::steady_clock::time_point fearStartTime;
        std::chrono::steady_clock::time_point huntStartTime;
        bool isFearful = false;
        bool isHunter = false;
        bool atHome = true;

        void updateState() {
            const auto now = std::chrono::steady_clock::now();
            if (isFearful) {
                const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - fearStartTime).count();
                if (elapsed >= 10) {
                    state_ = GhostState::SCATTER;
                    isFearful = false;
                }
            }
            if (isHunter) {
                const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - huntStartTime).count();
                if (elapsed >= 5) {
                    state_ = GhostState::SCATTER;
                    isHunter = false;
                }
            }
        }

        void move(position new_pos, Cell *new_cell, GameMap *map) {
            map->getCell(pos.x, pos.y)->entity = lastCellEntity;
            lastCellEntity = new_cell->entity;
            new_cell->entity = EntityType::ENEMY;
            pos = new_pos;
        }
    public:
        Ghost() : pos(1, 2) {}

        position getPosition() const {
            return pos;
        }

        void move_random(GameMap *map)
        {
            // Direction actuelle, sauvegardée statiquement entre les appels
            static int current_dir = random(1, 4);

            for (int attempts = 0; attempts < 4; ++attempts) {
                position new_pos = pos;

                switch (current_dir) {
                    case 1: new_pos.y--; break; // Haut
                    case 2: new_pos.x++; break; // Droite
                    case 3: new_pos.x--; break; // Gauche
                    case 4: new_pos.y++; break; // Bas
                    default: break;
                }

                Cell *new_cell = map->getCell(new_pos.x, new_pos.y);

                // Si la cellule est valide, on bouge
                if (new_cell && new_cell->entity != Arcade::EntityType::WALL && new_cell->entity != Arcade::EntityType::ENEMY) {
                    move(new_pos, new_cell, map);
                    return;
                }
                // Sinon, on choisit une nouvelle direction au hasard
                current_dir = random(1, 4);
            }
        }

        void move_chasse(GameMap *map, position player)
        {
            const int dx = (player.x > pos.x) ? 1 : (player.x < pos.x) ? -1 : 0;
            const int dy = (player.y > pos.y) ? 1 : (player.y < pos.y) ? -1 : 0;

            bool move_in_x = (std::rand() % 2 == 0);

            for (int i = 0; i < 2; ++i) {
                if (move_in_x && dx != 0) {
                    const size_t new_x = pos.x + dx;
                    Cell *new_cell = map->getCell(new_x, pos.y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL) {
                        move({new_x, pos.y}, new_cell, map);
                        return;
                    }
                }
                if (!move_in_x && dy != 0) {
                    const size_t new_y = pos.y + dy;
                    Cell *new_cell = map->getCell(pos.x, new_y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL) {
                        move({pos.x, new_y}, new_cell, map);
                        return;
                    }
                }
                move_in_x = !move_in_x;
            }
        }

        void move_afraid(GameMap *map, position player)
        {
            const int dx = (player.x > pos.x) ? -1 : (player.x < pos.x) ? 1 : 0;
            const int dy = (player.y > pos.y) ? -1 : (player.y < pos.y) ? 1 : 0;

            bool move_in_x = (std::rand() % 2 == 0);

            for (int i = 0; i < 2; ++i) {
                if (move_in_x && dx != 0) {
                    const size_t new_x = pos.x + dx;
                    Cell *new_cell = map->getCell(new_x, pos.y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL && new_cell->entity != Arcade::EntityType::ENEMY) {
                        move({new_x, pos.y}, new_cell, map);
                        return;
                    }
                }
                if (!move_in_x && dy != 0) {
                    const size_t new_y = pos.y + dy;
                    Cell *new_cell = map->getCell(pos.x, new_y);
                    if (new_cell && new_cell->entity != Arcade::EntityType::WALL && new_cell->entity != Arcade::EntityType::ENEMY) {
                        move({pos.x, new_y}, new_cell, map);
                        return;
                    }
                }
                move_in_x = !move_in_x;
            }
            move_random(map);
        }

        void setState(GhostState state) {
            state_ = state;
            if (state_ == GhostState::FEARFUL) {
                fearStartTime = std::chrono::steady_clock::now();
                isFearful = true;
            }
            if (state_ == GhostState::HUNTER) {
                huntStartTime = std::chrono::steady_clock::now();
                isHunter = true;
            }
        };

        void quitHome() {
            atHome = false;
        }

        void returnAtHome() {
            atHome = true;
        }

        void setPosition(position new_pos) {
            pos = new_pos;
        }

        void update(GameMap *map, position player) {
            if (atHome)
                return;
            if (state_ == GhostState::HUNTER)
                move_chasse(map, player);
            if (state_ == GhostState::SCATTER)
                move_random(map);
            if (state_ == GhostState::FEARFUL)
                move_afraid(map, player);
        }

        GhostState getState() const {
            return state_;
        }
    };


    class Pacman {
    private:
        position pos = {};
        bool bigPacman = false;
        size_t lives = 3;
        size_t score = 0;
        std::chrono::steady_clock::time_point bigPacmanStartTime;

        // void move(position new_pos, Cell *new_cell, GameMap *map) {
        //     map->getCell(pos.x, pos.y)->entity = lastCellEntity;
        //     lastCellEntity = new_cell->entity;
        //     new_cell->entity = EntityType::ENEMY;
        //     pos = new_pos;
        // }
        void move(position new_pos, Cell *new_cell, GameMap *map) {
            map->getCell(pos.x, pos.y)->entity = EntityType::EMPTY;
            pos = new_pos;
            new_cell->entity = EntityType::PLAYER;
        }
    public:

        void losesLives() {
            lives--;
        }

        void addScores(size_t s) {
            score += s;
        }

        void setPosition(position new_pos) {
            pos = new_pos;
        }

        size_t getLives() const{
            return lives;
        }

        position getPosition() const {
            return pos;
        }

        bool ifBigPacman() const {
            return bigPacman;
        }

        size_t getScore() const {
            return score;
        }

        void update(Arcade::Input userInput, GameMap *map, Arcade::Input &lastInput)
        {
            const auto now = std::chrono::steady_clock::now();
            if (bigPacman) {
                const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - bigPacmanStartTime).count();
                if (elapsed >= 10) {
                    bigPacman = false;
                }
            }
            position new_pos = pos;

            if (userInput == Arcade::Input::NONE)
                userInput = lastInput;
            switch (userInput) {
                case Arcade::Input::UP:    new_pos.y--; break;
                case Arcade::Input::DOWN:  new_pos.y++; break;
                case Arcade::Input::LEFT:  new_pos.x--; break;
                case Arcade::Input::RIGHT: new_pos.x++; break;
                default: return;
            }

            Cell *new_cell = map->getCell(new_pos.x, new_pos.y);
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
                bigPacman = true;
                score += 10;
                bigPacmanStartTime = std::chrono::steady_clock::now();
            }
            move(new_pos, new_cell, map);
        }

    };

    class GamePacman final : public AGame {
    private:
        Arcade::Input lastInput = Arcade::Input::LEFT;
        Pacman player;
        Ghost ghosts[4];
        std::chrono::steady_clock::time_point lastPlayerMove = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point lastGhostMove = std::chrono::steady_clock::now();


    public:
        GamePacman(): AGame() {
            name = "Pacman";
            map->setImagePathsDirectory("./lib/pacman_assets/img/");
        };
        void reset() override{
            return;
        };

        static bool isCollision(const position &a, const position &b) {
            return a.x == b.x && a.y == b.y;
        }

        void initMap() override
        {
            map->chargeMap(FILEPATH_LEVEL_1);
            map->afficherMap();
            size_t ghostIndex = 0;
            bool pacmanPlaced = false;

            for (size_t i = 0; i < map->getHeight(); ++i) {
                for (size_t j = 0; j < map->getWidth(); ++j) {
                    const Arcade::Cell* cell = map->getCell(j, i);
                    if (!cell) continue;
                    if (cell->entity == Arcade::EntityType::PLAYER && !pacmanPlaced) {
                        player.setPosition({j, i});
                        pacmanPlaced = true;
                    }
                    if (cell->entity == Arcade::EntityType::ENEMY && ghostIndex < 4) {
                        ghosts[ghostIndex].setPosition({j, i});
                        ghostIndex++;
                    }
                }
            }
            if (!pacmanPlaced)
                std::cerr << "Erreur: Pac-Man n'a pas été placé sur la carte !" << std::endl;
            if (ghostIndex < 4)
                std::cerr << "Avertissement: Seulement " << ghostIndex << " fantômes placés sur 4 !" << std::endl;
            for (auto &ghost : ghosts)
                ghost.quitHome();
        }

        void update(Arcade::Input userInput) override
        {
            const auto now = std::chrono::steady_clock::now();
            bool playerMoved = false;

            if (userInput != Arcade::Input::NONE || std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPlayerMove).count() >= 500) {
                player.update(userInput, map.get(), lastInput);
                lastPlayerMove = now;
                playerMoved = true;
            }
            const position playerPos = player.getPosition();

            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastGhostMove).count() >= 300) {
                for (auto &ghost : ghosts) {
                    if (player.ifBigPacman())
                        ghost.setState(GhostState::FEARFUL);

                    ghost.update(map.get(), playerPos);

                    if (isCollision(playerPos, ghost.getPosition())) {
                        if (ghost.getState() == GhostState::FEARFUL) {
                            ghost.setState(GhostState::EATEN);
                            ghost.returnAtHome();
                            player.addScores(200);
                        } else if (ghost.getState() != GhostState::EATEN) {
                            player.losesLives();
                            reset();
                            if (player.getLives() <= 0)
                                gameOver = true;
                            return;
                        }
                    }
                }
                lastGhostMove = now;
            }
            map->setLives(player.getLives());
            map->setScore(player.getScore());
            // map->afficherMap();
            if (playerMoved) std::cout << "Updating Pacman" << std::endl;
        }
    };
} // Acarde

#endif //PACMAN_HPP
