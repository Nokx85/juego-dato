#include <iostream>

#include "../Include/Carta.h"

Carta::Carta(){
    cantidadMovimientos = 0;

}

int Carta::getCantidadMovimientos() const{
    return cantidadMovimientos;

}
    
    Movimiento Carta::getMovimiento(int index) const{
        return movimientos[index];
    }

void Carta::mostrarMovimientos()const{
    for(int i = 0; i < cantidadMovimientos; ++i) {
        std::cout << "Movimiento " << i + 1 << ": dx = "
                  << movimientos[i].dx << ", dy = "
                  << movimientos[i].dy << std::endl;
    }

}
