#include "../Include/Tablero.h"
#include "../Include/Rey.h"
#include "../Include/Peon.h"
#include "../Include/Carta.h"
#include "../Include/CartaTigre.h"

int main(){

    Tablero tablero;   //inicialisar tablero

    tablero.colocarFicha(new Rey(0,0,'a'),0,2);  
    // creamos la ficha roja en la pos((*rey), fila, columna)


    tablero.colocarFicha(new Rey(4,0,'r'),4,2);
    // creamos la ficha aul en la pos((*rey), fila, columna)


    for(int j = 0; j < Tablero::Columnas; ++j) {

    
        if(j == 2) continue ; // para no eliminar al rey en la columan 2 

            tablero.colocarFicha(new Peon(0,j,'a'),1,j);
            // creamos la ficha peon aul en toda la fila 0 excepto la columna 2

            tablero.colocarFicha(new Peon(4,j,'r'),4,j);
            // creamos la ficha peon roja en toda la fila 4 excepto la columna 2
 
    } 

     tablero.mostrarTablero();

    return 0;
}