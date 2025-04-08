#ifndef SFML_HPP
#define SFML_HPP

#include "IGraphics.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

namespace Arcade {

    class VisualEffects;

    struct ColorTheme {
        sf::Color background;
        sf::Color gridBackground;
        sf::Color empty;
        sf::Color wall;
        sf::Color player;
        sf::Color enemy;
        sf::Color snakeHead;
        sf::Color snakeBody;
        sf::Color bonus;
        sf::Color bigBonus;
        sf::Color border;
        sf::Color text;
        sf::Color highlight;
    };

    class SFMLGraphics : public IGraphics {
        private:

            static constexpr int CELL_SIZE = 32;
            std::map<Arcade::EntityType, sf::Color> m_colorMap;
            sf::Color m_defaultColor;
            
            sf::RenderWindow m_window;
            sf::Font m_font;
            Input m_lastInput;
            
            sf::RectangleShape m_cellShape;
            sf::CircleShape m_circleShape;
            sf::RectangleShape m_playerShape;
            sf::RectangleShape m_enemyShape;
            
            sf::Clock m_animationClock;
            float m_animationTimer;
            float m_lastParticleTime = 0.0f;

            std::vector<ColorTheme> m_themes;
            int m_currentTheme;
            
            sf::RectangleShape m_infoPanel;
            sf::Text m_infoText;
            sf::RectangleShape m_statusPanel;
            sf::Text m_statusText;
            
            sf::RenderTexture m_backgroundTexture;
            sf::Sprite m_backgroundSprite;
            
            std::unique_ptr<VisualEffects> m_visualEffects;
            
            void loadResources();
            void initColorThemes();
            void initRenderObjects();
            std::map<Arcade::EntityType, sf::Color> getActiveColorMap() const;
            void generateBackground();
            
            void renderCell(Arcade::EntityType type, float x, float y, float size, 
                          const sf::Color& color, float animFactor, float rotationFactor);
            void displayGameInfo(const GameMap& map);
            void displayErrorMessage(const std::string& message);
            
        public:
            std::string name;
            SFMLGraphics();
            ~SFMLGraphics();
            
            std::string getName() override;
            Input getInput() override;
            void draw(GameMap map) override;
            void cycleTheme();
    };

} // namespace Arcade

#endif