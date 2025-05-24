#include "../Include/Tablero.h"

Tablero::Tablero(){
    for(int i = 0; i < Filas; i++){
        for(int j = 0; j < Columnas; j++){
            casillas[i][j] = nullptr;
        }
    }
}

