#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "IGame.hpp"
#include <deque>
#include <random>
#include <chrono>

namespace Arcade {

    struct SnakePart {
        size_t x;
        size_t y;
    };

    class Snake : public IGame {
        private:
            static constexpr size_t DEFAULT_WIDTH = 20;
            static constexpr size_t DEFAULT_HEIGHT = 20;
            static constexpr size_t INITIAL_SNAKE_SIZE = 4;
            static constexpr size_t INITIAL_SPEED = 200;
            std::deque<SnakePart> m_snake;
            Input m_direction;
            Input m_nextDirection;

            SnakePart m_food;

            std::chrono::time_point<std::chrono::steady_clock> m_lastUpdateTime;
            size_t m_updateInterval;

            std::mt19937 m_rng;

            void moveSnake();
            void spawnFood();
            bool checkCollision(size_t x, size_t y) const;
            void updateMap();

        public:
            Snake();
            ~Snake() override = default;

            void reset() override;
            void initMap() override;
            void update(Input userInput) override;
            GameMap getMap() const override; 
            bool isGameOver() const override;
            int getScore() const override;
            std::string getName() const override;
        };

}

#endif // SNAKE_HPP