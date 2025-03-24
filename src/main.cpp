#include "graphicals/sfml/SFMLGraphics.hpp"
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>

struct Position {
    int x, y;
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class SnakeGame {
    private:
        arcade::GameMap m_gameMap;
        std::vector<Position> m_snake;
        Position m_food;
        int m_direction;
        int m_nextDirection;
        int m_score;
        bool m_gameOver;
        std::mt19937 m_rng;
        
        int m_updateInterval;
        
    public:
        SnakeGame(size_t width, size_t height) 
            : m_gameMap(0, width, height),
            m_direction(0),
            m_nextDirection(0),
            m_score(0),
            m_gameOver(false),
            m_updateInterval(150)
        {
            std::random_device rd;
            m_rng.seed(rd());
            
            initMap();
            
            initSnake();
            
            placeFood();
        }
        
        void initMap() {
            auto& cells = m_gameMap.getCells();
            
            for (size_t y = 0; y < cells.size(); ++y) {
                for (size_t x = 0; x < cells[y].size(); ++x) {
                    cells[y][x].type = arcade::CellType::EMPTY;
                }
            }
            
            for (size_t x = 0; x < cells[0].size(); ++x) {
                cells[0][x].type = arcade::CellType::WALL;
                cells[cells.size() - 1][x].type = arcade::CellType::WALL;
            }
            
            for (size_t y = 0; y < cells.size(); ++y) {
                cells[y][0].type = arcade::CellType::WALL;
                cells[y][cells[0].size() - 1].type = arcade::CellType::WALL;
            }
        }
        
        void initSnake() {
            m_snake.clear();
            
            int centerX = m_gameMap.getCells()[0].size() / 2;
            int centerY = m_gameMap.getCells().size() / 2;
            
            m_snake.push_back({centerX, centerY});
            m_snake.push_back({centerX - 1, centerY});
            m_snake.push_back({centerX - 2, centerY});
            
            updateSnakeOnMap();
        }
        
        void placeFood() {
            auto& cells = m_gameMap.getCells();
            std::uniform_int_distribution<> distX(1, cells[0].size() - 2);
            std::uniform_int_distribution<> distY(1, cells.size() - 2);
            
            do {
                m_food.x = distX(m_rng);
                m_food.y = distY(m_rng);
            } while (isSnakeAt(m_food));
            
            cells[m_food.y][m_food.x].type = arcade::CellType::FOOD;
        }
        
        bool isSnakeAt(const Position& pos) {
            for (const auto& segment : m_snake) {
                if (segment.x == pos.x && segment.y == pos.y) {
                    return true;
                }
            }
            return false;
        }
        
        void updateSnakeOnMap() {
            auto& cells = m_gameMap.getCells();
            
            for (size_t y = 1; y < cells.size() - 1; ++y) {
                for (size_t x = 1; x < cells[y].size() - 1; ++x) {
                    if (cells[y][x].type == arcade::CellType::SNAKE_HEAD || 
                        cells[y][x].type == arcade::CellType::SNAKE_BODY) {
                        cells[y][x].type = arcade::CellType::EMPTY;
                    }
                }
            }
            
            if (!m_snake.empty()) {
                cells[m_snake[0].y][m_snake[0].x].type = arcade::CellType::SNAKE_HEAD;
                
                for (size_t i = 1; i < m_snake.size(); ++i) {
                    cells[m_snake[i].y][m_snake[i].x].type = arcade::CellType::SNAKE_BODY;
                }
            }
        }
        
        void handleInput(Arcade::Input input) {
            if (m_gameOver) {
                if (input == Arcade::Input::RESTART) {
                    restart();
                }
                return;
            }
            
            switch (input) {
                case Arcade::Input::UP:
                    if (m_direction != 1) m_nextDirection = 3;
                    break;
                case Arcade::Input::DOWN:
                    if (m_direction != 3) m_nextDirection = 1;
                    break;
                case Arcade::Input::LEFT:
                    if (m_direction != 0) m_nextDirection = 2;
                    break;
                case Arcade::Input::RIGHT:
                    if (m_direction != 2) m_nextDirection = 0;
                    break;
                default:
                    break;
            }
        }
        
        void update() {
            if (m_gameOver) {
                return;
            }
            
            m_direction = m_nextDirection;
            
            Position newHead = m_snake[0];
            
            switch (m_direction) {
                case 0: newHead.x++; break;
                case 1: newHead.y++; break;
                case 2: newHead.x--; break;
                case 3: newHead.y--; break;
            }
            
            auto& cells = m_gameMap.getCells();
            if (newHead.x <= 0 || newHead.x >= static_cast<int>(cells[0].size() - 1) || 
                newHead.y <= 0 || newHead.y >= static_cast<int>(cells.size() - 1)) {
                m_gameOver = true;
                return;
            }
            
            for (size_t i = 1; i < m_snake.size(); ++i) {
                if (newHead.x == m_snake[i].x && newHead.y == m_snake[i].y) {
                    m_gameOver = true;
                    return;
                }
            }
            
            bool ateFood = (newHead.x == m_food.x && newHead.y == m_food.y);
            
            m_snake.insert(m_snake.begin(), newHead);
            if (!ateFood) {
                m_snake.pop_back();
            } else {
                m_score += 10;
                placeFood();
                
                if (m_updateInterval > 50) {
                    m_updateInterval = std::max(50, m_updateInterval - 5);
                }
            }
            
            updateSnakeOnMap();
        }
        
        void restart() {
            m_gameOver = false;
            m_score = 0;
            m_direction = 0;
            m_nextDirection = 0;
            m_updateInterval = 150;
            
            initMap();
            initSnake();
            placeFood();
        }
        
        arcade::GameMap& getGameMap() {
            return m_gameMap;
        }
        
        bool isGameOver() const {
            return m_gameOver;
        }
        
        int getUpdateInterval() const {
            return m_updateInterval;
        }
};

int main() {
    Arcade::SFMLGraphics graphics;
    SnakeGame game(20, 15);
    
    auto lastUpdate = std::chrono::steady_clock::now();
    
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - lastUpdate).count();
        
        Arcade::Input input = graphics.getInput();
        if (input == Arcade::Input::EXIT) {
            break;
        }
        
        game.handleInput(input);
        
        if (elapsedTime >= game.getUpdateInterval()) {
            game.update();
            lastUpdate = currentTime;
        }
        
        graphics.draw(game.getGameMap());
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return 0;
}