#include "../Include/CartaLeon.h"

CartaLeon::CartaLeon(){
   cantidadMovimientos = 3;

    movimientos[0] = {1, 0};   // 1 paso adelante (hacia abajo)
    movimientos[1] = {1, 1};   // diagonal adelante-derecha
    movimientos[2] = {0, 1};   // 1 paso a la derecha

}