# 🎮 Arcade — A Retro Game Platform (B-OOP-400)

## 📝 Description

Arcade est une plateforme de jeux rétro modulaire permettant de jouer à différents jeux classiques avec différentes bibliothèques graphiques. Le projet est conçu avec une architecture plug-and-play qui permet de changer de jeu ou de bibliothèque graphique en temps réel sans redémarrer le programme.

## ✨ Fonctionnalités

- 🔄 Changement dynamique de bibliothèque graphique en cours de jeu
- 🎲 Plusieurs jeux classiques disponibles
- 🎛️ Interface utilisateur intuitive
- 🔌 Architecture modulaire extensible

## 🛠️ Prérequis

- 🧠 Connaissances en C++ orienté objet
- 📚 SFML >= 2.5
- 📚 nCurses
- 📚 SDL2
- 📚 GTK+

### Installation des dépendances

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install g++ libsfml-dev libncurses5-dev libsdl2-dev libgtk-3-dev

# Fedora
sudo dnf install gcc -c++ SFML-devel ncurses-devel SDL2-devel gtk3-devel

# Arch Linux
sudo pacman -S gcc sfml ncurses sdl2 gtk3
```

## 🚀 Compilation

```bash
# Compilation complète
make re           # Recompiler tout le projet
make core         # Compiler uniquement le cœur (arcade)
make games        # Compiler uniquement les jeux
make graphicals   # Compiler uniquement les interfaces graphiques
make fclean       # Nettoyer les binaires et les librairies
```

## 🎮 Utilisation

```bash
# Lancement basique
./arcade ./lib/arcade_sfml.so ./lib/arcade_snake.so

# Autres exemples
./arcade ./lib/arcade_ncurses.so ./lib/arcade_pacman.so
./arcade ./lib/arcade_sdl2.so ./lib/arcade_nibbler.so
```

⚠️ **Note** : Le premier argument doit être une bibliothèque graphique valide (.so)

### 🎹 Contrôles

| Touche | Action |
|--------|--------|
| ↑↓←→ | Déplacer |
| R | Redémarrer |
| L | Changer lib graphique |
| G | Changer de jeu |

## 🎯 Jeux disponibles

| Jeu | Description | Difficulté |
|-----|-------------|------------|
| Snake 🐍 | Le classique jeu du serpent | ⭐⭐ |
| Pacman 👻 | Mangez les pac-gums, évitez les fantômes | ⭐⭐⭐ |
| Nibbler 🍎 | Snake version arcade avec labyrinthe | ⭐⭐⭐⭐ |

## 📚 Bibliothèques graphiques

| Bibliothèque | Fichier | Type | Performance |
|--------------|---------|------|----------|
| 🖼️ SFML | arcade_sfml.so | 2D moderne | ⚡⚡⚡ |
| 🧮 NCurses | arcade_ncurses.so | Terminal | ⚡⚡⚡⚡⚡ |
| 🎮 SDL2 | arcade_sdl2.so | 2D optimisée | ⚡⚡⚡⚡ |
| 🖥️ GTK+ | arcade_gtk.so | Interface native |  |

## 🏗️ Architecture

```bash
arcade/
├── arcade              # Exécutable principal
├── lib/                # Bibliothèques dynamiques (.so)
│   ├── games/          # Jeux compilés
│   └── graphics/       # Libs graphiques compilées
├── include/            # Headers publics
│   ├── gameMap.hpp     # Représentation des jeux
│   └── my.hpp          # Input des jeux
├── src/
│   ├── core/           # Implémentation du core
│   ├── games/          # Code source des jeux
│   └── graphics/       # Code source des libs graphiques
└── Makefile            # Build system
```

## 👥 Équipe

| Nom | Rôle | Contact |
|-----|------|---------|
| Chrisnaud AGOSSOU | Chef de projet | chrisnaud.agossou@epitech.eu |
| Christian ABIALA | Développeur | christian.abiala@epitech.eu |
| Noah TOFFA | Développeur | noah.toffa@epitech.eu |

## ⚖️ Licence

Ce projet est développé dans le cadre du module B-OOP-400 à EPITECH.

## ⚠️ Notes importantes

- Les bibliothèques sont chargées dynamiquement au runtime
- Les scores sont se reinitialise après défaite ou changements de bibliothèques
- Le projet suit la norme EPITECH
