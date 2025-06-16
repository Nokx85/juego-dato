#pragma once
#include "Tablero.h" 
#include "Carta.h" 



#include <vector>



class IA {
public:
    IA();  // Constructor de la IA

    // Obtiene el mejor movimiento usando Minimax con poda alfa-beta
    Movimiento obtenerMejorMovimiento(const Tablero& tablero, int profundidad, char jugadorColor);  // Calcular el mejor movimiento


    // Método para evaluar el estado del tablero (básico, se puede mejorar)
    int evaluarEstado(const Tablero& tablero, char jugadorColor);

private:
    // Implementación básica de Minimax con poda alfa-beta
    int minimax(const Tablero& tablero, int profundidad, int alpha, int beta, bool esMaximizador, char jugadorColor);

    // Obtiene todos los movimientos posibles para un jugador
    std::vector<Movimiento> obtenerMovimientosPosibles(const Tablero& tablero, char jugadorColor);

    // Almacena el estado del tablero para poder hacer simulaciones de movimientos
    std::vector<Tablero> estadosFuturos;  // Esto guarda los estados del tablero tras realizar un movimiento

    // Evaluar la importancia de las posiciones de las piezas
    int evaluarPosicion(const Tablero& tablero, char jugadorColor);

    // Puede agregar más métodos aquí si necesitas optimizar más el algoritmo
};