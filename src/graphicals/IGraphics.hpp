/*
**  _                                              _      ___    ___  
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / / 
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/ 
**          __/ |     on 18/03/25.                                          
**         |___/ 
*/

#ifndef IGRAPHICS_HPP
#define IGRAPHICS_HPP

#include "../games/IGame.hpp"

namespace Arcade {

    class IGraphics {
        public:
        virtual ~IGraphics() = 0;
        virtual Arcade::Input getInput() = 0;
        virtual void draw(arcade::GameMap map) = 0;

    };

} // Arcade

#endif //IGRAPHICS_HPP
