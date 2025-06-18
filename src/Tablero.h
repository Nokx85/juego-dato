#pragma once


#include "Ficha.h"
#include "Carta.h"

#include <vector>


class Tablero {

public:

    static const int Filas = 5;
    static const int Columnas = 5;



    Tablero();  // crea el tablero de n filas y m columnas 

    void colocarFicha(Ficha* ficha, int fila, int columna);
    // coloca una ficha por primera ves (la inicialisa) (iniciar juego)

    bool enRango(int fila, int columna) const;
    // valida si se puede mover a esta poscicion

   bool movimientoValidos(int filaOrigen, int colOrigen,int filaDestino, int colDestino) const;

    char moverFicha(int filaOrigen, int colOrigen,int filaDestino, int colDestino,bool &juegoTerminado);

    char eliminarFicha(int fila, int columna, bool &juegoTerminado);
    // elimina una ficha ( por captura o victoria)

    Ficha* getPosicionFicha(int fila, int columna) const;
    // devuelve cual ficha esta en esta poscicion del tablero 

    // Muestra las fichas del jugador y almacena sus posiciones
    void mostrarFichasJugador(char jugadorColor,std::vector<std::pair<int,int>>& posiciones) const;

    void mostrarTablero()const;
    // muestra la poscicion actual del tablero
    
    bool verificarReyEnDojo() const;

    void mostrarMovimientosPosibles(int fila, int columna, const Carta* carta, char jugadorColor) const;


private:
    Ficha* casillas[Filas][Columnas];


};