﻿#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Classes.h"

int main() {
    gamemanager game;
    if (!piece::initResources()) return EXIT_FAILURE;

    game.getwindow().setFramerateLimit(60);
    while (game.getwindow().isOpen()) {
        game.startscreen();
        while (game.getrunning() && game.getwindow().isOpen())
        {
            game.mainloop();
        }
        game.losescreen();
    }
    game.getwindow().close();
    return 0;
}