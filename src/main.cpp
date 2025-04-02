/*
**  _                                              _      ___    ___
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / /
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/
**          __/ |     on 17/03/25.
**         |___/
*/
#include <iostream>

#include <iostream>
#include <vector>
#include <filesystem>

#include "core/Core.hpp"

void check_args(int ac, char **av)
{
    if (ac != 3)
        throw std::invalid_argument("\033[35m[Warning]\033[0m Usage: ./main <lib_path> <game_path>\033[0m");
    if (std::filesystem::path(av[1]).extension() != ".so")
        throw std::invalid_argument("\033[35m[Warning]\033[0m File extension is not supported");
    if (std::filesystem::path(av[2]).extension() != ".so")
        throw std::invalid_argument("\033[35m[Warning]\033[0m File extension is not supported");
}

int main(int ac, char **av) {
    try {
        check_args(ac, av);
        const std::string gamePath(av[1]);
        const std::string graphPath(av[2]);
        const Core core(gamePath, graphPath);
        core.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}

