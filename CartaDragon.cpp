#include "CartaDragon.h"

CartaDragon::CartaDragon(){
    cantidadMovimientos = 4;

    movimientos[0] = {1, 1};    // diagonal abajo-derecha
    movimientos[1] = {1, -1};   // diagonal abajo-izquierda
    movimientos[2] = {-1, 1};   // diagonal arriba-derecha
    movimientos[3] = {-1, -1};  // diagonal arriba-izquierda


}