#include "CartaTigre.h"

CartaTigre::CartaTigre(){
    cantidadMovimientos = 2;

  // Movimiento hacia arriba (fila -1), sin cambiar columna
    movimientos[0] = {0, -2};

    // Movimiento hacia abajo (fila +1), sin cambiar columna
    movimientos[1] = {0, -1};
    
}