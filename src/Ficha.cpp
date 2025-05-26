
#include "../Include/Ficha.h"


Ficha::Ficha(char tipoFicha,int fila,int columna, bool capturaDojo, char dueno){
    this->tipoFicha = tipoFicha;
    this->fila = fila;
    this->columna = columna;
    this->capturaDojo = capturaDojo;
    this->dueno = dueno;

}

void Ficha::setPosicionFicha (int nuevaFila, int nuevaColumna){
    fila = nuevaFila;
    columna = nuevaColumna;

}

char  Ficha::getTipo()    const { return tipoFicha; }

int   Ficha::getFila()    const { return fila; }

int   Ficha::getColumna() const { return columna; }

bool  Ficha::getCaptura() const { return capturaDojo; }

char  Ficha::getDueno()   const { return dueno; }

std::string Ficha::getNombre() const {
    return std::string(1, tipoFicha);  // ejemplo: 'R' → "R"
}