#include "SFMLGraphics.hpp"
#include <iostream>
#include <cmath>

namespace Arcade {

    SFMLGraphics::SFMLGraphics() : m_lastInput(Input::NONE)
    {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 4;
        
        m_window.create(
            sf::VideoMode(1024, 768),
            "Arcade - Snake",
            sf::Style::Close,
            settings
        );
        
        m_window.setFramerateLimit(60);
        
        m_colorMap[arcade::CellType::EMPTY] = sf::Color(50, 55, 65, 0);
        m_colorMap[arcade::CellType::WALL] = sf::Color(97, 175, 239, 220);
        m_colorMap[arcade::CellType::SNAKE_HEAD] = sf::Color(152, 195, 121);
        m_colorMap[arcade::CellType::SNAKE_BODY] = sf::Color(98, 174, 119);
        m_colorMap[arcade::CellType::FOOD] = sf::Color(224, 108, 117);
        
        if (!m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            std::cerr << "Warning: Could not load font file. Using system font." << std::endl;
        }
        
        m_scoreText.setFont(m_font);
        m_scoreText.setCharacterSize(32);
        m_scoreText.setFillColor(sf::Color(220, 220, 220));
        m_scoreText.setPosition(20, 20);
        m_scoreText.setStyle(sf::Text::Bold);
        
        m_gameOverText.setFont(m_font);
        m_gameOverText.setCharacterSize(64);
        m_gameOverText.setFillColor(sf::Color(224, 108, 117));
        m_gameOverText.setString("Game Over!\nPress R to restart");
        m_gameOverText.setStyle(sf::Text::Bold);
        
        sf::FloatRect textRect = m_gameOverText.getLocalBounds();
        m_gameOverText.setOrigin(textRect.left + textRect.width/2.0f,
                                textRect.top + textRect.height/2.0f);
        m_gameOverText.setPosition(1024/2.0f, 768/2.0f);
    }

    Input SFMLGraphics::getInput()
    {
        m_lastInput = Input::NONE;
        
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                return Input::EXIT;
            }
            
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Up:
                    case sf::Keyboard::Z:
                    case sf::Keyboard::W:
                        m_lastInput = Input::UP;
                        break;
                    case sf::Keyboard::Down:
                    case sf::Keyboard::S:
                        m_lastInput = Input::DOWN;
                        break;
                    case sf::Keyboard::Left:
                    case sf::Keyboard::Q:
                    case sf::Keyboard::A:
                        m_lastInput = Input::LEFT;
                        break;
                    case sf::Keyboard::Right:
                    case sf::Keyboard::D:
                        m_lastInput = Input::RIGHT;
                        break;
                    case sf::Keyboard::Return:
                        m_lastInput = Input::ENTER;
                        break;
                    case sf::Keyboard::Escape:
                        m_lastInput = Input::BACK;
                        break;
                    case sf::Keyboard::G:
                        m_lastInput = Input::SWITCH_GAME;
                        break;
                    case sf::Keyboard::L:
                        m_lastInput = Input::SWITCH_LIB;
                        break;
                    case sf::Keyboard::R:
                        m_lastInput = Input::RESTART;
                        break;
                    case sf::Keyboard::M:
                        m_lastInput = Input::MENU;
                        break;
                    default:
                        break;
                }
            }
        }
        
        return m_lastInput;
    }

    void SFMLGraphics::draw(arcade::GameMap map)
    {
        if (!m_window.isOpen()) {
            return;
        }

        m_window.clear(sf::Color(30, 33, 39));
        
        auto& grid = map.getCells();
        
        size_t gridWidth = grid[0].size();
        size_t gridHeight = grid.size();
        
        float totalWidth = gridWidth * CELL_SIZE;
        float totalHeight = gridHeight * CELL_SIZE;
        float offsetX = (1024 - totalWidth) / 2.0f;
        float offsetY = (768 - totalHeight) / 2.0f;
        
        sf::RectangleShape gridBackground;
        gridBackground.setSize(sf::Vector2f(totalWidth, totalHeight));
        gridBackground.setPosition(offsetX, offsetY);
        gridBackground.setFillColor(sf::Color(40, 44, 52, 180));
        m_window.draw(gridBackground);
        
        static float foodAnimationTimer = 0.0f;
        foodAnimationTimer += 0.02f;
        if (foodAnimationTimer > 2 * M_PI) {
            foodAnimationTimer -= 2 * M_PI;
        }
        
        sf::RectangleShape cell;
        sf::CircleShape foodCircle;
        
        for (size_t y = 0; y < gridHeight; ++y) {
            for (size_t x = 0; x < gridWidth; ++x) {
                arcade::CellType type = grid[y][x].type;
                
                if (type == arcade::CellType::EMPTY) {
                    continue;
                }
                
                float posX = offsetX + x * CELL_SIZE;
                float posY = offsetY + y * CELL_SIZE;
                
                if (type == arcade::CellType::FOOD) {
                    float pulse = 0.8f + 0.2f * std::sin(foodAnimationTimer * 3);
                    float foodSize = CELL_SIZE * 0.4f * pulse;
                    
                    foodCircle.setRadius(foodSize);
                    foodCircle.setOrigin(foodSize, foodSize);
                    foodCircle.setPosition(posX + CELL_SIZE / 2, posY + CELL_SIZE / 2);
                    foodCircle.setFillColor(m_colorMap[type]);
                    
                    m_window.draw(foodCircle);
                }
                else if (type == arcade::CellType::SNAKE_HEAD) {
                    sf::RectangleShape head;
                    head.setSize(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
                    head.setPosition(posX + 2, posY + 2);
                    head.setFillColor(m_colorMap[type]);
                    
                    m_window.draw(head);
                }
                else if (type == arcade::CellType::SNAKE_BODY) {
                    sf::RectangleShape body;
                    body.setSize(sf::Vector2f(CELL_SIZE - 6, CELL_SIZE - 6));
                    body.setPosition(posX + 3, posY + 3);
                    body.setFillColor(m_colorMap[type]);
                    
                    m_window.draw(body);
                }
                else {
                    cell.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                    cell.setPosition(posX + 1, posY + 1);
                    
                    if (m_colorMap.find(type) != m_colorMap.end()) {
                        cell.setFillColor(m_colorMap[type]);
                    } else {
                        cell.setFillColor(sf::Color::White);
                    }
                    
                    m_window.draw(cell);
                }
            }
        }
        
        m_window.display();
    }

}