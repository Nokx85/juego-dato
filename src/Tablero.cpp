#include "../Include/Tablero.h"
#include <iostream>



Tablero::Tablero(){
    for(int i = 0; i < Filas; i++){
        for(int j = 0; j < Columnas; j++){
            casillas[i][j] = nullptr;
        }
    }
}

void Tablero::colocarFicha(Ficha* ficha, int fila, int columna){
    casillas[fila][columna] = ficha;
}

bool Tablero::enRango(int fila, int columna) const{
        return fila>=0 && fila<Filas && columna>=0 && columna<Columnas;
}

bool Tablero::movimientoValidos(int filaOrigen, int colOrigen, int filaDestino, int colDestino){
    if ( !enRango(filaOrigen, colOrigen) || !enRango(filaDestino, colDestino))
        return false; // sobre pasa el limite del tablero 

    Ficha* fOrigen = casillas[filaOrigen][colOrigen];
    if(!fOrigen)
        return false;  // no hay ficha en el origen

    Ficha* fDestino = casillas[filaDestino][colDestino];
    if (fDestino && fDestino->getDueno() == fOrigen->getDueno())
        return false; // no se puede comer una piesa propia 

    return true;
}


void Tablero::moverFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino){
    // por si no existe una ficha en la poscion origen
    Ficha* fOrigen = casillas[filaOrigen][colOrigen];
    if (!fOrigen) return;

    // existe una ficha en esta poscicion? si es asi la borra(captura)
    if(casillas [filaDestino][colDestino] != nullptr){
        eliminarFicha(filaDestino, colDestino);
    }

    //mueve la ficha
    casillas[filaDestino][colDestino] = fOrigen;
    casillas[filaOrigen][colOrigen] = nullptr;

    //actualisa la pos en la fila 
    casillas[filaDestino][colDestino]->setPosicionFicha(filaDestino, colDestino);

}
 // elimina la ficha si existe una ficha en la pos
void Tablero::eliminarFicha(int fila, int columna){
    if(casillas [fila][columna] != nullptr){
        delete casillas[fila][columna];
        casillas[fila][columna] = nullptr;
    }
}

Ficha* Tablero::getPosicionFicha(int fila, int columna) const{
    return casillas[fila][columna];
}

void Tablero::mostrarTablero() const{
    for(int fila = 0; fila < 5; fila++ ){
        for(int col = 0; col < 5; col++){
            if(casillas[fila][col] == nullptr){
                std::cout << ".  ";
            }else{
                std::cout << casillas[fila][col]->getNombre() << " ";
            }
        
        }
        std::cout << std::endl;
    }

}
