/*
**  _                                              _      ___    ___  
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / / 
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/ 
**          __/ |     on 21/03/25.                                          
**         |___/ 
*/

#ifndef GTK_HPP
#define GTK_HPP
#include <iostream>
#include <ostream>

#include "IGraphics.hpp"
#include "../games/IGame.hpp"

namespace Arcade
{
    class gtk final : public Arcade::IGraphics {
    public:
        gtk(): IGraphics() {
            name = "GTK+";
        }
        Arcade::Input getInput() override {
            std::cout << "getting input" << std::endl;
            return Arcade::Input::NONE;
        }

        void draw(GameMap map) override {
            std::cout << "Drawing map" << std::endl;
            (void)map;
        }
        std::string getName() override {
            return name;
        }
    };
}



#endif //GTK_HPP
