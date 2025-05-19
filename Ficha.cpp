#include "Ficha.h"


Ficha::Ficha(char tipoFicha,int fila,int columna){
    this->tipoFicha = tipoFicha;
    this->fila = fila;
    this->columna = columna;
}

void Ficha::moverFicha (int nuevaFila, int nuevaColumna){
    fila = nuevaFila;
    columna = nuevaColumna;

}

