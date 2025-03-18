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

typedef struct Cell
{
    /// positions
    size_t x = -1;
    size_t y = -1;

    ///Propriétés générales
    bool isWall = false;        // Pour Pacman, Nibbler, Qix (bordures)
    bool isOccupied = false;    // Par un joueur ou un ennemi (utile pour Snake, Centipede)
    bool isVisible = true;      // Pour Qix ou Minesweeper (cases découvertes/cachées)

    /// Entités possibles
    bool hasPlayer = false;     // Snake, Pacman, Solarfox
    bool hasEnemy = false;      // Ghosts (Pacman), Qix entity, Centipede segments
    bool hasBonus = false;      // Fruits (Snake), Dots (Pacman), Power-up (Solarfox)
    bool hasProjectile = false; // Tir dans Centipede ou Solarfox

    explicit Cell(size_t _x = -1, size_t _y = -1) : x(_x), y(_y) {}
} Cell;

#endif //MY_HPP
