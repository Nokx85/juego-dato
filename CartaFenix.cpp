#include "CartaFenix.h"

CartaFenix::CartaFenix(){
    cantidadMovimientos = 2;

    movimientos[0] = {-1, 0};  // mover 1 casilla hacia arriba
    movimientos[1] = {1, 0};   // mover 1 casilla hacia abajo
}