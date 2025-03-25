NAME = arcade

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
INCLUDE_DIR = includes

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -fno-gnu-unique -I$(INCLUDE_DIR) -I$(SRC_DIR)

LDFLAGS = -ldl
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

MAIN_SRC = $(SRC_DIR)/main.cpp \
           $(SRC_DIR)/graphicals/sfml/SFMLGraphics.cpp \

MAIN_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRC))

SNAKE_LIB = $(LIB_DIR)/arcade_snake.so
SFML_LIB = $(LIB_DIR)/arcade_sfml.so

all: core games graphicals

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/graphicals/sfml
	@mkdir -p $(LIB_DIR)

core: $(NAME)

$(NAME): $(MAIN_OBJ) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(SFML_LIBS)
	@echo "✅ Compilation du core terminée"

games: $(SNAKE_LIB)

$(SNAKE_LIB): $(SRC_DIR)/games/snake/Snake.cpp $(SRC_DIR)/games/snake/SnakeModule.cpp | $(LIB_DIR)
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $^ $(LDFLAGS)
	@echo "✅ Compilation de la bibliothèque Snake terminée"

graphicals: $(SFML_LIB)

$(SFML_LIB): $(SRC_DIR)/graphicals/sfml/SFMLGraphics.cpp | $(LIB_DIR)
	$(CXX) $(CXXFLAGS) -shared -fPIC -o $@ $^ $(SFML_LIBS)
	@echo "✅ Compilation de la bibliothèque SFML terminée"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "🧹 Nettoyage des fichiers objets"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(LIB_DIR)/*.so
	@echo "🧹 Nettoyage complet"

re: fclean all

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)
