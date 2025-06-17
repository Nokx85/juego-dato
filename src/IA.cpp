#include "IA.h"
#include <vector>
#include <iostream>
#include <algorithm>

IA::IA() {}

MovimientoIA IA::obtenerMejorMovimiento(const Tablero& tablero,
                                        const std::array<Carta*, 2>& cartasJugador,
                                        const std::array<Carta*, 2>& cartasEnemigo,
                                        int profundidad, char jugadorColor) {
    int mejorValor = -99999;
    MovimientoIA mejorMovimiento{0,0,0,0};
    
    // Asegurémonos de que esta variable esté definida
    std::vector<Tablero> estadosFuturos;

    std::vector<MovimientoIA> movimientos = obtenerMovimientosPosibles(tablero, jugadorColor, cartasJugador);
    for (const MovimientoIA& mov : movimientos) {
        Tablero nuevoTablero = tablero;
        bool terminado = false;
        nuevoTablero.moverFicha(mov.filaOrigen, mov.columnaOrigen,
                                mov.filaDestino, mov.columnaDestino, terminado);
        estadosFuturos.push_back(nuevoTablero);
        int valor = minimax(nuevoTablero, profundidad - 1, -99999, 99999, false,
                           jugadorColor, cartasJugador, cartasEnemigo);
        estadosFuturos.pop_back();
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

int IA::minimax(const Tablero& tablero, int profundidad, int alpha, int beta, bool esMaximizador, char jugadorColor, const std::array<Carta*,2>& cartasJugador, const std::array<Carta*,2>& cartasEnemigo) {
    if (profundidad == 0) {
        return evaluarEstado(tablero, jugadorColor);
    }

    char enemigo = (jugadorColor == 'r') ? 'a' : 'r';
    char colorActual = esMaximizador ? jugadorColor : enemigo;

    const std::array<Carta*,2>& cartasActual = esMaximizador ? cartasJugador : cartasEnemigo;
    std::vector<MovimientoIA> movimientos = obtenerMovimientosPosibles(tablero, colorActual, cartasActual);
    if (movimientos.empty()) {
        return evaluarEstado(tablero, jugadorColor);
    }

    int mejorValor = esMaximizador ? -99999 : 99999;

    for (const MovimientoIA& mov : movimientos) {
        Tablero nuevo = tablero;
        bool terminado = false;
        nuevo.moverFicha(mov.filaOrigen, mov.columnaOrigen,
                         mov.filaDestino, mov.columnaDestino, terminado);
        // Asegurémonos de que esta variable esté definida
        std::vector<Tablero> estadosFuturos;
        estadosFuturos.push_back(nuevo);
        int valor = minimax(nuevo, profundidad - 1, alpha, beta, !esMaximizador,
                             jugadorColor, cartasJugador, cartasEnemigo);
        estadosFuturos.pop_back();
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

// Evaluación del estado del tablero
int IA::evaluarEstado(const Tablero& tablero, char jugadorColor) {
    // Verificación de victoria o derrota inmediata
    Ficha* enDojoAzul = tablero.getPosicionFicha(0, 2);
    if (enDojoAzul && enDojoAzul->getTipo() == 'R') {
        if (enDojoAzul->getDueno() == jugadorColor)
            return 10000;              // Victoria inmediata
        else
            return -10000;             // Derrota inmediata
    }

    Ficha* enDojoRojo = tablero.getPosicionFicha(4, 2);
    if (enDojoRojo && enDojoRojo->getTipo() == 'R') {
        if (enDojoRojo->getDueno() == jugadorColor)
            return 10000;
        else
            return -10000;
    }

    // Verificación de si un rey ha sido capturado
    bool reyAzulPresente = false;
    bool reyRojoPresente = false;
    for (int i = 0; i < Tablero::Filas; ++i) {
        for (int j = 0; j < Tablero::Columnas; ++j) {
            Ficha* f = tablero.getPosicionFicha(i, j);
            if (f && f->getTipo() == 'R') {
                if (f->getDueno() == 'a') reyAzulPresente = true;
                if (f->getDueno() == 'r') reyRojoPresente = true;
            }
        }
    }

    if (!reyAzulPresente)
        return (jugadorColor == 'r') ? 10000 : -10000;
    if (!reyRojoPresente)
        return (jugadorColor == 'a') ? 10000 : -10000;

    // Evaluación de la posición si nadie ha ganado
    return evaluarPosicion(tablero, jugadorColor);
}

int IA::evaluarPosicion(const Tablero& tablero, char jugadorColor) {
    int puntuacion = 0;
    char enemigo = (jugadorColor == 'r') ? 'a' : 'r';

    Ficha* reyJugador = nullptr;
    Ficha* reyEnemigo = nullptr;
    std::vector<std::pair<int, int>> piezasJugador;
    std::vector<std::pair<int, int>> piezasEnemigo;

    for (int i = 0; i < Tablero::Filas; ++i) {
        for (int j = 0; j < Tablero::Columnas; ++j) {
            Ficha* f = tablero.getPosicionFicha(i, j);
            if (!f) continue;
            int valor = (f->getTipo() == 'R') ? 10 : 1;
            if (f->getDueno() == jugadorColor) {
                puntuacion += valor;
                piezasJugador.push_back({i, j});
                if (f->getTipo() == 'R') reyJugador = f;
            } else {
                puntuacion -= valor;
                piezasEnemigo.push_back({i, j});
                if (f->getTipo() == 'R') reyEnemigo = f;
            }
        }
    }

    // Evaluación de la proximidad del rey propio al dojo enemigo
    if (reyJugador) {
        int destinoFila = (jugadorColor == 'r') ? 0 : 4;
        int dist = std::abs(reyJugador->getFila() - destinoFila) +
                   std::abs(reyJugador->getColumna() - 2);
        puntuacion += (4 - dist) * 3; // cuanto más cerca mejor

        // Evaluar peligro: enemigo adyacente
        for (const auto& pos : piezasEnemigo) {
            if (std::abs(pos.first - reyJugador->getFila()) <= 1 &&
                std::abs(pos.second - reyJugador->getColumna()) <= 1) {
                puntuacion -= 20;
                break;
            }
        }
    }

    // Evaluación de la proximidad del rey enemigo a nuestro dojo
    if (reyEnemigo) {
        int destinoFila = (jugadorColor == 'r') ? 4 : 0;
        int dist = std::abs(reyEnemigo->getFila() - destinoFila) +
                   std::abs(reyEnemigo->getColumna() - 2);
        puntuacion -= (4 - dist) * 3;

        // Amenazar al rey enemigo
        for (const auto& pos : piezasJugador) {
            if (std::abs(pos.first - reyEnemigo->getFila()) <= 1 &&
                std::abs(pos.second - reyEnemigo->getColumna()) <= 1) {
                puntuacion += 20;
                break;
            }
        }
    }

    return puntuacion;
}
