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
#include <string>
#include <map>
#include <fstream>

#include "my.hpp"
namespace Arcade
{
    enum class EntityType {
        EMPTY,      // Case vide (ex: terrain par défaut)
        WALL,       // Mur ou obstacle
        PLAYER,     // Joueur (Serpent, Pacman, Vaisseau, etc.)
        ENEMY,      // Ennemi (Fantôme, Centipede, etc.)
        BONUS,      // Bonus ou point à collecter (Fruits, PacGum, etc.)
        BIG_BONUS,
        PROJECTILE, // Projectile tiré (Solarfox, Centipede)
        HIDDEN,     // Case cachée (Minesweeper, Qix)
        BORDER,      // Délimitation spécifique (Qix)
        SNAKE_HEAD,     // Tête du serpent
        SNAKE_BODY, // Corps du serpent
    };

    typedef struct Cell {
        size_t x = -1;
        size_t y = -1;
        EntityType entity = EntityType::EMPTY;

        explicit Cell() : x(0), y(0), entity(EntityType::EMPTY) {}
        explicit Cell(size_t _x, size_t _y , EntityType type) : x(_x), y(_y), entity(type) {}
    } Cell;

    class GameMap {
        protected:
            size_t level;
            size_t width;
            size_t height;
            std::vector<std::vector<Cell>> map;

            int score = 0;
            int highScore = 0;
            int lives = 3;
            int timeLeft = 0;
            std::string message = "";
            bool gameOver = false;
            std::map<std::string, bool> flags;

        public:
            GameMap(size_t level, size_t width, size_t height) : level(level), width(width), height(height) {
                reset();
            }

            Cell* getCell(size_t x, size_t y) {
                if (y < height && x < width)
                    return &map[y][x];
                return nullptr;
            }

            size_t getHeight() const {
                return height;
            }

            size_t getWidth() const {
                return width;
            }

            const std::vector<std::vector<Cell>>& getCell() const {
                return map;
            }

            void reset()
            {
                map.clear();
                map.resize(height, std::vector<Cell>(width));

                for (size_t y = 0; y < height; ++y) {
                    for (size_t x = 0; x < width; ++x) {
                        map[y][x] = Cell(x, y, EntityType::EMPTY);
                    }
                }
            }

            void chargeMap(const std::string& filepath)
            {
                std::ifstream file(filepath);
                if (!file.is_open()) throw std::runtime_error("Impossible d'ouvrir le fichier de la map.");

                map.clear();
                std::string line;
                size_t y = 0;
                while (std::getline(file, line)) {
                    std::vector<Cell> row;
                    for (size_t x = 0; x < line.size(); ++x) {
                        auto type = EntityType::EMPTY;
                        switch (line[x]) {
                            case '#': type = EntityType::WALL; break;
                            case 'P': type = EntityType::PLAYER; break;
                            case 'E': type = EntityType::ENEMY; break;
                            case 'B': type = EntityType::BONUS; break;
                            case 'X': type = EntityType::PROJECTILE; break;
                            case '?': type = EntityType::HIDDEN; break;
                            case '|': type = EntityType::BORDER; break;
                            default:  type = EntityType::EMPTY; break;
                        }
                        row.emplace_back(x, y, type);
                    }
                    map.push_back(row);
                    y++;
                }
            }


            bool hasScore() const { return score > 0; }
            int getScore() const { return score; }
            void setScore(int _score) { score = _score; }

            bool hasHighScore() const { return highScore > 0; }
            int getHighScore() const { return highScore; }
            void setHighScore(int _highScore) { highScore = _highScore; }

            bool hasLevel() const { return true; }
            size_t getLevel() const { return level; }
            void setLevel(size_t _level) { level = _level; }

            bool hasLives() const { return lives > 0; }
            int getLives() const { return lives; }
            void setLives(int _lives) { lives = _lives; }

            bool hasTimeLeft() const { return timeLeft > 0; }
            int getTimeLeft() const { return timeLeft; }
            void setTimeLeft(int _timeLeft) { timeLeft = _timeLeft; }

