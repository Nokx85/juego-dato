#include "CartaLeon.h"

CartaLeon::CartaLeon(){
   cantidadMovimientos = 3;

    movimientos[0] = {0, 3};   // 1 paso adelante (hacia abajo)
    movimientos[1] = {0, 1};   // diagonal adelante-derecha
    movimientos[2] = {0, 2};   // 1 paso a la derecha

    }

std::string CartaLeon::getNombre() const {
    return "Leon";

}