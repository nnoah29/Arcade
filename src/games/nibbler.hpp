#ifndef NIBBLER_HPP
#define NIBBLER_HPP

#include <deque>
#include <random>
#include <chrono>
#include "IGame.hpp"
#include <memory>

namespace Arcade {

    struct NibblerPart {
        size_t x;
        size_t y;
    };

    class Nibbler : public IGame {
    private:
        static constexpr size_t DEFAULT_WIDTH = 20;
        static constexpr size_t DEFAULT_HEIGHT = 20;
        static constexpr size_t INITIAL_NIBBLER_SIZE = 4;
        static constexpr size_t INITIAL_SPEED = 200;
        
        std::deque<NibblerPart> m_nibbler;
        Input m_direction;
        Input m_nextDirection;

        NibblerPart m_food;

        std::chrono::time_point<std::chrono::steady_clock> m_lastUpdateTime;
        size_t m_updateInterval;

        std::mt19937 m_rng;

        void moveNibbler();
        void spawnFood();
        bool checkCollision(size_t x, size_t y) const;
        void updateMap();
        
    public:
        Nibbler();
        ~Nibbler() override = default;

        void reset() override;
        void initMap() override;
        void update(Input userInput) override;
        GameMap getMap() const override;
        bool isGameOver() const override;
        int getScore() const override;
        std::string getName() const override;
    };
}

#endif