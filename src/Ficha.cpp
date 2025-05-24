#include "../Include/Ficha.h"


Ficha::Ficha(char tipoFicha,int fila,int columna, bool capturaDojo){
    this->tipoFicha = tipoFicha;
    this->fila = fila;
    this->columna = columna;
    this->capturaDojo = capturaDojo;
}

void Ficha::moverFicha (int nuevaFila, int nuevaColumna){
    fila = nuevaFila;
    columna = nuevaColumna;

}

std::string Ficha::getNombre() const {
    return std::string(1, tipoFicha);  // ejemplo: 'R' → "R"
}
