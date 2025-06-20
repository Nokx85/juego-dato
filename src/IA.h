#pragma once
#include "Tablero.h"
#include "Carta.h"
#include <vector>
#include <array>

// Representa un movimiento concreto sobre el tablero
struct MovimientoIA {
    int filaOrigen;
    int columnaOrigen;
    int filaDestino;
    int columnaDestino;
    int indiceCarta;
};

class IA {
public:
    IA();  // Constructor de la IA

    
    // Calcula el mejor movimiento actual considerando las cartas del jugador y del enemigo
    MovimientoIA obtenerMejorMovimiento(const Tablero& tablero,const std::array<Carta*,2>& cartasJugador,const std::array<Carta*,2>& cartasEnemigo,int profundidad, char jugadorColor);

    // Método para evaluar el estado del tablero (devuelve un puntaje el mas alto lo devuelve)
    int evaluarEstado(const Tablero& tablero, char jugadorColor);

private:
    // Implementación de Minimax con poda alfa-beta
    int minimax(const Tablero& tablero, int profundidad, int alpha,  int beta,  bool esMaximizador,  char jugadorColor,  const std::array<Carta*,2>& cartasJugador, const std::array<Carta*,2>& cartasEnemigo);

    // Obtiene todos los movimientos posibles para un jugador
    std::vector<MovimientoIA> obtenerMovimientosPosibles(const Tablero& tablero, char jugadorColor, const std::array<Carta*,2>& cartas);

    // Guarda el estado del tablero para poder hacer simulaciones de movimientos
    std::vector<Tablero> estadosFuturos;  

    // Evaluar la importancia de las posiciones de las piezas
    int evaluarPosicion(const Tablero& tablero, char jugadorColor);
};