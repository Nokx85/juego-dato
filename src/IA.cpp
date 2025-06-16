
#include "IA.h"
#include <vector>
#include <iostream>

Movimiento IA::obtenerMejorMovimiento(const Tablero& tablero, int profundidad, char jugadorColor) {
    int mejorValor = -9999;  // Un valor muy bajo al principio
    Movimiento mejorMovimiento;

    // Obtiene todos los movimientos posibles para el jugador
    std::vector<Movimiento> movimientos = obtenerMovimientosPosibles(tablero, jugadorColor);

    // Explora cada movimiento y evalúa su valor
    for (const Movimiento& mov : movimientos) {
        // Simula el movimiento en el tablero
        Tablero nuevoTablero = tablero;
        nuevoTablero.moverFicha(mov.getFilaOrigen(), mov.getColumnaOrigen(), mov.getFilaDestino(), mov.getColumnaDestino());

        // Usamos Minimax con poda alfa-beta para obtener el puntaje
        int valor = minimax(nuevoTablero, profundidad, -9999, 9999, false, jugadorColor);
        if (valor > mejorValor) {
            mejorValor = valor;
            mejorMovimiento = mov;
        }
    }

    return mejorMovimiento;
}


std::vector<Movimiento> IA::obtenerMovimientosPosibles(const Tablero& tablero, char jugadorColor) {
    std::vector<Movimiento> movimientosPosibles;

    // Recorre el tablero y obtiene los movimientos de las cartas del jugador
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            Ficha* ficha = tablero.getPosicionFicha(i, j);
            if (ficha != nullptr && ficha->getDueno() == jugadorColor) {
                Carta* carta = ficha->getCarta();  // Obtenemos la carta asociada a la ficha

                // Agregamos los movimientos posibles de la carta al vector
                for (int k = 0; k < carta->getCantidadMovimientos(); ++k) {
                    Movimiento mov = carta->getMovimiento(k);
                    movimientosPosibles.push_back(mov);  // Añadimos el movimiento al vector
                }
            }
        }
    }

    return movimientosPosibles;
}