            bool hasMessage() const { return !message.empty(); }
            std::string getMessage() const { return message; }
            void setMessage(const std::string& _message) { message = _message; }

            bool isGameOver() const { return gameOver; }
            void setGameOver(bool _gameOver) { gameOver = _gameOver; }

            bool hasFlag(const std::string& flag) const {
                auto it = flags.find(flag);
                return it != flags.end() && it->second;
            }
            
            void setFlag(const std::string& flag, bool value = true) { flags[flag] = value; }
    };
}

#endif //GAMEMAP_HPP


/*
 * # Documentation de la Classe GameMap

## 1. Introduction
La classe `GameMap` est une abstraction générique permettant de représenter et de gérer une carte de jeu dans l'architecture du projet Arcade. Elle fournit une structure modulaire pour stocker les éléments du jeu sous forme d'une grille de cellules (`Cell`), facilitant ainsi la compatibilité entre différents types de jeux.

## 2. Prérequis
- un fichier de configuration de carte (`level1.txt`) contenant les caractères représentant les différents types d'entités.
-

## 3. Les Composants de GameMap
### 3.1. `EntityType`
Un `enum class` permettant de catégoriser les types d'entités possibles sur la carte :

| Valeur      | Description                              |
|-------------|------------------------------------------|
| `EMPTY`     | Case vide                                |
| `WALL`      | Mur ou obstacle                          |
| `PLAYER`    | Joueur (Pacman, Serpent, Vaisseau, etc.) |
| `ENEMY`     | Ennemi (Fantômes, Centipede, etc.)       |
| `BONUS`     | Objet collectable (PacGum, Fruits, etc.) |
| `PROJECTILE`| Projectile tiré (Solarfox, Centipede)    |
| `HIDDEN`    | Case cachée (Minesweeper, Qix)           |
| `BORDER`    | Bordure spécifique (Qix)                 |

### 3.2. `Cell`
Une structure représentant une case de la carte avec :
- **Coordonnées (`x, y`)** : Position de la cellule dans la grille.
- **Type d'entité (`entity`)** : Contenu de la cellule sous forme d'un `EntityType`.

### 3.3. `GameMap`
Une classe qui stocke et gère la carte du jeu :
- **Attributs :**
  - `size_t level` : Niveau actuel du jeu.
  - `size_t width, height` : Dimensions de la carte.
  - `std::vector<std::vector<Cell>> map` : Grille contenant les cellules.

- **Méthodes :**
  - `GameMap(size_t level, size_t width, size_t height)` : Constructeur initialisant la carte avec une taille spécifique.
  - `Cell* getCell(int x, int y)` : Retourne un pointeur vers la cellule aux coordonnées `(x, y)` si elle est valide, sinon `nullptr`.
  - `void reset()` : Réinitialise la carte avec des cellules vides (`EntityType::EMPTY`).
  - `void chargeMap(const std::string& filepath)` : Charge une carte depuis un fichier texte en associant les caractères aux entités correspondantes.

## 4. Fonctionnement Global
1. **Initialisation** : Lorsqu'un jeu commence, une instance de `GameMap` est créée avec une taille définie.
2. **Mise à jour** : Chaque cycle de jeu peut modifier la carte (ajout/suppression d'entités).
3. **Affichage** : Une bibliothèque graphique interagit avec `GameMap` pour dessiner la scène.
4. **Réinitialisation** : Lorsqu'un niveau change ou qu'une partie redémarre, `reset()` est appelée.
5. **Chargement de fichier** : `chargeMap()` permet de récupérer une configuration de carte à partir d'un fichier externe.

## 6. Conclusion
La classe `GameMap` est conçue pour être **modulaire**, **flexible** et **réutilisable** dans plusieurs jeux. Grâce à cette abstraction, les jeux peuvent gérer leurs cartes dynamiquement, tout en restant compatibles avec la bibliothèque graphique qui interprétera la carte sans connaître le jeu spécifique.


 */