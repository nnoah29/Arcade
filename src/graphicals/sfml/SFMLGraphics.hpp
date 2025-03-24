#ifndef SFMLGRAPHICS_HPP
#define SFMLGRAPHICS_HPP

#include "../IGraphics.hpp"
#include <SFML/Graphics.hpp>
#include <map>

namespace Arcade {

    class SFMLGraphics : public IGraphics {
        private:
            // Fenêtre SFML
            sf::RenderWindow m_window;
            
            static constexpr int CELL_SIZE = 30;
            
            std::map<arcade::CellType, sf::Color> m_colorMap;
            
            sf::Font m_font;
            sf::Text m_scoreText;
            sf::Text m_gameOverText;
            
            Input m_lastInput;

        public:
            SFMLGraphics();
            
            Input getInput() override;
            void draw(arcade::GameMap map) override;
    };

}

#endif // SFMLGRAPHICS_HPP