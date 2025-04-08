# === CONFIGURATION ===
NAME        := arcade
LIB_DIR     := lib

CXX         := g++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++20 -Iincludes -fPIC -fno-gnu-unique
LDFLAGS     := -ldl
SFML_LIBS	:= -lsfml-graphics -lsfml-window -lsfml-system
NCURSES_LIBS:= -lncurses
SDL2_LIBS   := -lSDL2 -lSDL2_image -lSDL2_ttf


DEBUG       ?= 0
ifeq ($(DEBUG),1)
	CXXFLAGS += -g -DDEBUG
endif

# === VERBOSE SWITCH ===
ifndef V
	SILENT = @
else
	SILENT =
endif

# === PATHS ===
SRC_DIR     := src
INC_DIR     := includes
OBJ_DIR     := obj
GRAPHICS_DIR := src/graphicals
GAMES_DIR   := src/games

# === FILES ===
CORE_SRCS  := $(shell find $(SRC_DIR) -maxdepth 1 -name '*.cpp')
CORE_OBJS  := $(CORE_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

GRAPHICS_SRCS := $(shell find $(GRAPHICS_DIR) -name '*.cpp')
GRAPHICS_LIBS := $(GRAPHICS_SRCS:$(GRAPHICS_DIR)/%.cpp=$(LIB_DIR)/arcade_%.so)

GAMES_SRCS := $(shell find $(GAMES_DIR) -name '*.cpp')
GAMES_LIBS := $(GAMES_SRCS:$(GAMES_DIR)/%.cpp=$(LIB_DIR)/arcade_%.so)

# === COLORS ===
GREEN   := $(shell echo -e "\033[0;32m")
RED     := $(shell echo -e "\033[0;31m")
VIOLET  := $(shell echo -e "\033[0;35m")
BLUE    := $(shell echo -e "\033[0;34m")
NC      := $(shell echo -e "\033[0m")

# === RULES ===
all: core graphicals games
	@echo "$(GREEN)[OK] Full build complete.$(NC)"

core: $(NAME)

graphicals: $(GRAPHICS_LIBS)
	@echo "$(GREEN)[OK] Graphics libraries built.$(NC)"

games: $(GAMES_LIBS)
	@echo "$(GREEN)[OK] Game libraries built.$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(SILENT)$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(CORE_OBJS)
	$(SILENT)$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "$(GREEN)[OK] Core built.$(NC)"

$(LIB_DIR)/arcade_%.so: $(GRAPHICS_DIR)/%.cpp | $(LIB_DIR)
	-$(SILENT)$(CXX) $(CXXFLAGS) -shared $< -o $@ $(SFML_LIBS) $(NCURSES_LIBS) $(SDL2_LIBS)

$(LIB_DIR)/arcade_%.so: $(GAMES_DIR)/%.cpp | $(LIB_DIR)
	-$(SILENT)$(CXX) $(CXXFLAGS) -shared $< -o $@

$(OBJ_DIR):
	$(SILENT)mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	$(SILENT)mkdir -p $(LIB_DIR)

clean:
	$(SILENT)$(RM) -r $(OBJ_DIR)
	@echo "$(VIOLET)[CLEAN] Object files removed.🧹$(NC)"

fclean: clean
	$(SILENT)$(RM) $(NAME) $(GRAPHICS_LIBS) $(GAMES_LIBS)
	@echo "$(VIOLET)[FCLEAN] Binaries and libs removed.🧹$(NC)"

re: fclean all

.PHONY: all clean fclean re core graphicals games