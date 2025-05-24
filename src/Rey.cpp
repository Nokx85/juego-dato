
#include "../Include/Rey.h"

Rey::Rey(int fila, int columna): Ficha('R',fila,columna,true){

}

void Rey::moverFicha(int nuevaFila, int nuevaColumna){

    Ficha::moverFicha(nuevaFila ,nuevaColumna);
}


