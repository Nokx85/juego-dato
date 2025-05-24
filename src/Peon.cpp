
#include "../Include/Peon.h"

Peon::Peon(int fila, int columna) : 
Ficha('P',fila,columna,false){


}

void Peon::moverFicha(int nuevaFila, int nuevaColumna){

    Ficha::moverFicha(nuevaFila ,nuevaColumna);
}




