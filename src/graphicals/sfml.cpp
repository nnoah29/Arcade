#include "sfml.hpp"
#include <iostream>
#include <cmath>

namespace Arcade {

    SFMLGraphics::SFMLGraphics() : m_lastInput(Input::NONE)
    {
        name = "SFML";
        
        sf::ContextSettings settings;
        settings.antialiasingLevel = 4;
        
        m_window.create(sf::VideoMode(1024, 768), "Arcade - SFML", sf::Style::Close, settings);
        
        m_window.setFramerateLimit(60);
        
        initDefaultColorMap();
        
        if (!m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
            if (!m_font.loadFromFile("/lib/assets.fonts/arial/ttf")) {
                std::cerr << "Warning: Could not load font file. Text rendering may be disabled." << std::endl;
            }
        }
        
        m_infoText.setFont(m_font);
        m_infoText.setCharacterSize(32);
        m_infoText.setFillColor(sf::Color(220, 220, 220));
        m_infoText.setPosition(20, 20);
        m_infoText.setStyle(sf::Text::Bold);
        
        m_statusText.setFont(m_font);
        m_statusText.setCharacterSize(64);
        m_statusText.setFillColor(sf::Color(224, 108, 117));
        m_statusText.setStyle(sf::Text::Bold);
        
        sf::FloatRect textRect = m_statusText.getLocalBounds();
        m_statusText.setOrigin(textRect.left + textRect.width/2.0f,
                               textRect.top + textRect.height/2.0f);
        m_statusText.setPosition(1024/2.0f, 768/2.0f);
        
        initRenderObjects();
    }

    void SFMLGraphics::initDefaultColorMap()
    {
        m_colorMap[Arcade::EntityType::EMPTY] = sf::Color(40, 44, 52, 0);          // Transparent
        m_colorMap[Arcade::EntityType::WALL] = sf::Color(97, 175, 239, 220);       // Bleu
        m_colorMap[Arcade::EntityType::PLAYER] = sf::Color(152, 195, 121);         // Vert
        m_colorMap[Arcade::EntityType::ENEMY] = sf::Color(224, 108, 117);          // Rouge
        
        // Compatibilité avec Snake
        m_colorMap[Arcade::EntityType::SNAKE_HEAD] = sf::Color(152, 195, 121);     // Vert
        m_colorMap[Arcade::EntityType::SNAKE_BODY] = sf::Color(98, 174, 119);      // Vert foncé
        m_colorMap[Arcade::EntityType::BONUS] = sf::Color(229, 192, 123);           // Jaune
        
        m_defaultColor = sf::Color(180, 180, 180);
    }

