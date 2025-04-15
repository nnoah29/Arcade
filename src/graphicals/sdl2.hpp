/*
**  _                                              _      ___    ___  
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / / 
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/ 
**          __/ |     on 05/04/25.                                          
**         |___/ 
*/

#ifndef SDL2_HPP
#define SDL2_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>

#include "IGraphics.hpp"
#include "../../includes/gameMap.hpp"
#include <SDL2/SDL_image.h>


namespace Arcade
{
    class SDLGraphics final : public Arcade::IGraphics {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        int cellSize = 32; // taille d'une cellule (pixels)
        TTF_Font *font = nullptr;

        std::unordered_map<Arcade::EntityType, SDL_Color> colors;
        std::unordered_map<std::string, SDL_Texture*> imgTexture;

        static SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
            SDL_Surface* surface = IMG_Load(path.c_str());
            if (!surface) {
                return nullptr;
            }
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            return texture;
        }

        SDL_Texture *getTexture(const std::string& path)
        {
            if (imgTexture.contains(path))
                return imgTexture[path];

            const std::unordered_map<std::string, SDL_Texture*>::mapped_type newTexture = loadTexture(path, renderer);
            if (newTexture) {
                imgTexture[path] = newTexture;
            }
            return newTexture;
        }

    public:
        SDLGraphics()
        {
            name = "SDL2";
            SDL_Init(SDL_INIT_VIDEO);
            TTF_Init();
            font = TTF_OpenFont("./lib/sdl2_assets/fonts/arialbd.ttf", 24);
            window = SDL_CreateWindow("Arcade SDL2",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      1920, 1080,
                                      SDL_WINDOW_SHOWN);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            colors = {
                {Arcade::EntityType::EMPTY, {0, 0, 0, 255}},
                {Arcade::EntityType::WALL, {100, 100, 100, 255}},
                {Arcade::EntityType::PLAYER, {0, 255, 0, 255}},
                {Arcade::EntityType::ENEMY, {255, 0, 0, 255}},
                {Arcade::EntityType::BONUS, {255, 255, 0, 255}},
                {Arcade::EntityType::BIG_BONUS, {255, 165, 0, 255}},
                {Arcade::EntityType::PROJECTILE, {255, 0, 255, 255}},
                {Arcade::EntityType::HIDDEN, {50, 50, 50, 255}},
                {Arcade::EntityType::BORDER, {0, 0, 255, 255}}
            };
        }

        ~SDLGraphics() override
        {
            for (auto& pair : imgTexture)
                SDL_DestroyTexture(pair.second);
            imgTexture.clear();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }

        Arcade::Input getInput() override
        {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    return Arcade::Input::ESCAPE;
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                    case SDLK_UP: return Arcade::Input::UP;
                    case SDLK_DOWN: return Arcade::Input::DOWN;
                    case SDLK_LEFT: return Arcade::Input::LEFT;
                    case SDLK_RIGHT: return Arcade::Input::RIGHT;
                    case SDLK_ESCAPE: return Arcade::Input::ESCAPE;
                    case SDLK_l: return Arcade::Input::SWITCH_LIB;
                    case SDLK_g: return Arcade::Input::SWITCH_GAME;
                    default: return Arcade::Input::NONE;
                    }
                }
            }
            return Arcade::Input::NONE;
        }

        void draw_score(size_t score) const
        {
            if (!font) return;

            const SDL_Color white = { 255, 255, 255, 255 };
            const std::string scoreString = "Score:" + std::to_string(score);

            SDL_Surface *texte = TTF_RenderText_Solid(font, scoreString.c_str(), white);
            SDL_Texture *texteTexture = SDL_CreateTextureFromSurface(renderer, texte);
            const SDL_Rect texteRect = {20, 20, texte->w, texte->h};
            SDL_RenderCopy(renderer, texteTexture, nullptr, &texteRect);
            SDL_DestroyTexture(texteTexture);
            SDL_FreeSurface(texte);
        }

        void draw_lives(size_t lives)
        {
            const std::string imagePath = "./lib/sdl2_assets/img/heart.png";
            SDL_Texture* image = getTexture(imagePath);
            if (image == nullptr) return;
            int dist = 0;
            for (size_t i = 0; i < lives; i++) {
                const SDL_Rect dest = { 1800 + dist, 20, cellSize, cellSize };
                SDL_RenderCopy(renderer, image, nullptr, &dest);
                dist += 35;
            }
        }

        void draw(Arcade::GameMap map) override
        {
            int windowWidth, windowHeight;
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            const int offsetX = (windowWidth - static_cast<int>(map.getWidth()) * cellSize) / 2;
            const int offsetY = (windowHeight - static_cast<int>(map.getHeight()) * cellSize) / 2;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            draw_score(map.getScore());
            draw_lives(map.getLives());
            for (size_t y = 0; y <= map.getHeight(); ++y) {
                for (size_t x = 0; x <= map.getWidth(); ++x) {
                    const Arcade::Cell* cell = map.getCell(x, y);
                    if (cell)
                        drawCell(x, y, cell->entity, map, offsetX, offsetY);
                }
            }
            SDL_RenderPresent(renderer);
//          std::cout << "map received" << std::endl;
        }
        std::string getName() override { return name; }

    private:
        void drawCell(size_t x, size_t y, Arcade::EntityType type, Arcade::GameMap &map, int offsetX, int offsetY)
        {
            IMG_Init(IMG_INIT_PNG);
            const std::string imagePath = map.getEntityImagePath(type);
            SDL_Texture* image = getTexture(map.getImagePathsDirectory() + imagePath);
            if (image != nullptr) {
                const SDL_Rect dest = { static_cast<int>(offsetX + x * cellSize), static_cast<int>(offsetY + y * cellSize), cellSize, cellSize };
                SDL_RenderCopy(renderer, image, nullptr, &dest);
                return;
            }
            SDL_DestroyTexture(image);
            const SDL_Rect rect = { static_cast<int>(offsetX + x * cellSize), static_cast<int>(offsetY + y * cellSize), cellSize, cellSize };
            auto [r, g, b, a] = colors[type];
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderFillRect(renderer, &rect);
        }
    };
}

#endif //SDL2_HPP
