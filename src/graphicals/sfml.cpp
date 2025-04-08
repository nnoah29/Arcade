#include "sfml.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <random>

namespace Arcade {

    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Color color;
        float lifetime;
        float maxLifetime;
        float size;
    };

    class VisualEffects {
        private:
            std::vector<Particle> m_particles;
            sf::Clock m_effectClock;
            std::mt19937 m_randomEngine;
            
        public:
            VisualEffects() : m_randomEngine(std::random_device{}()) {}
            
            void update() {
                float deltaTime = m_effectClock.restart().asSeconds();
                
                for (auto it = m_particles.begin(); it != m_particles.end();) {
                    it->lifetime -= deltaTime;
                    it->position += it->velocity * deltaTime;
                    it->velocity *= 0.95f;
                    
                    if (it->lifetime <= 0) {
                        it = m_particles.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
            
            void addParticleEffect(sf::Vector2f position, sf::Color baseColor, int count) {
                std::uniform_real_distribution<float> angleDist(0, 2 * M_PI);
                std::uniform_real_distribution<float> speedDist(20.0f, 60.0f);
                std::uniform_real_distribution<float> lifeDist(0.3f, 1.0f);
                std::uniform_real_distribution<float> sizeDist(2.0f, 5.0f);
                
                for (int i = 0; i < count; ++i) {
                    Particle p;
                    p.position = position;
                    
                    float angle = angleDist(m_randomEngine);
                    float speed = speedDist(m_randomEngine);
                    p.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
                    
                    // Variante de couleur
                    sf::Color color = baseColor;
                    color.r = std::min(255, std::max(0, (int)color.r + (rand() % 60 - 30)));
                    color.g = std::min(255, std::max(0, (int)color.g + (rand() % 60 - 30)));
                    color.b = std::min(255, std::max(0, (int)color.b + (rand() % 60 - 30)));
                    p.color = color;
                    
                    p.lifetime = lifeDist(m_randomEngine);
                    p.maxLifetime = p.lifetime;
                    p.size = sizeDist(m_randomEngine);
                    
                    m_particles.push_back(p);
                }
            }
            
            void draw(sf::RenderWindow& window) {
                for (const auto& p : m_particles) {
                    sf::CircleShape shape(p.size * (p.lifetime / p.maxLifetime));
                    shape.setPosition(p.position - sf::Vector2f(p.size, p.size));
                    
                    sf::Color color = p.color;
                    color.a = static_cast<sf::Uint8>(255 * (p.lifetime / p.maxLifetime));
                    
                    shape.setFillColor(color);
                    window.draw(shape);
                }
            }
        };

    bool endsWith(const std::string& str, const std::string& suffix) {
        if (str.length() < suffix.length()) 
            return false;
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    SFMLGraphics::SFMLGraphics() : m_lastInput(Input::NONE), m_currentTheme(0)
    {
        name = "SFML";
        
        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        
        m_window.create(sf::VideoMode(1024, 768), "Made By Chrisnaud (JaceX10)", sf::Style::Close, settings);
        m_window.setFramerateLimit(60);
        
        initColorThemes();
        
        loadResources();
        
        m_infoPanel.setSize(sf::Vector2f(984, 100));
        m_infoPanel.setPosition(20, 20);
        m_infoPanel.setFillColor(sf::Color(40, 44, 52, 180));
        m_infoPanel.setOutlineThickness(2);
        m_infoPanel.setOutlineColor(sf::Color(97, 175, 239, 150));
        
        m_infoText.setFont(m_font);
        m_infoText.setCharacterSize(24);
        m_infoText.setFillColor(sf::Color(220, 220, 220));
        m_infoText.setPosition(35, 35);
        m_infoText.setStyle(sf::Text::Bold);
        
        m_statusPanel.setSize(sf::Vector2f(600, 200));
        m_statusPanel.setFillColor(sf::Color(40, 44, 52, 220));
        m_statusPanel.setOutlineThickness(3);
        m_statusPanel.setOutlineColor(sf::Color(229, 192, 123));
        
        m_statusText.setFont(m_font);
        m_statusText.setCharacterSize(48);
        m_statusText.setFillColor(sf::Color(224, 108, 117));
        m_statusText.setStyle(sf::Text::Bold);
        
        m_visualEffects = std::make_unique<VisualEffects>();
        
        initRenderObjects();
        
        m_backgroundTexture.create(1024, 768);
        generateBackground();
        
        m_animationTimer = 0.0f;
        
    }

    void SFMLGraphics::loadResources()
    {
        std::vector<std::string> fontPaths = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/lib/assets/fonts/arial.ttf",
            "./assets/fonts/DejaVuSans.ttf",
            "C:/Windows/Fonts/arial.ttf"
        };
        
        bool fontLoaded = false;
        for (const auto& path : fontPaths) {
            if (m_font.loadFromFile(path)) {
                fontLoaded = true;
                break;
            }
        }
        
        if (!fontLoaded) {
            std::cerr << "Warning: Could not load font file. Using system default font." << std::endl;
        }
    }

    void SFMLGraphics::initColorThemes()
    {
        ColorTheme modernTheme;
        modernTheme.background = sf::Color(30, 33, 39);
        modernTheme.gridBackground = sf::Color(40, 44, 52, 180);
        modernTheme.empty = sf::Color(40, 44, 52, 0);
        modernTheme.wall = sf::Color(97, 175, 239, 220);
        modernTheme.player = sf::Color(152, 195, 121);
        modernTheme.enemy = sf::Color(224, 108, 117);
        modernTheme.snakeHead = sf::Color(152, 195, 121);
        modernTheme.snakeBody = sf::Color(98, 174, 119);
        modernTheme.bonus = sf::Color(229, 192, 123);
        modernTheme.bigBonus = sf::Color(229, 162, 103);
        modernTheme.border = sf::Color(86, 182, 194);
        modernTheme.text = sf::Color(220, 220, 220);
        modernTheme.highlight = sf::Color(198, 120, 221);
        m_themes.push_back(modernTheme);
        
        ColorTheme retroTheme;
        retroTheme.background = sf::Color(25, 20, 40);
        retroTheme.gridBackground = sf::Color(35, 25, 60, 180);
        retroTheme.empty = sf::Color(40, 30, 70, 0);
        retroTheme.wall = sf::Color(80, 50, 160, 220);
        retroTheme.player = sf::Color(230, 100, 200);
        retroTheme.enemy = sf::Color(250, 100, 100);
        retroTheme.snakeHead = sf::Color(0, 255, 200);
        retroTheme.snakeBody = sf::Color(0, 200, 160);
        retroTheme.bonus = sf::Color(255, 230, 120);
        retroTheme.bigBonus = sf::Color(255, 160, 50);
        retroTheme.border = sf::Color(200, 120, 255);
        retroTheme.text = sf::Color(240, 240, 255);
        retroTheme.highlight = sf::Color(255, 100, 200);
        m_themes.push_back(retroTheme);
        
        ColorTheme natureTheme;
        natureTheme.background = sf::Color(30, 50, 40);
        natureTheme.gridBackground = sf::Color(40, 60, 50, 180);
        natureTheme.empty = sf::Color(40, 70, 50, 0);
        natureTheme.wall = sf::Color(100, 140, 90, 220);
        natureTheme.player = sf::Color(180, 210, 120);
        natureTheme.enemy = sf::Color(190, 80, 60);
        natureTheme.snakeHead = sf::Color(120, 200, 80);
        natureTheme.snakeBody = sf::Color(90, 160, 70);
        natureTheme.bonus = sf::Color(220, 200, 110);
        natureTheme.bigBonus = sf::Color(230, 175, 65);
        natureTheme.border = sf::Color(80, 120, 100);
        natureTheme.text = sf::Color(210, 230, 200);
        natureTheme.highlight = sf::Color(190, 220, 130);
        m_themes.push_back(natureTheme);
        
        m_colorMap = getActiveColorMap();
    }

    std::map<Arcade::EntityType, sf::Color> SFMLGraphics::getActiveColorMap() const
    {
        std::map<Arcade::EntityType, sf::Color> colorMap;
        
        const auto& theme = m_themes[m_currentTheme];
        
        colorMap[Arcade::EntityType::EMPTY] = theme.empty;
        colorMap[Arcade::EntityType::WALL] = theme.wall;
        colorMap[Arcade::EntityType::PLAYER] = theme.player;
        colorMap[Arcade::EntityType::ENEMY] = theme.enemy;
        colorMap[Arcade::EntityType::SNAKE_HEAD] = theme.snakeHead;
        colorMap[Arcade::EntityType::SNAKE_BODY] = theme.snakeBody;
        colorMap[Arcade::EntityType::BONUS] = theme.bonus;
        colorMap[Arcade::EntityType::BIG_BONUS] = theme.bigBonus;
        colorMap[Arcade::EntityType::BORDER] = theme.border;
        
        return colorMap;
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

    void SFMLGraphics::generateBackground()
    {
        m_backgroundTexture.clear(m_themes[m_currentTheme].background);
        
        sf::RectangleShape gridLine;
        gridLine.setFillColor(sf::Color(255, 255, 255, 10));
        
        gridLine.setSize(sf::Vector2f(1024, 1));
        for (int y = 0; y < 768; y += 40) {
            gridLine.setPosition(0, y);
            m_backgroundTexture.draw(gridLine);
        }
        
        gridLine.setSize(sf::Vector2f(1, 768));
        for (int x = 0; x < 1024; x += 40) {
            gridLine.setPosition(x, 0);
            m_backgroundTexture.draw(gridLine);
        }
        
        sf::CircleShape dot(2);
        dot.setFillColor(sf::Color(255, 255, 255, 40));
        
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> xDist(0, 1024);
        std::uniform_int_distribution<int> yDist(0, 768);
        
        for (int i = 0; i < 100; ++i) {
            dot.setPosition(xDist(rng), yDist(rng));
            m_backgroundTexture.draw(dot);
        }
        
        m_backgroundTexture.display();
        m_backgroundSprite.setTexture(m_backgroundTexture.getTexture());
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
                    case sf::Keyboard::A:
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
                    case sf::Keyboard::T:
                        m_currentTheme = (m_currentTheme + 1) % m_themes.size();
                        m_colorMap = getActiveColorMap();
                        generateBackground();
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
        
        m_animationTimer += m_animationClock.restart().asSeconds();
        m_visualEffects->update();

        m_window.clear(m_themes[m_currentTheme].background);
        m_window.draw(m_backgroundSprite);
        
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
        gridBackground.setSize(sf::Vector2f(totalWidth + 10, totalHeight + 10));
        gridBackground.setPosition(offsetX - 5, offsetY - 5);
        
        sf::Color glowColor = m_themes[m_currentTheme].highlight;
        glowColor.a = 100 + 50 * std::sin(m_animationTimer * 2.0f);
        
        gridBackground.setFillColor(m_themes[m_currentTheme].gridBackground);
        gridBackground.setOutlineThickness(3);
        gridBackground.setOutlineColor(glowColor);
        m_window.draw(gridBackground);
        
        float pulseFactor = 0.8f + 0.2f * std::sin(m_animationTimer * 3);
        float rotationFactor = 45.0f * std::sin(m_animationTimer * 0.5f);
        
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
                    cellColor = m_themes[m_currentTheme].text;
                }
                
                if ((type == Arcade::EntityType::PLAYER || type == Arcade::EntityType::SNAKE_HEAD) && 
                    m_animationTimer - m_lastParticleTime > 0.2f) {
                    m_visualEffects->addParticleEffect(
                        sf::Vector2f(posX + effectiveCellSize/2, posY + effectiveCellSize/2),
                        cellColor, 3);
                    m_lastParticleTime = m_animationTimer;
                }
                
                renderCell(type, posX, posY, effectiveCellSize, cellColor, pulseFactor, rotationFactor);
            }
        }
        
        m_visualEffects->draw(m_window);
        
        displayGameInfo(map);
        
        m_window.display();
    }

    void SFMLGraphics::renderCell(Arcade::EntityType type, float x, float y, float size, 
        const sf::Color& color, float animFactor, float rotationFactor)
    {
        float itemSize;
        sf::Color renderColor = color;

        switch (type) {
            case Arcade::EntityType::EMPTY:
            case Arcade::EntityType::HIDDEN:
            break;

            case Arcade::EntityType::PLAYER:
            case Arcade::EntityType::SNAKE_HEAD: {
                m_playerShape.setSize(sf::Vector2f((size - 4) * animFactor, (size - 4) * animFactor));
                m_playerShape.setPosition(x + size/2, y + size/2);
                m_playerShape.setFillColor(renderColor);

                m_playerShape.setOutlineThickness(2);
                m_playerShape.setOutlineColor(sf::Color(renderColor.r, renderColor.g, renderColor.b, 150));

                m_window.draw(m_playerShape);

                sf::CircleShape dirPoint(size * 0.15f);
                dirPoint.setOrigin(size * 0.15f, size * 0.15f);
                dirPoint.setPosition(x + size/2 + size * 0.2f * std::cos(m_animationTimer * 2), 
                    y + size/2 + size * 0.2f * std::sin(m_animationTimer * 2));
                dirPoint.setFillColor(sf::Color::White);
                m_window.draw(dirPoint);
                break;
            }

            case Arcade::EntityType::ENEMY: {
                m_enemyShape.setSize(sf::Vector2f((size - 6) * animFactor, (size - 6) * animFactor));
                m_enemyShape.setPosition(x + size/2, y + size/2);
                m_enemyShape.setFillColor(renderColor);
                m_enemyShape.setOutlineThickness(1.5f);
                m_enemyShape.setOutlineColor(sf::Color(renderColor.r + 40, renderColor.g + 40, renderColor.b + 40, 180));
                m_enemyShape.setRotation(rotationFactor);
                m_window.draw(m_enemyShape);
                break;
            }

            case Arcade::EntityType::BONUS: {
                itemSize = size * 0.4f * animFactor;
                m_circleShape.setRadius(itemSize);
                m_circleShape.setOrigin(itemSize, itemSize);
                m_circleShape.setPosition(x + size/2, y + size/2);

                sf::Color glowColor = renderColor;
                glowColor.a = 150 + 100 * std::sin(m_animationTimer * 5.0f + x * 0.1f + y * 0.1f);

                m_circleShape.setFillColor(glowColor);
                m_window.draw(m_circleShape);

                float starSize = itemSize * 0.7f;
                sf::CircleShape star(starSize, 5);
                star.setOrigin(starSize, starSize);
                star.setPosition(x + size/2, y + size/2);
                star.setRotation(m_animationTimer * 30.0f);
                star.setFillColor(sf::Color::White);
                m_window.draw(star);
                break;
            }

            case Arcade::EntityType::BIG_BONUS: {
                itemSize = size * 0.6f * animFactor;
                sf::CircleShape bigBonus(itemSize, 6);
                bigBonus.setOrigin(itemSize, itemSize);
                bigBonus.setPosition(x + size/2, y + size/2);
                bigBonus.setRotation(m_animationTimer * 20.0f);

                sf::Color outerColor = renderColor;
                sf::Color innerColor = sf::Color(
                std::min(255, renderColor.r + 80),
                std::min(255, renderColor.g + 80),
                std::min(255, renderColor.b + 40),
                200 + 55 * std::sin(m_animationTimer * 4.0f)
                );

                bigBonus.setFillColor(outerColor);
                bigBonus.setOutlineThickness(3);
                bigBonus.setOutlineColor(innerColor);
                m_window.draw(bigBonus);
                break;
            }

            case Arcade::EntityType::SNAKE_BODY: {
                sf::RectangleShape body;
                body.setSize(sf::Vector2f(size - 6, size - 6));
                body.setPosition(x + 3, y + 3);
                body.setFillColor(renderColor);

                sf::RectangleShape scale;
                scale.setSize(sf::Vector2f((size - 8) / 3, (size - 8) / 3));
                scale.setFillColor(sf::Color(
                std::max(0, renderColor.r - 30),
                std::max(0, renderColor.g - 20),
                std::max(0, renderColor.b - 20),
                180
                ));

                m_window.draw(body);

                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        scale.setPosition(x + 5 + i * (size - 8) / 2, y + 5 + j * (size - 8) / 2);
                        m_window.draw(scale);
                    }
                }
                    break;
            }

            case Arcade::EntityType::WALL: {
                sf::RectangleShape wall;
                wall.setSize(sf::Vector2f(size - 2, size - 2));
                wall.setPosition(x + 1, y + 1);
                wall.setFillColor(renderColor);
                m_window.draw(wall);

                sf::RectangleShape brickLine;
                brickLine.setFillColor(sf::Color(
                    std::max(0, renderColor.r - 30),
                    std::max(0, renderColor.g - 30),
                    std::max(0, renderColor.b - 30),
                    200
                ));

                brickLine.setSize(sf::Vector2f(size - 4, 1));
                for (int i = 0; i < 3; i++) {
                    brickLine.setPosition(x + 2, y + (i+1) * (size-2) / 3);
                    m_window.draw(brickLine);
                }

                brickLine.setSize(sf::Vector2f(1, (size-2) / 3));
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        int offset = (i % 2 == 0) ? 0 : (size-2) / 6;
                        brickLine.setPosition(x + 2 + (j+1) * (size-4) / 2 + offset, y + 2 + i * (size-2) / 3);
                        m_window.draw(brickLine);
                    }
                }
                break;
            }

            case Arcade::EntityType::BORDER: {
                sf::RectangleShape border;
                border.setSize(sf::Vector2f(size - 1, size - 1));
                border.setPosition(x, y);

                sf::Color borderColor = renderColor;
                borderColor.a = 150 + 100 * std::sin(m_animationTimer * 1.5f + x * 0.05f + y * 0.05f);

                border.setFillColor(sf::Color(renderColor.r, renderColor.g, renderColor.b, 80));
                border.setOutlineThickness(2);
                border.setOutlineColor(borderColor);
                m_window.draw(border);
                break;
            }

            case Arcade::EntityType::PROJECTILE: {
                sf::CircleShape projectile(size * 0.25f);
                projectile.setOrigin(size * 0.25f, size * 0.25f);
                projectile.setPosition(x + size/2, y + size/2);

                sf::Color projectileColor = renderColor;
                projectileColor.a = 200 + 55 * std::sin(m_animationTimer * 8.0f);

                projectile.setFillColor(projectileColor);
                m_window.draw(projectile);

                sf::ConvexShape trail;
                trail.setPointCount(3);
                trail.setPoint(0, sf::Vector2f(x + size/2, y + size/2));
                trail.setPoint(1, sf::Vector2f(x + size/2 - size * 0.4f, y + size/2 - size * 0.15f));
                trail.setPoint(2, sf::Vector2f(x + size/2 - size * 0.4f, y + size/2 + size * 0.15f));

                sf::Color trailColor = renderColor;
                trailColor.a = 100;
                trail.setFillColor(trailColor);
                m_window.draw(trail);
                break;
            }

            default: {
                m_cellShape.setSize(sf::Vector2f(size - 2, size - 2));
                m_cellShape.setPosition(x + 1, y + 1);
                m_cellShape.setFillColor(renderColor);
                m_cellShape.setOutlineThickness(1);
                m_cellShape.setOutlineColor(sf::Color(renderColor.r, renderColor.g, renderColor.b, 120));
                m_window.draw(m_cellShape);
                break;
                }
            }
    }

    void SFMLGraphics::displayGameInfo(const GameMap& map)
    {
        std::string infoText;
        
        if (map.hasScore()) {
            infoText = "SCORE: " + std::to_string(map.getScore());
            
            if (map.hasHighScore()) {
                infoText += "  |  HIGH SCORE: " + std::to_string(map.getHighScore());
            }
        }
        
        if (map.hasLevel()) {
            if (!infoText.empty()) infoText += "\n";
            infoText += "LEVEL: " + std::to_string(map.getLevel());
        }
        
        if (map.hasLives()) {
            if (!infoText.empty() && !endsWith(infoText, "\n")) infoText += "  |  ";
            infoText += "LIVES: ";
            
            int lives = map.getLives();
            for (int i = 0; i < lives && i < 5; i++) {
                infoText += "♥ ";
            }
            if (lives > 5) {
                infoText += "+ " + std::to_string(lives - 5);
            }
        }
        
        if (map.hasMessage()) {
            if (!infoText.empty()) infoText += "\n\n";
            infoText += map.getMessage();
        }
        
        if (!infoText.empty()) {
            sf::RectangleShape shadowPanel = m_infoPanel;
            shadowPanel.setPosition(m_infoPanel.getPosition() + sf::Vector2f(4, 4));
            shadowPanel.setFillColor(sf::Color(20, 20, 20, 100));
            shadowPanel.setOutlineThickness(0);
            m_window.draw(shadowPanel);
            
            m_window.draw(m_infoPanel);
            
            m_infoText.setString(infoText);
            m_window.draw(m_infoText);
            
            sf::Text themeIndicator;
            themeIndicator.setFont(m_font);
            themeIndicator.setCharacterSize(14);
            themeIndicator.setFillColor(sf::Color(180, 180, 180));
            themeIndicator.setPosition(m_infoPanel.getPosition().x + m_infoPanel.getSize().x - 200, 
                                      m_infoPanel.getPosition().y + m_infoPanel.getSize().y - 25);
            themeIndicator.setString("Theme: " + std::to_string(m_currentTheme + 1) + " [T to change]");
            m_window.draw(themeIndicator);
        }

        if (map.isGameOver() || map.hasFlag("VICTORY") || map.hasFlag("PAUSED")) {
            sf::RectangleShape overlay;
            overlay.setSize(sf::Vector2f(1024, 768));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            m_window.draw(overlay);
            
            std::string statusMessage;
            sf::Color statusColor;
            
            if (map.isGameOver()) {
                statusMessage = "GAME OVER\nScore: " + std::to_string(map.getScore()) + "\nPress R to restart";
                statusColor = sf::Color(224, 108, 117);
                
                static bool gameOverParticlesGenerated = false;
                if (!gameOverParticlesGenerated) {
                    m_visualEffects->addParticleEffect(sf::Vector2f(512, 384), sf::Color(255, 100, 100), 50);
                    gameOverParticlesGenerated = true;
                }
            } 
            else if (map.hasFlag("VICTORY")) {
                statusMessage = "VICTORY!\nScore: " + std::to_string(map.getScore()) + "\nPress R to restart";
                statusColor = sf::Color(152, 195, 121);
                
                static bool victoryParticlesGenerated = false;
                if (!victoryParticlesGenerated) {
                    m_visualEffects->addParticleEffect(sf::Vector2f(512, 384), sf::Color(100, 255, 100), 50);
                    victoryParticlesGenerated = true;
                }
            } 
            else if (map.hasFlag("PAUSED")) {
                statusMessage = "PAUSED\nPress P to continue";
                statusColor = sf::Color(229, 192, 123);
            }
            
            m_statusPanel.setSize(sf::Vector2f(std::max(300.0f, float(statusMessage.length() * 14)), 200));
            m_statusPanel.setOrigin(m_statusPanel.getSize().x / 2, m_statusPanel.getSize().y / 2);
            m_statusPanel.setPosition(1024 / 2.0f, 768 / 2.0f);
            
            sf::RectangleShape statusShadow = m_statusPanel;
            statusShadow.setPosition(m_statusPanel.getPosition() + sf::Vector2f(6, 6));
            statusShadow.setFillColor(sf::Color(20, 20, 20, 120));
            statusShadow.setOutlineThickness(0);
            m_window.draw(statusShadow);
            
            sf::Color outlineColor = m_statusPanel.getOutlineColor();
            outlineColor.a = 150 + 100 * std::sin(m_animationTimer * 3.0f);
            m_statusPanel.setOutlineColor(outlineColor);
            m_window.draw(m_statusPanel);
            
            m_statusText.setString(statusMessage);
            m_statusText.setFillColor(statusColor);
            
            sf::FloatRect textRect = m_statusText.getLocalBounds();
            m_statusText.setOrigin(textRect.left + textRect.width/2.0f,
                               textRect.top + textRect.height/2.0f);
            m_statusText.setPosition(1024/2.0f, 768/2.0f);
            
            sf::Text statusShadowText = m_statusText;
            statusShadowText.setPosition(m_statusText.getPosition() + sf::Vector2f(2, 2));
            statusShadowText.setFillColor(sf::Color(30, 30, 30, 180));
            m_window.draw(statusShadowText);
            
            m_window.draw(m_statusText);
        }
    }
    
    void SFMLGraphics::displayErrorMessage(const std::string& message)
    {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(1024, 768));
        overlay.setFillColor(sf::Color(30, 30, 30, 200));
        m_window.draw(overlay);
        
        sf::RectangleShape errorPanel;
        errorPanel.setSize(sf::Vector2f(600, 200));
        errorPanel.setOrigin(300, 100);
        errorPanel.setPosition(1024/2.0f, 768/2.0f);
        errorPanel.setFillColor(sf::Color(80, 40, 40, 220));
        errorPanel.setOutlineThickness(3);
        errorPanel.setOutlineColor(sf::Color(224, 108, 117));
        m_window.draw(errorPanel);
        
        m_statusText.setString("Error: " + message);
        m_statusText.setFillColor(sf::Color(255, 200, 200));
        
        sf::FloatRect textRect = m_statusText.getLocalBounds();
        m_statusText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        m_statusText.setPosition(1024/2.0f, 768/2.0f);
        
        sf::Text errorShadowText = m_statusText;
        errorShadowText.setPosition(m_statusText.getPosition() + sf::Vector2f(2, 2));
        errorShadowText.setFillColor(sf::Color(0, 0, 0, 180));
        m_window.draw(errorShadowText);
        
        m_window.draw(m_statusText);
        
        sf::CircleShape errorIcon(30, 3);
        errorIcon.setOrigin(30, 30);
        errorIcon.setPosition(errorPanel.getPosition().x - 200, errorPanel.getPosition().y);
        errorIcon.setFillColor(sf::Color(224, 108, 117));
        errorIcon.setRotation(180 + std::sin(m_animationTimer * 4.0f) * 10.0f);
        m_window.draw(errorIcon);
    }

    void SFMLGraphics::cycleTheme()
    {
        m_currentTheme = (m_currentTheme + 1) % m_themes.size();
        m_colorMap = getActiveColorMap();
        generateBackground();
    }

    SFMLGraphics::~SFMLGraphics() {
        if (m_window.isOpen()) {
            m_window.close();
        }
    }

} // namespace Arcade

extern "C" {
    Arcade::IGraphics* createGraphics() {
        return new Arcade::SFMLGraphics();
    }
}