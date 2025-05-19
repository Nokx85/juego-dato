#include "Peon.h"

Peon::Peon(int fila, int columna) : 
Ficha('P',fila,columna){

}

void Peon::moverFicha(int nuevaFila, int nuevaColumna){

    Ficha::moverFicha(nuevaFila ,nuevaColumna);
}




