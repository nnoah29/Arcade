#include "snake.hpp"
#include <algorithm>
#include <iostream>

namespace Arcade {
    Snake::Snake()
        : IGame()
    {
        name = "Snake";
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

    void Snake::reset()
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

        m_snake.clear();

        for (size_t i = 0; i < INITIAL_SNAKE_SIZE; ++i) {
            m_snake.push_back({startX - i, startY});
        }
        

        if (startX < INITIAL_SNAKE_SIZE + 2) startX = INITIAL_SNAKE_SIZE + 2;
        
        m_lastUpdateTime = std::chrono::steady_clock::now();
        
        spawnFood();
        
        updateMap();
    }

    void Snake::initMap()
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
    }

    void Snake::update(Input userInput)
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
            
            moveSnake();
            
            updateMap();
        }
    }

    void Snake::moveSnake()
    {
        m_direction = m_nextDirection;
        
        SnakePart head = m_snake.front();
        
        SnakePart newHead = head;
        
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
            case Input::EXIT:
            break;
            case Input::ESCAPE:
                break;
            case Input::SWITCH_GAME:
            case Input::SWITCH_LIB:
            case Input::MENU:
            case Input::NONE:
            case Input::BACK:
            case Input::RESTART:
            case Input::ENTER:
                break;
        }

        Cell* newHeadCell = map->getCell(newHead.x, newHead.y);
        
        if (newHeadCell && newHeadCell->entity == EntityType::WALL) {
            gameOver = true;
            return;
        }
        
        bool collidesWithBody = false;
        auto it = std::next(m_snake.begin());

        for (; it != m_snake.end(); ++it) {
            if (newHead.x == it->x && newHead.y == it->y) {
                collidesWithBody = true;
                break;
            }
        }

        if (collidesWithBody) {
            gameOver = true;
            return;
        }
        
        m_snake.push_front(newHead);
        
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
            m_snake.pop_back();
        }
    }

    void Snake::spawnFood()
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

    bool Snake::checkCollision(size_t x, size_t y) const
    {
        auto it = std::next(m_snake.begin());
        
        return std::any_of(it, m_snake.end(), [x, y](const SnakePart& segment) {
            return segment.x == x && segment.y == y; 
        });
    }

    void Snake::updateMap()
    {
        for (size_t y = 1; y < mapHeight - 1; y++) {
            for (size_t x = 1; x < mapWidth - 1; x++) {
                Cell* cell = map->getCell(x, y);
                if (cell) cell->entity = EntityType::EMPTY;
            }
        }
        
        Cell* foodCell = map->getCell(m_food.x, m_food.y);
        if (foodCell) foodCell->entity = EntityType::BONUS;
        
        bool isHead = true;
        for (const auto& segment : m_snake) {
            Cell* snakeCell = map->getCell(segment.x, segment.y);
            if (snakeCell) {
                if (isHead) {
                    snakeCell->entity = EntityType::PLAYER;
                    isHead = false;
                } else {
                    snakeCell->entity = EntityType::PLAYER;
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

    GameMap Snake::getMap() const
    {
        if (map) {
            return *map;
        }
        return GameMap(level, mapWidth, mapHeight);
    }

    bool Snake::isGameOver() const
    {
        return gameOver;
    }

    int Snake::getScore() const
    {
        return score;
    }

    std::string Snake::getName() const
    {
        return name;
    }
}

extern "C" {
    Arcade::IGame* createGame() {
        return new Arcade::Snake();
    }
}