#pragma once
#include <SFML/Graphics.hpp>
#include "../src/Tablero.h"
#include "../src/Jugador.h"

class BoardView {
public:
    BoardView();
    bool loadResources();
    void draw(sf::RenderWindow& window,
              const Tablero& tablero,
              const Jugador& jugadorActual,
              const Carta* cartaCentro) const;
private:
    sf::Font font;
};
