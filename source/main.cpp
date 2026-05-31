/*
** EPITECH PROJECT, 2026
** Arcade
** File description:
** main
*/

#include "Arcade.hpp"

int main(int ac, char* av[])
{
    try {
        arcade::Arcade arcade(ac, av);
        arcade.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    }
    return 0;
}