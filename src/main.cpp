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

std::unordered_map<int, std::string> GamePath;
std::unordered_map<int, std::string> GraphPath;
std::vector<std::filesystem::path> allLib;
std::string path = "./lib";
int CurrentGraphIndex = 0;
int CurrentGameIndex = 0;

void getAllLib()
{
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (!std::filesystem::is_directory(entry.path())) {
                allLib.push_back(entry.path());
            }
        }
}

bool ifGraphic(const std::string& libPath)
{
    (void)libPath;
    // void* _graphicsHandle = dlopen(libPath.c_str(), RTLD_LAZY);
    // if (!_graphicsHandle) throw std::runtime_error(dlerror());
    //
    // const void *createGraphics = dlsym(_graphicsHandle, "createGraphics");
    // if (createGraphics) {
    //     dlclose(_graphicsHandle);
    //     return true;
    // }
    // dlclose(_graphicsHandle);
    return false;
}

bool ifGame(const std::string& libPath)
{
    (void)libPath;
    // void* _gameHandle = dlopen(libPath.c_str(), RTLD_LAZY);
    // if (!_gameHandle) throw std::runtime_error(dlerror());
    //
    // const void *createGame = dlsym(_gameHandle, "createGame");
    // if (!createGame) {
    //     dlclose(_gameHandle);
    //     return true;
    // }
    // dlclose(_gameHandle);
    return false;
}

void getAllGame()
{
    int i = 1;
    for (const auto& l : allLib) {
        if (ifGame(l)) {
            GamePath[i] = l;
            i++;
        }
    }
}

void getAllGraph()
{
    int i = 1;
    for (const auto& l : allLib) {
        if (ifGraphic(l)) {
            GraphPath[i] = l;
            i++;
        }
    }
}
void check_args(int ac, char **av)
{
    if (ac != 3)
        throw std::invalid_argument("\033[35m[Warning]\033[0m Usage: ./main <lib_path> <game_path>\033[0m");
    if (std::filesystem::path(av[1]).extension() != ".so")
        throw std::invalid_argument("\033[35m[Warning]\033[0m File extension is not supported");
    if (std::filesystem::path(av[2]).extension() != ".so")
        throw std::invalid_argument("\033[35m[Warning]\033[0m File extension is not supported");
}

int getGraphIndex(const std::string& libPath)
{
    for (const auto& [index, lib] : GraphPath) {
        if (libPath == lib)
            return index;
    }
    return -1;
}

int getGameIndex(const std::string& libPath)
{
    for (const auto& [index, lib] : GamePath) {
        if (libPath == lib)
            return index;
    }
    return -1;
}

std::string getNextGraphics()
{
    CurrentGraphIndex++;
    return GraphPath[static_cast<int>(CurrentGraphIndex % GraphPath.size())];
}


std::string getNextGame()
{
    CurrentGameIndex++;
    return GamePath[static_cast<int>(CurrentGameIndex % GamePath.size())];
}
int main(int ac, char **av) {
    try {
        check_args(ac, av);
        getAllLib();
        getAllGame();
        getAllGraph();

        const std::string gamePath(av[1]);
        const std::string graphPath(av[2]);

        CurrentGameIndex = getGameIndex(gamePath);
        CurrentGraphIndex = getGraphIndex(graphPath);

        const auto core = new Core(gamePath, graphPath);
        core->run();
        delete core;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}
