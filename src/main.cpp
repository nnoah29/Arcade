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
#include <iostream>

#include <iostream>
#include <vector>

// Taille de la map
const int WIDTH = 10;
const int HEIGHT = 10;

// Struct pour chaque cellule
struct Cell {
    bool isWall = false;
    bool hasPlayer = false;
    bool hasGhost = false;
    bool hasDot = false;
};

// Création de la map
std::vector<std::vector<Cell>> createMap() {
    std::vector<std::vector<Cell>> map(HEIGHT, std::vector<Cell>(WIDTH));

    // Exemple : on crée un contour de murs
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                map[y][x].isWall = true;
            } else {
                map[y][x].hasDot = true; // des pastilles partout au début
            }
        }
    }

    // Pacman au centre
    map[5][5].hasPlayer = true;
    map[5][5].hasDot = false;

    return map;
}

// Fonction d'affichage simple
void draw(const std::vector<std::vector<Cell>>& map) {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (map[y][x].isWall) {
                std::cout << "#";
            } else if (map[y][x].hasPlayer) {
                std::cout << "P";
            } else if (map[y][x].hasGhost) {
                std::cout << "G";
            } else if (map[y][x].hasDot) {
                std::cout << ".";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

int main() {
    auto map = createMap();
    draw(map);
    return 0;
}

