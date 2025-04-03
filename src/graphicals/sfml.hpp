#ifndef SFMLGRAPHICS_HPP
#define SFMLGRAPHICS_HPP

#include "IGraphics.hpp"
#include <SFML/Graphics.hpp>
#include <map>

namespace Arcade {

    class SFMLGraphics : public IGraphics {
        private:
            sf::RenderWindow m_window;
            
            static constexpr int CELL_SIZE = 30;
            
            std::map<Arcade::EntityType, sf::Color> m_colorMap;
            sf::Color m_defaultColor;
            
            sf::Font m_font;
            sf::Text m_infoText;
            sf::Text m_statusText;
            
            sf::RectangleShape m_cellShape;
            sf::CircleShape m_circleShape;
            sf::RectangleShape m_playerShape;
            sf::RectangleShape m_enemyShape;
            
            sf::Clock m_animationClock;
            
            Input m_lastInput;
            
            void initDefaultColorMap();
            void initRenderObjects();
            
            void renderCell(Arcade::EntityType type, float x, float y, float size, 
                           const sf::Color& color, float animFactor);
            void displayGameInfo(const GameMap& map);
            void displayErrorMessage(const std::string& message);

        public:
            SFMLGraphics();
            ~SFMLGraphics() override;
            
            Input getInput() override;
            void draw(GameMap map) override;
            std::string getName() override;
    };

}

extern "C" {
    Arcade::IGraphics* createGraphics();
}

#endif // SFMLGRAPHICS_HPP