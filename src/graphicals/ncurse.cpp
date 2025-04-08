#include "ncurse.hpp"

namespace Arcade {

    NCurseGraphics::NCurseGraphics() : 
        m_window(nullptr),
        m_lastInput(Input::NONE),
        m_isRunning(true)
    {
        name = "NCurses";
        
        // Initialize ncurses
        initscr();
        start_color();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);           // Hide cursor
        nodelay(stdscr, TRUE); // Non-blocking input
        timeout(100);          // Timeout for getch()
        
        m_window = newwin(LINES, COLS, 0, 0);
        box(m_window, 0, 0);
        wrefresh(m_window);
        
        // Initialize colors
        initColors();
    }

    void NCurseGraphics::initColors()
    {
        init_pair(1, COLOR_BLACK, COLOR_BLACK);  // EMPTY
        init_pair(2, COLOR_BLUE, COLOR_BLUE);    // WALL
        init_pair(3, COLOR_GREEN, COLOR_BLACK);  // PLAYER
        init_pair(4, COLOR_RED, COLOR_BLACK);    // ENEMY
        init_pair(5, COLOR_YELLOW, COLOR_BLACK); // BONUS
        init_pair(6, COLOR_WHITE, COLOR_BLACK);  // DEFAULT
        
        m_colorPairs[EntityType::EMPTY] = 1;
        m_colorPairs[EntityType::WALL] = 2;
        m_colorPairs[EntityType::PLAYER] = 3;
        m_colorPairs[EntityType::ENEMY] = 4;
        m_colorPairs[EntityType::BONUS] = 5;
        m_colorPairs[EntityType::BIG_BONUS] = 5;
        m_colorPairs[EntityType::SNAKE_HEAD] = 3;
        m_colorPairs[EntityType::SNAKE_BODY] = 3;
        m_colorPairs[EntityType::PROJECTILE] = 6;
        m_colorPairs[EntityType::BORDER] = 2;
    }

    std::string NCurseGraphics::getName()
    {
        return name;
    }

    Input NCurseGraphics::getInput()
    {
        m_lastInput = Input::NONE;
        
        int ch = getch();
        if (ch != ERR) {
            switch (ch) {
                case KEY_UP:
                case 'z':
                case 'w':
                    m_lastInput = Input::UP;
                    break;
                case KEY_DOWN:
                case 's':
                    m_lastInput = Input::DOWN;
                    break;
                case KEY_LEFT:
                case 'q':
                case 'a':
                    m_lastInput = Input::LEFT;
                    break;
                case KEY_RIGHT:
                case 'd':
                    m_lastInput = Input::RIGHT;
                    break;
                case 'g':
                    m_lastInput = Input::SWITCH_GAME;
                    break;
                case 'l':
                    m_lastInput = Input::SWITCH_LIB;
                    break;
                case 'r':
                    m_lastInput = Input::RESTART;
                    break;
                case 'm':
                    m_lastInput = Input::MENU;
                    break;
                case 27: // ESC
                    m_lastInput = Input::EXIT;
                    m_isRunning = false;
                    break;
            }
        }
        
        return m_lastInput;
    }

    void NCurseGraphics::renderCell(int y, int x, EntityType type)
    {
        short colorPair = 6;
        char displayChar = ' ';
        
        if (m_colorPairs.find(type) != m_colorPairs.end()) {
            colorPair = m_colorPairs[type];
        }
        
        switch (type) {
            case EntityType::EMPTY:
                displayChar = ' ';
                break;
            case EntityType::WALL:
            case EntityType::BORDER:
                displayChar = '#';
                break;
            case EntityType::PLAYER:
                displayChar = 'P';
                break;
            case EntityType::ENEMY:
                displayChar = 'E';
                break;
            case EntityType::SNAKE_HEAD:
                displayChar = '@';
                break;
            case EntityType::SNAKE_BODY:
                displayChar = 'o';
                break;
            case EntityType::BONUS:
            case EntityType::BIG_BONUS:
                displayChar = '*';
                break;
            case EntityType::PROJECTILE:
                displayChar = '>';
                break;
            default:
                displayChar = '?';
                break;
        }
        
        wattron(m_window, COLOR_PAIR(colorPair));
        mvwaddch(m_window, y + 1, x + 1, displayChar);
        wattroff(m_window, COLOR_PAIR(colorPair));
    }

    void NCurseGraphics::draw(GameMap map)
    {
        wclear(m_window);
        box(m_window, 0, 0);
        auto& grid = map.getCell();
        for (size_t y = 0; y < grid.size(); ++y) {
            for (size_t x = 0; x < grid[y].size(); ++x) {
                renderCell(y, x, grid[y][x].entity);
            }
        }
        if (map.hasScore()) {
            mvwprintw(m_window, 0, 2, "Score: %ld", map.getScore());
        }
        
        if (map.hasLives()) {
            mvwprintw(m_window, 0, 15, "Lives: %ld", map.getLives());
        }
        if (map.isGameOver()) {
            wattron(m_window, A_BOLD);
            mvwprintw(m_window, grid.size() / 2, (COLS - 9) / 2, "GAME OVER");
            wattroff(m_window, A_BOLD);
        }
        wrefresh(m_window);
    }

    NCurseGraphics::~NCurseGraphics()
    {
        delwin(m_window);
        endwin();
    }
}

extern "C" {
    Arcade::IGraphics* createGraphics() {
        return new Arcade::NCurseGraphics();
    }
}