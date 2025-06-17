#include "CartaFenix.h"

CartaFenix::CartaFenix(){
    cantidadMovimientos = 2;

    movimientos[0] = {-1, 1};  // mover 1 casilla hacia arriba
    movimientos[1] = {1, 1};   // mover 1 casilla hacia abajo

    }

std::string CartaFenix::getNombre() const {
    return "Fenix";
}