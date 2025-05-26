#include "Ficha.h"

class Tablero {

public:

    static const int Filas = 5;
    static const int Columnas = 5;



    Tablero();  // crea el tablero de n filas y m columnas 

    void colocarFicha(Ficha* ficha, int fila, int columna);
    // coloca una ficha por primera ves (la inicialisa) (iniciar juego)

    bool enRango(int fila, int columna) const;
    // valida si se puede mover a esta poscicion

    bool movimientoValidos(int filaOrigen, int colOrigen, int filaDestino, int colDestino);
    // muestra los movimientos posibles 

    void moverFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino);
    // mueve una ficha de un punto a otro (moviento)

    void eliminarFicha(int fila, int columna);
    // elimina una ficha ( por captura o victoria)

    Ficha* getPosicionFicha(int fila, int columna) const;
    // devuelve cual ficha esta en esta poscicion del tablero 

    void mostrarTablero()const;
    // muestra la poscicion actual del tablero


private:
    Ficha* casillas[Filas][Columnas];


};