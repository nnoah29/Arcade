#include "Snake.hpp"

extern "C" {
    Arcade::IGame* createGame() {
        return new Arcade::Snake();
    }
}