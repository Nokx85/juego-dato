#include "Ficha.h"

const int Filas = 5;
const int Columnas = 5;


class Tablero {
private:
    Ficha* casillas[Filas][Columnas];

public:
    Tablero();  // crea el tablero de n filas y m columnas 

    void colocarFicha(Ficha* ficha, int fila, int columna);
    // coloca una ficha por primera ves (la inicialisa) (iniciar juego)

    void moverFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino);
    // mueve una ficha de un punto a otro (moviento)

    void eliminarFicha(int fila, int columna);
    // elimina una ficha ( por captura o victoria)

    Ficha* getFicha(int fila, int columna) const;
    // devuelve cual ficha esta en esta poscicion del tablero 

    void mostrarTablero()const;
    // muestra la poscicion actual del tablero

};