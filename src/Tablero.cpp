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

void Tablero::moverFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino){
    // si la poscicion no tiene ficha no realisamos ninguna accion
    if (casillas[filaOrigen][colOrigen] == nullptr) return;

    // existe una ficha en esta poscicion? si es asi la borra(captura)
    if(casillas [filaDestino][colDestino] != nullptr){
        eliminarFicha(filaDestino, colDestino);
    }

    //mueve la ficha
    casillas[filaDestino][colDestino] = casillas[filaOrigen][colOrigen];
    casillas[filaOrigen][colOrigen] = nullptr;

    //actualisa la pos en la fila 
    casillas[filaDestino][colDestino]->moverFicha(filaDestino, colDestino);

}
 // elimina la ficha si existe una ficha en la pos
void Tablero::eliminarFicha(int fila, int columna){
    if(casillas [fila][columna] != nullptr){
        delete casillas[fila][columna];
        casillas[fila][columna] = nullptr;
    }
}

Ficha* Tablero::getFicha(int fila, int columna) const{
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

