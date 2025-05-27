#include "Carta.h"
#include "CartaDragon.h"
#include "CartaFenix.h"
#include "CartaLeon.h"
#include "CartaOso.h"
#include "CartaTigre.h"
#include "Ficha.h"
#include "Tablero.h"
#include "Rey.h"
#include "Peon.h"
#include "Jugador.h"
#include <iostream>
#include <vector>

int main() {
    Tablero tablero;

    // Crear jugadores y cartas
    Jugador jugadorAzul('a');
    Jugador jugadorRojo('r');

    jugadorAzul.cartas[0] = new CartaLeon();
    jugadorAzul.cartas[1] = new CartaFenix();

    jugadorRojo.cartas[0] = new CartaTigre();
    jugadorRojo.cartas[1] = new CartaOso();

    Carta* cartaCentro = new CartaDragon();

    // Colocar fichas en el tablero (solo 2 peones por equipo)
    tablero.colocarFicha(new Rey('a'), 0, 2);
    tablero.colocarFicha(new Rey('r'), 4, 2);

    tablero.colocarFicha(new Peon('a'), 0, 1);
    tablero.colocarFicha(new Peon('a'), 0, 3);
    tablero.colocarFicha(new Peon('r'), 4, 1);
    tablero.colocarFicha(new Peon('r'), 4, 3);

    bool juegoTerminado = false;
    bool turnoRojo = true;

    while (!juegoTerminado) {
        tablero.mostrarTablero();
        std::cout << "\nCarta en el centro (bonus):\n";
        cartaCentro->mostrarMovimientos();

        Jugador* jugador;
        if (turnoRojo) {
            std::cout << "\nTurno del jugador ROJO\n";
            jugador = &jugadorRojo;
        } else {
            std::cout << "\nTurno del jugador AZUL\n";
            jugador = &jugadorAzul;
        }

        jugador->mostrarCartas();

        std::cout << "Elige una carta (0 o 1): ";
        int indiceCarta;
        std::cin >> indiceCarta;
        while (indiceCarta != 0 && indiceCarta != 1) {
            std::cout << "Esa carta no es valida. Intenta otra vez: ";
            std::cin >> indiceCarta;
        }

        Carta* cartaUsada = jugador->cartas[indiceCarta];
        cartaUsada->mostrarMovimientos();

        // Mostrar fichas disponibles y enumerarlas
        std::vector<std::pair<int, int>> posiciones;
        std::cout << "Fichas disponibles para mover:\n";
        int index = 0;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                Ficha* f = tablero.getPosicionFicha(i, j);
                if (f != nullptr && f->getDueno() == jugador->getColor()) {
                    std::cout << index << ": " << f->getTipo() << " en (" << i << "," << j << ")\n";
                    posiciones.push_back({i, j});
                    ++index;
                }
            }
        }

        int eleccionFicha = -1;
        std::cout << "Elige el numero de la ficha que quieres mover: ";
        std::cin >> eleccionFicha;
        while (eleccionFicha < 0 || eleccionFicha >= posiciones.size()) {
            std::cout << "Numero invalido. Intenta otra vez: ";
            std::cin >> eleccionFicha;
        }

        int filaOrigen = posiciones[eleccionFicha].first;
        int colOrigen = posiciones[eleccionFicha].second;

        Ficha* ficha = tablero.getPosicionFicha(filaOrigen, colOrigen);
        if (ficha == nullptr || ficha->getDueno() != jugador->getColor()) {
            std::cout << "No puedes mover esa ficha. Intenta de nuevo.\n";
            continue;
        }

        std::cout << "Elige el numero del movimiento que quieres usar (0 hasta " << cartaUsada->getCantidadMovimientos()-1 << "): ";
        int movIndex;
        std::cin >> movIndex;
        while (movIndex < 0 || movIndex >= cartaUsada->getCantidadMovimientos()) {
            std::cout << "Ese movimiento no es valido. Intenta otra vez: ";
            std::cin >> movIndex;
        }

        Movimiento mov = cartaUsada->getMovimiento(movIndex);

        // Los movimientos se invierten para el jugador rojo porque esta abajo
        int direccion = (jugador->getColor() == 'r') ? -1 : 1;

        int filaDestino = filaOrigen + mov.dx * direccion;
        int colDestino = colOrigen + mov.dy;

        if (tablero.movimientoValidos(filaOrigen, colOrigen, filaDestino, colDestino)) {
            tablero.moverFicha(filaOrigen, colOrigen, filaDestino, colDestino);

            if (tablero.verificarReyEnDojo()) {
                std::cout << "\nEl juego ha terminado porque un rey llego al dojo enemigo.\n";
                juegoTerminado = true;
                continue;
            }

            jugador->usarCarta(indiceCarta, cartaCentro);
        } else {
            std::cout << "\nEse movimiento no esta permitido. Intenta de nuevo.\n";
            continue;
        }

        turnoRojo = !turnoRojo;
    }

    std::cout << "\nGracias por jugar.\n";
    return 0;
}
