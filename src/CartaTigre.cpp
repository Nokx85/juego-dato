#include "../Include/CartaTigre.h"

CartaTigre::CartaTigre(){
    cantidadMovimientos = 2;

  // Movimiento hacia arriba (fila -1), sin cambiar columna
    movimientos[0] = {-1, 0};

    // Movimiento hacia abajo (fila +1), sin cambiar columna
    movimientos[1] = {1, 0};
    
}