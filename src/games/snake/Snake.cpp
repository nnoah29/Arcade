#include "Snake.hpp"
#include <algorithm>

namespace Arcade {

    Snake::Snake()
        : IGame(0, DEFAULT_WIDTH, DEFAULT_HEIGHT)
        , m_snake()
        , m_direction(Direction::RIGHT)
        , m_nextDirection(Direction::RIGHT)
        , m_food{0, 0}
        , m_updateInterval(INITIAL_SPEED)
        , m_rng(std::random_device{}())
    {
        name = "Snake";
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
        
        m_snake.clear();
        
        size_t startX = mapWidth / 2;
        size_t startY = mapHeight / 2;
        
        for (size_t i = 0; i < INITIAL_SNAKE_SIZE; ++i) {
            m_snake.push_front({startX - i, startY});
        }
        
        m_direction = Direction::RIGHT;
        m_nextDirection = Direction::RIGHT;
        
        m_lastUpdateTime = std::chrono::steady_clock::now();
        
        spawnFood();
        
        updateMap();
    }

    void Snake::initMap()
    {
        auto& cells = map.getCells();
        cells.resize(mapHeight);
        
        for (size_t y = 0; y < mapHeight; y++) {
            cells[y].resize(mapWidth);
            for (size_t x = 0; x < mapWidth; x++) {
                cells[y][x].type = arcade::CellType::EMPTY;
            }
        }
        
        for (size_t x = 0; x < mapWidth; x++) {
            cells[0][x].type = arcade::CellType::WALL;
            cells[mapHeight - 1][x].type = arcade::CellType::WALL;
        }
        
        for (size_t y = 0; y < mapHeight; y++) {
            cells[y][0].type = arcade::CellType::WALL;
            cells[y][mapWidth - 1].type = arcade::CellType::WALL;
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
                if (m_direction != Direction::DOWN) {
                    m_nextDirection = Direction::UP;
                }
                break;
            case Input::DOWN:
                if (m_direction != Direction::UP) {
                    m_nextDirection = Direction::DOWN;
                }
                break;
            case Input::LEFT:
                if (m_direction != Direction::RIGHT) {
                    m_nextDirection = Direction::LEFT;
                }
                break;
            case Input::RIGHT:
                if (m_direction != Direction::LEFT) {
                    m_nextDirection = Direction::RIGHT;
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
            case Direction::UP:
                if (newHead.y > 0) newHead.y--;
                break;
            case Direction::DOWN:
                if (newHead.y < mapHeight - 1) newHead.y++;
                break;
            case Direction::LEFT:
                if (newHead.x > 0) newHead.x--;
                break;
            case Direction::RIGHT:
                if (newHead.x < mapWidth - 1) newHead.x++;
                break;
        }
        
        if (map.getCells()[newHead.y][newHead.x].type == arcade::CellType::WALL) {
            gameOver = true;
            return;
        }
        
        if (checkCollision(newHead.x, newHead.y)) {
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
        } while (map.getCells()[m_food.y][m_food.x].type == arcade::CellType::WALL || 
                checkCollision(m_food.x, m_food.y));
    }

    bool Snake::checkCollision(size_t x, size_t y) const
    {
        return std::any_of(m_snake.begin(), m_snake.end(), 
                        [x, y](const SnakePart& segment) {
                            return segment.x == x && segment.y == y;
                        });
    }

    void Snake::updateMap()
    {
        auto& cells = map.getCells();
        
        for (size_t y = 1; y < mapHeight - 1; ++y) {
            for (size_t x = 1; x < mapWidth - 1; ++x) {
                cells[y][x].type = arcade::CellType::EMPTY;
            }
        }
        
        cells[m_food.y][m_food.x].type = arcade::CellType::FOOD;
        
        bool isHead = true;
        for (const auto& segment : m_snake) {
            if (isHead) {
                cells[segment.y][segment.x].type = arcade::CellType::SNAKE_HEAD;
                isHead = false;
            } else {
                cells[segment.y][segment.x].type = arcade::CellType::SNAKE_BODY;
            }
        }
    }

    arcade::GameMap Snake::getMap() const
    {
        return map;
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

} // namespace Arcade