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

#include "GameState.hpp"

#include <iostream>
#include <vector>
#include <limits>

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
        while (!(std::cin >> indiceCarta) || (indiceCarta != 0 && indiceCarta != 1)) {
            std::cout << "Esa carta no es valida. Intenta otra vez: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        Carta* cartaUsada = jugador->cartas[indiceCarta];

        std::cout << "Movimientos posibles de la carta elegida:\n";
        cartaUsada->mostrarMovimientos();

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
        while (!(std::cin >> eleccionFicha) ||
               eleccionFicha < 0 || eleccionFicha >= posiciones.size()) {
            std::cout << "Numero invalido. Intenta otra vez: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int filaOrigen = posiciones[eleccionFicha].first;
        int colOrigen = posiciones[eleccionFicha].second;

        Ficha* ficha = tablero.getPosicionFicha(filaOrigen, colOrigen);
        if (ficha == nullptr || ficha->getDueno() != jugador->getColor()) {
            std::cout << "No puedes mover esa ficha. Intenta de nuevo.\n";
            continue;
        }

        tablero.mostrarMovimientosPosibles(filaOrigen, colOrigen, cartaUsada, jugador->getColor());

        // Mostrar y guardar los movimientos válidos numerados
        std::vector<std::pair<int, int>> movimientosValidos;
        std::cout << "\nMovimientos posibles desde esa ficha:\n";
        for (int i = 0; i < cartaUsada->getCantidadMovimientos(); ++i) {
            Movimiento mov = cartaUsada->getMovimiento(i);
            int filaDestino = filaOrigen + mov.dx;
            int colDestino = colOrigen + mov.dy;

            if (tablero.movimientoValidos(filaOrigen, colOrigen, filaDestino, colDestino)) {
                std::cout << movimientosValidos.size() << ": a (" << filaDestino << "," << colDestino << ")\n";
                movimientosValidos.push_back({filaDestino, colDestino});
            }
        }

        if (movimientosValidos.empty()) {
            std::cout << "\nNo hay movimientos válidos para esta ficha. Elige otra.\n";
            continue;
        }

        std::cout << "Elige el numero del movimiento que quieres usar: ";
        int movIndex;
        while (!(std::cin >> movIndex) || movIndex < 0 || movIndex >= movimientosValidos.size()) {
            std::cout << "Movimiento invalido. Intenta otra vez: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int filaDestino = movimientosValidos[movIndex].first;
        int colDestino = movimientosValidos[movIndex].second;

        tablero.moverFicha(filaOrigen, colOrigen, filaDestino, colDestino);

        if (tablero.verificarReyEnDojo()) {
            std::cout << "\nEl juego ha terminado porque un rey llego al dojo enemigo.\n";
            juegoTerminado = true;
            continue;
        }

        turnoRojo = !turnoRojo;
        jugador->usarCarta(indiceCarta, cartaCentro);
    }

    std::cout << "\nGracias por jugar.\n";
    return 0;
}
