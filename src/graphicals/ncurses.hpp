#ifndef NCURSE_GRAPHICS_HPP
#define NCURSE_GRAPHICS_HPP

#include "IGraphics.hpp"
#include <ncurses.h>
#include <map>
#include <string>

namespace Arcade {

    class NCurseGraphics : public IGraphics {
    private:
        WINDOW *m_window;
        std::map<EntityType, short> m_colorPairs;
        Input m_lastInput;
        bool m_isRunning;
        std::string name;

        void initColors();
        void renderCell(int y, int x, EntityType type);        
    public:
        NCurseGraphics();
        ~NCurseGraphics() override;
        void draw(GameMap map) override;
        Input getInput() override;
        std::string getName() override;
    };
}

extern "C" {
    Arcade::IGraphics* createGraphics();
}

#endif