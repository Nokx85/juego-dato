#include "Tablero.h"
#include <iostream>



Tablero::Tablero(){
    for(int i = 0; i < Filas; i++){
        for(int j = 0; j < Columnas; j++){
            casillas[i][j] = nullptr;
        }
    }
}

void Tablero::colocarFicha(Ficha* ficha, int fila, int columna) {
    ficha->setPosicionFicha(fila, columna); 
    casillas[fila][columna] = ficha;
}

bool Tablero::enRango(int fila, int columna) const{
        return fila>=0 && fila<Filas && columna>=0 && columna<Columnas;
}

bool Tablero::movimientoValidos(int filaOrigen, int colOrigen, int filaDestino, int colDestino)const{
    if ( !enRango(filaOrigen, colOrigen) || !enRango(filaDestino, colDestino))
        return false; // sobre pasa el limite del tablero 

    Ficha* fOrigen = casillas[filaOrigen][colOrigen];
    if(!fOrigen)
        return false;  // no hay ficha en el origen

    Ficha* fDestino = casillas[filaDestino][colDestino];
    if (fDestino && fDestino->getDueno() == fOrigen->getDueno())
        return false; // no se puede comer una piesa propia 

    return true;
}


void Tablero::moverFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino){
    // por si no existe una ficha en la poscion origen
    Ficha* fOrigen = casillas[filaOrigen][colOrigen];
    if (!fOrigen) return;

    // existe una ficha en esta poscicion? si es asi la borra(captura)
    if(casillas [filaDestino][colDestino] != nullptr){
        eliminarFicha(filaDestino, colDestino);
    }

    //mueve la ficha
    casillas[filaDestino][colDestino] = fOrigen;
    casillas[filaOrigen][colOrigen] = nullptr;

    //actualisa la pos en la fila 
    // Actualizar posición
    fOrigen->setPosicionFicha(filaDestino, colDestino);

    // Verificar si se movió un Rey a un dojo enemigo
    if (fOrigen->getTipo() == 'R') {
        if (fOrigen->getDueno() == 'r' && filaDestino == 0 && colDestino == 2) {
            std::cout << "¡El Rey Rojo llegó al dojo azul! Gana el jugador ROJO." << std::endl;
           std::cout << "[DEBUG] Saliendo del juego por victoria." << std::endl;

            
    }
    if (fOrigen->getDueno() == 'a' && filaDestino == 4 && colDestino == 2) {
        std::cout << "¡El Rey Azul llegó al dojo rojo! Gana el jugador AZUL." << std::endl;
        std::cout << "[DEBUG] Saliendo del juego por victoria." << std::endl;

        
    }

}

}
 // elimina la ficha si existe una ficha en la pos
void Tablero::eliminarFicha(int fila, int columna){
    Ficha* ficha = casillas[fila][columna]; 
    if(ficha != nullptr){
        if(ficha->getTipo() == 'R' ){
            if(ficha->getDueno() == 'a'){
                 std::cout << "¡El Rey Azul ha sido capturado! Gana el jugador ROJO." << std::endl;
                 std::cout << "[DEBUG] Saliendo del juego por victoria." << std::endl;

                 
            }
            if(ficha->getDueno() == 'r'){
                std::cout << "¡El Rey Azul ha sido capturado! Gana el jugador AZUL." << std::endl;
                std::cout << "[DEBUG] Saliendo del juego por victoria." << std::endl;

                
            }

            
        }
        delete casillas[fila][columna];
        casillas[fila][columna] = nullptr;
    }
}

Ficha* Tablero::getPosicionFicha(int fila, int columna) const{
    return casillas[fila][columna];
}

void Tablero::mostrarTablero() const{
    for(int fila = 0; fila < 5; fila++ ){
        for(int col = 0; col < 5; col++){
            if(casillas[fila][col] == nullptr){
                std::cout << ".  ";
            }else{
                std::cout << casillas[fila][col]->getTipo() << casillas[fila][col]->getDueno() << " ";
            }

        }
        std::cout << std::endl;
    }

}

bool Tablero::verificarReyEnDojo() const {
    // Verifica si el Rey Rojo llegó al dojo azul
    Ficha* fichaEnDojoAzul = casillas[0][2];
    if (fichaEnDojoAzul &&
        fichaEnDojoAzul->getTipo() == 'R' &&
        fichaEnDojoAzul->getDueno() == 'r') {
        std::cout << "¡El Rey Rojo llegó al dojo azul! Gana el jugador ROJO." << std::endl;
        return true;
    }

    // Verifica si el Rey Azul llegó al dojo rojo
    Ficha* fichaEnDojoRojo = casillas[4][2];
    if (fichaEnDojoRojo &&
        fichaEnDojoRojo->getTipo() == 'R' &&
        fichaEnDojoRojo->getDueno() == 'a') {
        std::cout << "¡El Rey Azul llegó al dojo rojo! Gana el jugador AZUL." << std::endl;
        return true;
    }
    return false; // ningún rey ha llegado al dojo enemigo
}





