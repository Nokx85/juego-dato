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

    Jugador jugadorAzul('a');
    Jugador jugadorRojo('r');

    // Agregar cartas (el orden importa)
    jugadorAzul.agregarCarta(new CartaLeon());
    jugadorAzul.agregarCarta(new CartaFenix());

    jugadorRojo.agregarCarta(new CartaTigre());
    jugadorRojo.agregarCarta(new CartaOso());

    Carta* cartaCentro = new CartaDragon();

    tablero.colocarFicha(new Rey('a'), 0, 2);
    tablero.colocarFicha(new Rey('r'), 4, 2);
    tablero.colocarFicha(new Peon('a'), 0, 1);
    tablero.colocarFicha(new Peon('a'), 0, 3);
    tablero.colocarFicha(new Peon('r'), 4, 1);
    tablero.colocarFicha(new Peon('r'), 4, 3);

    bool juegoTerminado = false;
    bool turnoRojo = false;

    while (!juegoTerminado) {
        tablero.mostrarTablero();
        std::cout << "\nCarta en el centro (bonus):\n";
        cartaCentro->mostrarMovimientos();

        Jugador* jugador = turnoRojo ? &jugadorRojo : &jugadorAzul;
        std::cout << "\nTurno del jugador " << (jugador->getColor() == 'r' ? "ROJO" : "AZUL") << "\n";

        jugador->mostrarCartas();

        // Obtener la carta del frente (la que se debe usar)
        Carta* cartaUsada = jugador->getCartaFrente();

        std::cout << "Usarás esta carta:\n";
        cartaUsada->mostrarMovimientos();

        std::vector<std::pair<int, int>> posiciones;
        std::cout << "Fichas disponibles para mover:\n";
        int index = 0;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                Ficha* f = tablero.getPosicionFicha(i, j);
                if (f && f->getDueno() == jugador->getColor()) {
                    std::cout << index << ": " << f->getTipo() << " en (" << i << "," << j << ")\n";
                    posiciones.push_back({i, j});
                    ++index;
                }
            }
        }

        int eleccionFicha;
        std::cout << "Elige el numero de la ficha que quieres mover: ";
        std::cin >> eleccionFicha;
        while (eleccionFicha < 0 || eleccionFicha >= posiciones.size()) {
            std::cout << "Numero invalido. Intenta otra vez: ";
            std::cin >> eleccionFicha;
        }

        int filaOrigen = posiciones[eleccionFicha].first;
        int colOrigen = posiciones[eleccionFicha].second;

        Ficha* ficha = tablero.getPosicionFicha(filaOrigen, colOrigen);
        if (!ficha || ficha->getDueno() != jugador->getColor()) {
            std::cout << "No puedes mover esa ficha. Intenta de nuevo.\n";
            continue;
        }

        // Mostrar movimientos válidos de esa ficha con esa carta
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

        int movIndex;
        std::cout << "Elige el numero del movimiento que quieres usar: ";
        std::cin >> movIndex;
        while (movIndex < 0 || movIndex >= movimientosValidos.size()) {
            std::cout << "Movimiento invalido. Intenta otra vez: ";
            std::cin >> movIndex;
        }

        int filaDestino = movimientosValidos[movIndex].first;
        int colDestino = movimientosValidos[movIndex].second;

        tablero.moverFicha(filaOrigen, colOrigen, filaDestino, colDestino);

        if (tablero.verificarReyEnDojo()) {
            std::cout << "\nEl juego ha terminado porque un rey llego al dojo enemigo.\n";
            juegoTerminado = true;
            continue;
        }

        // Rotar carta después del movimiento
        jugador->usarCarta(cartaCentro);
        turnoRojo = !turnoRojo;
    }

    std::cout << "\nGracias por jugar.\n";
    return 0;
}