    void SFMLGraphics::initRenderObjects()
    {
        m_cellShape.setSize(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
        
        m_circleShape.setRadius(CELL_SIZE * 0.4f);
        m_circleShape.setOrigin(CELL_SIZE * 0.4f, CELL_SIZE * 0.4f);
        
        m_playerShape.setSize(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
        m_playerShape.setOrigin(m_playerShape.getSize().x / 2, m_playerShape.getSize().y / 2);
        
        m_enemyShape.setSize(sf::Vector2f(CELL_SIZE - 6, CELL_SIZE - 6));
        m_enemyShape.setOrigin(m_enemyShape.getSize().x / 2, m_enemyShape.getSize().y / 2);
        
        m_animationClock.restart();
    }

    std::string SFMLGraphics::getName()
    {
        return name;
    }

    Input SFMLGraphics::getInput()
    {
        m_lastInput = Input::NONE;
        
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_window.close();
                exit(0);
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
                        m_lastInput = Input::LEFT;
                        break;
                    case sf::Keyboard::Right:
                    case sf::Keyboard::D:
                        m_lastInput = Input::RIGHT;
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

    void SFMLGraphics::draw(GameMap map)
    {
        if (!m_window.isOpen()) {
            return;
        }

        m_window.clear(sf::Color(30, 33, 39));
        
        auto& grid = map.getCell();
        
        if (grid.empty() || grid[0].empty()) {
            displayErrorMessage("Empty grid data");
            return;
        }
        
        size_t gridWidth = grid[0].size();
        size_t gridHeight = grid.size();
        
        float cellSizeX = std::min(float(1024) / float(gridWidth), float(CELL_SIZE));
        float cellSizeY = std::min(float(768) / float(gridHeight), float(CELL_SIZE));
        float effectiveCellSize = std::min(cellSizeX, cellSizeY);
        
        float totalWidth = gridWidth * effectiveCellSize;
        float totalHeight = gridHeight * effectiveCellSize;
        float offsetX = (1024 - totalWidth) / 2.0f;
        float offsetY = (768 - totalHeight) / 2.0f;
        
        sf::RectangleShape gridBackground;
        gridBackground.setSize(sf::Vector2f(totalWidth, totalHeight));
        gridBackground.setPosition(offsetX, offsetY);
        gridBackground.setFillColor(sf::Color(40, 44, 52, 180));
        m_window.draw(gridBackground);
        
        float animationTime = m_animationClock.getElapsedTime().asSeconds();
        float pulseFactor = 0.8f + 0.2f * std::sin(animationTime * 3);
        
        for (size_t y = 0; y < gridHeight; ++y) {
            for (size_t x = 0; x < gridWidth; ++x) {
                Arcade::EntityType type = grid[y][x].entity;
                
                if (type == Arcade::EntityType::EMPTY) {
                    continue;
                }
                
                float posX = offsetX + x * effectiveCellSize;
                float posY = offsetY + y * effectiveCellSize;
                
                sf::Color cellColor;
                if (m_colorMap.find(type) != m_colorMap.end()) {
                    cellColor = m_colorMap[type];
                } else {
                    cellColor = m_defaultColor;
                }
                
                renderCell(type, posX, posY, effectiveCellSize, cellColor, pulseFactor);
            }
        }
        
        displayGameInfo(map);
        
        m_window.display();
    }

    void SFMLGraphics::renderCell(Arcade::EntityType type, float x, float y, float size, const sf::Color& color, float animFactor)
    {
        float itemSize;

        switch (type) {
            case Arcade::EntityType::EMPTY:
            case Arcade::EntityType::HIDDEN:
                break;

            case Arcade::EntityType::PLAYER:
            case Arcade::EntityType::SNAKE_HEAD:
                m_playerShape.setSize(sf::Vector2f(size - 4, size - 4));
                m_playerShape.setPosition(x + size/2, y + size/2);
                m_playerShape.setFillColor(color);
                m_window.draw(m_playerShape);
                break;
                
            case Arcade::EntityType::ENEMY:
                m_enemyShape.setSize(sf::Vector2f(size - 6, size - 6));
                m_enemyShape.setPosition(x + size/2, y + size/2);
                m_enemyShape.setFillColor(color);
                m_window.draw(m_enemyShape);
                break;
                
            case Arcade::EntityType::BONUS:
            case Arcade::EntityType::BIG_BONUS:
                itemSize = size * 0.4f * animFactor;
                m_circleShape.setRadius(itemSize);
                m_circleShape.setOrigin(itemSize, itemSize);
                m_circleShape.setPosition(x + size/2, y + size/2);
                m_circleShape.setFillColor(color);
                m_window.draw(m_circleShape);
                break;
                
            case Arcade::EntityType::SNAKE_BODY:
                m_cellShape.setSize(sf::Vector2f(size - 6, size - 6));
                m_cellShape.setPosition(x + 3, y + 3);
                m_cellShape.setFillColor(color);
                m_window.draw(m_cellShape);
                break;

            case Arcade::EntityType::WALL:
            case Arcade::EntityType::BORDER:
            case Arcade::EntityType::PROJECTILE:
            default:
                m_cellShape.setSize(sf::Vector2f(size - 2, size - 2));
                m_cellShape.setPosition(x + 1, y + 1);
                m_cellShape.setFillColor(color);
                m_window.draw(m_cellShape);
                break;
        }
    }
    
    void SFMLGraphics::displayGameInfo(const GameMap& map)
    {
        std::string infoText;
        
        if (map.hasScore()) {
            infoText = "Score: " + std::to_string(map.getScore());
            
            if (map.hasHighScore()) {
                infoText += "  High Score: " + std::to_string(map.getHighScore());
            }
        }
        
        if (map.hasLevel()) {
            if (!infoText.empty()) infoText += "  |  ";
            infoText += "Level: " + std::to_string(map.getLevel());
        }
        
        if (map.hasLives()) {
            if (!infoText.empty()) infoText += "  |  ";
            infoText += "Lives: " + std::to_string(map.getLives());
        }
        
        if (map.hasTimeLeft()) {
            if (!infoText.empty()) infoText += "  |  ";
            infoText += "Time: " + std::to_string(map.getTimeLeft());
        }
        
        if (map.hasMessage()) {
            if (!infoText.empty()) infoText += "\n";
            infoText += map.getMessage();
        }
        
        if (!infoText.empty()) {
            m_infoText.setString(infoText);
            m_window.draw(m_infoText);
        }

        if (map.isGameOver()) {
            m_statusText.setString("Game Over!\nPress R to restart");
            m_window.draw(m_statusText);
        } else if (map.hasFlag("VICTORY")) {
            m_statusText.setString("Victory!\nPress R to restart");
            m_window.draw(m_statusText);
        } else if (map.hasFlag("PAUSED")) {
            m_statusText.setString("Paused\nPress P to continue");
            m_window.draw(m_statusText);
        }
    }
    
    void SFMLGraphics::displayErrorMessage(const std::string& message)
    {
        m_statusText.setString("Error: " + message);
        sf::FloatRect textRect = m_statusText.getLocalBounds();
        m_statusText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        m_statusText.setPosition(1024/2.0f, 768/2.0f);
        m_window.draw(m_statusText);
    }

    SFMLGraphics::~SFMLGraphics() {
        if (m_window.isOpen()) {
            m_window.close();
        }
    }

}

extern "C" {
    Arcade::IGraphics* createGraphics() {
        return new Arcade::SFMLGraphics();
    }
}