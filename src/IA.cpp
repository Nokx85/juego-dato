#include "IA.h"
#include <vector>
#include <iostream>
#include <algorithm>

IA::IA() {}

MovimientoIA IA::obtenerMejorMovimiento(const Tablero& tablero, const std::array<Carta*, 2>& cartas,int profundidad, char jugadorColor) {
    int mejorValor = -99999;
    MovimientoIA mejorMovimiento{0,0,0,0};

    std::vector<MovimientoIA> movimientos = obtenerMovimientosPosibles(tablero, jugadorColor, cartas);
    for (const MovimientoIA& mov : movimientos) {
        Tablero nuevoTablero = tablero;
        bool terminado = false;
        nuevoTablero.moverFicha(mov.filaOrigen, mov.columnaOrigen,
                                mov.filaDestino, mov.columnaDestino, terminado);
        int valor = minimax(nuevoTablero, profundidad - 1,-99999,99999,false,jugadorColor,cartas,cartas);
        if (valor > mejorValor) {
            mejorValor = valor;
            mejorMovimiento = mov;
        }
    }

    return mejorMovimiento;
}

std::vector<MovimientoIA> IA::obtenerMovimientosPosibles(const Tablero& tablero, char jugadorColor, const std::array<Carta*,2>& cartas) {
    std::vector<MovimientoIA> movimientosPosibles;

    for (int i = 0; i < Tablero::Filas; ++i) {
        for (int j = 0; j < Tablero::Columnas; ++j) {
            Ficha* ficha = tablero.getPosicionFicha(i, j);
            if (ficha != nullptr && ficha->getDueno() == jugadorColor) {
                for (Carta* carta : cartas) {
                    for (int k = 0; k < carta->getCantidadMovimientos(); ++k) {
                        Movimiento mov = carta->getMovimiento(k);
                        int nf = i + mov.dx;
                        int nc = j + mov.dy;
                        if (tablero.movimientoValidos(i, j, nf, nc)) {
                            movimientosPosibles.push_back({i, j, nf, nc});
                        }
                    }
                }
            }
        }
    }

    return movimientosPosibles;
}

int IA::minimax(const Tablero& tablero, int profundidad,int alpha,int beta,bool esMaximizador,char jugadorColor,const std::array<Carta*,2>& cartasJugador, const std::array<Carta*,2>& cartasEnemigo) {
    if (profundidad == 0) {
        return evaluarEstado(tablero, jugadorColor);
    }

    char enemigo = (jugadorColor == 'r') ? 'a' : 'r';
    char colorActual = esMaximizador ? jugadorColor : enemigo;

    const std::array<Carta*,2>& cartasActual = esMaximizador ? cartasJugador : cartasEnemigo;
    std::vector<MovimientoIA> movimientos = obtenerMovimientosPosibles(tablero, colorActual,cartasActual);
    if (movimientos.empty()) {
        return evaluarEstado(tablero, jugadorColor);
    }

    int mejorValor = esMaximizador ? -99999 : 99999;

    for (const MovimientoIA& mov : movimientos) {
        Tablero nuevo = tablero;
        bool terminado = false;
        nuevo.moverFicha(mov.filaOrigen, mov.columnaOrigen,
                         mov.filaDestino, mov.columnaDestino, terminado);
        int valor = minimax(nuevo, profundidad - 1, alpha, beta, !esMaximizador, jugadorColor,  cartasJugador, cartasEnemigo);
        if (esMaximizador) {
            mejorValor = std::max(mejorValor, valor);
            alpha = std::max(alpha, valor);
        } else {
            mejorValor = std::min(mejorValor, valor);
            beta = std::min(beta, valor);
        }
        if (beta <= alpha)
            break;
    }
    return mejorValor;
}

int IA::evaluarEstado(const Tablero& tablero, char jugadorColor) {
    return evaluarPosicion(tablero, jugadorColor);
}

int IA::evaluarPosicion(const Tablero& tablero, char jugadorColor) {
    int puntuacion = 0;
    for (int i = 0; i < Tablero::Filas; ++i) {
        for (int j = 0; j < Tablero::Columnas; ++j) {
            Ficha* f = tablero.getPosicionFicha(i, j);
            if (!f) continue;
            int valor = (f->getTipo() == 'R') ? 10 : 1;
            if (f->getDueno() == jugadorColor)
                puntuacion += valor;
            else
                puntuacion -= valor;
        }
    }
    return puntuacion;
}
