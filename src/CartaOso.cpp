#include "CartaOso.h"

CartaOso::CartaOso(){
     cantidadMovimientos = 3;

    movimientos[0] = {-1, -1};   // 1 paso adelante (hacia arriba)
    movimientos[1] = {1, -1};  // diagonal adelante-izquierda
    movimientos[2] = {0, -2};   // 1 paso a la izquierda

    }

std::string CartaOso::getNombre() const {
    return "Oso";
}