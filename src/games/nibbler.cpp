#include "nibbler.hpp"
#include <algorithm>
#include <iostream>

namespace Arcade {
    Nibbler::Nibbler() 
        : IGame()
    {
        name = "Nibbler";
        score = 0;
        level = 1;
        mapWidth = DEFAULT_WIDTH;
        mapHeight = DEFAULT_HEIGHT;
        
        m_direction = Arcade::Input::RIGHT;
        m_nextDirection = Arcade::Input::RIGHT;
        m_food = {0, 0};
        m_updateInterval = INITIAL_SPEED;
        m_rng = std::mt19937(std::random_device{}());
        
        reset();
    }

    void Nibbler::reset()
    {
        gameOver = false;
        gameWon = false;
        playing = true;
        score = 0;
        level = 1;
        
        mapWidth = DEFAULT_WIDTH;
        mapHeight = DEFAULT_HEIGHT;
        
        initMap();

        m_direction = Arcade::Input::RIGHT;
        m_nextDirection = Arcade::Input::RIGHT;
        
        size_t startX = mapWidth / 2;
        size_t startY = mapHeight / 2;

        m_nibbler.clear();

        for (size_t i = 0; i < INITIAL_NIBBLER_SIZE; ++i) {
            m_nibbler.push_back({startX - i, startY});
        }

        if (startX < INITIAL_NIBBLER_SIZE + 2) startX = INITIAL_NIBBLER_SIZE + 2;
        
        m_lastUpdateTime = std::chrono::steady_clock::now();
        
        spawnFood();
        
        updateMap();
    }

    void Nibbler::initMap()
    {
        if (!map) {
            map = std::make_unique<GameMap>(level, mapWidth, mapHeight);
        }
        
        map->reset();
        
        for (size_t x = 0; x < mapWidth; x++) {
            Cell* topCell = map->getCell(x, 0);
            Cell* bottomCell = map->getCell(x, mapHeight - 1);
            if (topCell) topCell->entity = EntityType::WALL;
            if (bottomCell) bottomCell->entity = EntityType::WALL;
        }
        
        for (size_t y = 0; y < mapHeight; y++) {
            Cell* leftCell = map->getCell(0, y);
            Cell* rightCell = map->getCell(mapWidth - 1, y);
            if (leftCell) leftCell->entity = EntityType::WALL;
            if (rightCell) rightCell->entity = EntityType::WALL;
        }
        for (size_t y = 2; y < mapHeight - 2; y += 3) {
            for (size_t x = 2; x < mapWidth - 2; x += 6) {
                Cell* cell = map->getCell(x, y);
                if (cell) cell->entity = EntityType::WALL;
            }
        }
    }

    void Nibbler::update(Input userInput)
    {
        if (gameOver || gameWon) {
            if (userInput == Input::RESTART) {
                reset();
            }
            return;
        }
        
        switch (userInput) {
            case Input::UP:
                if (m_direction != Arcade::Input::DOWN) {
                    m_nextDirection = Arcade::Input::UP;
                }
                break;
            case Input::DOWN:
                if (m_direction != Arcade::Input::UP) {
                    m_nextDirection = Arcade::Input::DOWN;
                }
                break;
            case Input::LEFT:
                if (m_direction != Arcade::Input::RIGHT) {
                    m_nextDirection = Arcade::Input::LEFT;
                }
                break;
            case Input::RIGHT:
                if (m_direction != Arcade::Input::LEFT) {
                    m_nextDirection = Arcade::Input::RIGHT;
                }
                break;
            default:
                break;
        }
        
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - m_lastUpdateTime).count();
        
        if (static_cast<size_t>(elapsedTime) >= m_updateInterval) {
            m_lastUpdateTime = currentTime;
            
            moveNibbler();
            
            updateMap();
        }
    }

    void Nibbler::moveNibbler()
    {
        m_direction = m_nextDirection;
        
        NibblerPart head = m_nibbler.front();
        
        NibblerPart newHead = head;
        
        switch (m_direction) {
            case Input::UP:
                newHead.y--;
                break;
            case Input::DOWN:
                newHead.y++;
                break;
            case Input::LEFT:
                newHead.x--;
                break;
            case Input::RIGHT:
                newHead.x++;
                break;
            default:
                break;
        }

        Cell* newHeadCell = map->getCell(newHead.x, newHead.y);
        
        if (newHeadCell && newHeadCell->entity == EntityType::WALL) {
            gameOver = true;
            return;
        }
        
        bool collidesWithBody = false;
        auto it = std::next(m_nibbler.begin());

        for (; it != m_nibbler.end(); ++it) {
            if (newHead.x == it->x && newHead.y == it->y) {
                collidesWithBody = true;
                break;
            }
        }

        if (collidesWithBody) {
            gameOver = true;
            return;
        }
        
        m_nibbler.push_front(newHead);
        
        if (newHead.x == m_food.x && newHead.y == m_food.y) {
            score += 10;
            
            spawnFood();
            
            if (score % 50 == 0) {
                level++;
                if (m_updateInterval > 50) {
                    m_updateInterval -= 10;
                }
            }
        } else {
            m_nibbler.pop_back();
        }
    }

    void Nibbler::spawnFood()
    {
        std::uniform_int_distribution<size_t> distX(1, mapWidth - 2);
        std::uniform_int_distribution<size_t> distY(1, mapHeight - 2);
        
        do {
            m_food.x = distX(m_rng);
            m_food.y = distY(m_rng);
            Cell* foodCell = map->getCell(m_food.x, m_food.y);
            
            if (foodCell && foodCell->entity == EntityType::WALL) continue;
            if (checkCollision(m_food.x, m_food.y)) continue;
            
            break;
        } while (true);
    }

    bool Nibbler::checkCollision(size_t x, size_t y) const
    {
        for (const auto& segment : m_nibbler) {
            if (segment.x == x && segment.y == y) {
                return true;
            }
        }
        return false;
    }

    void Nibbler::updateMap()
    {
        for (size_t y = 1; y < mapHeight - 1; y++) {
            for (size_t x = 1; x < mapWidth - 1; x++) {
                Cell* cell = map->getCell(x, y);
                if (cell && cell->entity != EntityType::WALL) 
                    cell->entity = EntityType::EMPTY;
            }
        }
        
        Cell* foodCell = map->getCell(m_food.x, m_food.y);
        if (foodCell) foodCell->entity = EntityType::BONUS;
        
        bool isHead = true;
        for (const auto& segment : m_nibbler) {
            Cell* nibblerCell = map->getCell(segment.x, segment.y);
            if (nibblerCell) {
                if (isHead) {
                    nibblerCell->entity = EntityType::SNAKE_HEAD;
                    isHead = false;
                } else {
                    nibblerCell->entity = EntityType::SNAKE_BODY;
                }
            }
        }
        
        map->setScore(score);
        map->setLevel(level);
        map->setGameOver(gameOver);
        
        if (gameOver) {
            map->setMessage("Game Over! Press R to restart");
        } else if (gameWon) {
            map->setMessage("Victory! Press R to restart");
            map->setFlag("VICTORY", true);
        } else {
            map->setMessage("");
            map->setFlag("VICTORY", false);
        }
    }

    GameMap Nibbler::getMap() const
    {
        if (map) {
            return *map;
        }
        return GameMap(level, mapWidth, mapHeight);
    }

    bool Nibbler::isGameOver() const
    {
        return gameOver;
    }

    int Nibbler::getScore() const
    {
        return score;
    }

    std::string Nibbler::getName() const
    {
        return name;
    }
}

extern "C" {
    Arcade::IGame* createGame() {
        return new Arcade::Nibbler();
    }
}