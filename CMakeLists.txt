cmake_minimum_required(VERSION 3.10)
project(arcade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-gnu-unique -Wall -Wextra")

# Trouver SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)


# Créer le répertoire lib s'il n'existe pas
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Définir les chemins d'inclusion
include_directories(
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/src
)

# Ajouter l'exécutable principal
add_executable(arcade
    src/main.cpp
    src/graphicals/sfml/SFMLGraphics.cpp
    src/graphicals/IGraphics.cpp
    # Ajoutez ici tous les fichiers source du core
)

# Créer la bibliothèque Snake
add_library(arcade_snake SHARED
    src/games/snake/Snake.cpp
    src/games/snake/SnakeModule.cpp
)

# Définir le dossier de sortie pour Snake
set_target_properties(arcade_snake PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

# Créer la bibliothèque SFML
add_library(arcade_sfml SHARED
    src/graphicals/sfml/SFMLGraphics.cpp
)

# Lier SFML
target_link_libraries(arcade_sfml
    sfml-graphics
    sfml-window
    sfml-system
)

# Définir le dossier de sortie pour SFML
set_target_properties(arcade_sfml PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

# Ajouter les règles spécifiques du sujet
add_custom_target(core
    DEPENDS arcade
)

add_custom_target(games
    DEPENDS arcade_snake
    # Ajouter d'autres jeux ici
)

add_custom_target(graphicals
    DEPENDS arcade_sfml
    # Ajouter d'autres bibliothèques graphiques ici
)

# Ajouter les règles de nettoyage
add_custom_target(clean_all
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
)

add_custom_target(fclean
    DEPENDS clean_all
    COMMAND ${CMAKE_COMMAND} -E remove -f arcade
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/lib/arcade_*.so
)

add_custom_target(re
    DEPENDS fclean
    COMMAND ${CMAKE_MAKE_PROGRAM}
)

# Tests
# enable_testing()
# add_executable(unit_tests
#     tests/test_sfml_graphics.cpp
#     src/graphicals/sfml/SFMLGraphics.cpp
# )
# 
# target_link_libraries(unit_tests
#     sfml-graphics
#     sfml-window
#     sfml-system
# )

# Inclure les dossiers d'en-têtes
target_include_directories(arcade PRIVATE
    ${PROJECT_SOURCE_DIR}/includes
    ${PROJECT_SOURCE_DIR}/src
)

# Lier les bibliothèques SFML
target_link_libraries(arcade
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)