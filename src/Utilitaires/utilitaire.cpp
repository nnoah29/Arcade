/*
**  _                                              _      ___    ___  
** | |                                            | |    |__ \  / _ \
** | |_Created _       _ __   _ __    ___    __ _ | |__     ) || (_) |
** | '_ \ | | | |     | '_ \ | '_ \  / _ \  / _` || '_ \   / /  \__, |
** | |_) || |_| |     | | | || | | || (_) || (_| || | | | / /_    / / 
** |_.__/  \__, |     |_| |_||_| |_| \___/  \__,_||_| |_||____|  /_/ 
**          __/ |     on 01/04/25.                                          
**         |___/ 
*/

#include <iostream>
#include <memory>
#include <random>

int random(int a, int b)
{
    static std::random_device rd;  // Générateur de nombres aléatoires basé sur le matériel
    static std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_int_distribution<int> distrib(a, b); // Distribution uniforme entre 1 et 4

    return distrib(gen);

    std::unique_ptr<int> nb;
}

void bof (int *i)
{
    *i = 4;
}