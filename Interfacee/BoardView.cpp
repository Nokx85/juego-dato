#include "BoardView.h"

BoardView::BoardView() {}

bool BoardView::loadResources() {
    return font.loadFromFile("files/Minecraft.ttf");
}

void BoardView::draw(sf::RenderWindow& window,
                     const Tablero& tablero,
                     const Jugador& jugadorActual,
                     const Carta* cartaCentro) const {
    const int cellSize = 100;
    const int offsetX = 50;
    const int offsetY = 50;

    // Dibujar tablero de 5x5
    for (int i = 0; i < Tablero::Filas; ++i) {
        for (int j = 0; j < Tablero::Columnas; ++j) {
            sf::RectangleShape cell(sf::Vector2f(cellSize - 2, cellSize - 2));
            cell.setPosition(offsetX + j * cellSize, offsetY + i * cellSize);
            cell.setFillColor(((i + j) % 2 == 0) ? sf::Color(238, 238, 210)
                                                : sf::Color(118, 150, 86));
            window.draw(cell);

            Ficha* ficha = tablero.getPosicionFicha(i, j);
            if (ficha) {
                sf::CircleShape piece(cellSize / 2 - 10);
                piece.setPosition(offsetX + j * cellSize + 10,
                                   offsetY + i * cellSize + 10);
                piece.setFillColor(
                    ficha->getDueno() == 'r' ? sf::Color::Red : sf::Color::Blue);
                window.draw(piece);

                sf::Text letter;
                letter.setFont(font);
                letter.setString(std::string(1, ficha->getTipo()));
                letter.setCharacterSize(20);
                letter.setFillColor(sf::Color::White);
                letter.setPosition(offsetX + j * cellSize + cellSize / 2 - 8,
                                    offsetY + i * cellSize + cellSize / 2 - 12);
                window.draw(letter);
            }
        }
    }

    // Dibujar cartas del jugador actual
    for (int i = 0; i < 2; ++i) {
        sf::RectangleShape card(sf::Vector2f(120, 80));
        card.setPosition(offsetX + i * 140, offsetY + 5 * cellSize + 10);
        card.setFillColor(sf::Color(240, 240, 240));
        card.setOutlineThickness(2);
        card.setOutlineColor(sf::Color::Black);
        window.draw(card);

        sf::Text name;
        name.setFont(font);
        name.setCharacterSize(16);
        name.setFillColor(sf::Color::Black);
        name.setString(jugadorActual.cartas[i]->getNombre());
        name.setPosition(card.getPosition().x + 10, card.getPosition().y + 30);
        window.draw(name);
    }

    // Texto del turno
    sf::Text turno;
    turno.setFont(font);
    turno.setCharacterSize(20);
    turno.setFillColor(sf::Color::White);
    turno.setString(jugadorActual.getColor() == 'r' ? "Turno: Rojo"
                                                    : "Turno: Azul");
    turno.setPosition(offsetX + 300, offsetY + 5 * cellSize + 20);
    window.draw(turno);

    // Carta en uso/centro
    if (cartaCentro) {
        sf::Text centro;
        centro.setFont(font);
        centro.setCharacterSize(16);
        centro.setFillColor(sf::Color::Yellow);
        centro.setString("Carta centro: " + cartaCentro->getNombre());
        centro.setPosition(offsetX + 300, offsetY + 5 * cellSize + 50);
        window.draw(centro);
    }
}
