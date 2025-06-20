#include "Tablero.h"
#include <iostream>
#include <vector>


Tablero::Tablero(){
    for(int i = 0; i < Filas; i++){
        for(int j = 0; j < Columnas; j++){
            casillas[i][j] = nullptr;
        }
    }
}


Tablero::Tablero(const Tablero& other){
    for(int i=0;i<Filas;++i){
        for(int j=0;j<Columnas;++j){
            if(other.casillas[i][j])
                casillas[i][j] = new Ficha(*other.casillas[i][j]);
            else
                casillas[i][j] = nullptr;
        }
    }
}

Tablero& Tablero::operator=(const Tablero& other){
    if(this!=&other){
        for(int i=0;i<Filas;++i){
            for(int j=0;j<Columnas;++j){
                delete casillas[i][j];
                if(other.casillas[i][j])
                    casillas[i][j] = new Ficha(*other.casillas[i][j]);
                else
                    casillas[i][j] = nullptr;
            }
        }
    }
    return *this;
}

Tablero::~Tablero(){
    for(int i=0;i<Filas;++i){
        for(int j=0;j<Columnas;++j){
            delete casillas[i][j];
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


char Tablero::moverFicha(int filaOrigen, int colOrigen, int filaDestino, int colDestino, bool &juegoTerminado){
    // por si no existe una ficha en la poscion origen
    Ficha* fOrigen = casillas[filaOrigen][colOrigen];
     if (!fOrigen) return '0';

    // existe una ficha en esta poscicion? si es asi la borra(captura)
    char ganador='0';
    if(casillas [filaDestino][colDestino] != nullptr){
       ganador = eliminarFicha(filaDestino, colDestino,juegoTerminado);
    }

    //mueve la ficha
    casillas[filaDestino][colDestino] = fOrigen;
    casillas[filaOrigen][colOrigen] = nullptr;

    //actualisa la pos en la fila y columna
    fOrigen->setPosicionFicha(filaDestino, colDestino);

    // Verificar si se movió un Rey a un dojo enemigo
    if (verificarReyEnDojo()) {
        // Si la función retorna true, ya se ha producido una victoria
        ganador = fOrigen->getDueno();
        juegoTerminado = true;
    }
 return ganador;
}
 // elimina la ficha si existe una ficha en la pos
char Tablero::eliminarFicha(int fila, int columna, bool &juegoTerminado){
    Ficha* ficha = casillas[fila][columna];
    if(ficha != nullptr){
          char ganador='0';
        if(ficha->getTipo() == 'R' ){
            if(ficha->getDueno() == 'a'){
                 
                 juegoTerminado = true;
                 ganador='r';
                   
            }
            if(ficha->getDueno() == 'r'){
                
                juegoTerminado = true;
                ganador='a';
                
            }

            
        }

        delete casillas[fila][columna];
        casillas[fila][columna] = nullptr;
        return ganador;
    }
    return '0';
}

Ficha* Tablero::getPosicionFicha(int fila, int columna) const{
    return casillas[fila][columna];
}

void Tablero::mostrarFichasJugador(char jugadorColor,std::vector<std::pair<int,int>>& posiciones) const {
    posiciones.clear();
    int index = 0;
    for (int i = 0; i < Filas; ++i) {
        for (int j = 0; j < Columnas; ++j) {
            Ficha* f = casillas[i][j];
            if (f != nullptr && f->getDueno() == jugadorColor) {
                std::cout << index << ": " << f->getTipo() << " en (" << i << "," << j << ")\n";
                posiciones.push_back({i, j});
                ++index;
            }
        }
    }
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
       
        return true;
    }

    // Verifica si el Rey Azul llegó al dojo rojo
    Ficha* fichaEnDojoRojo = casillas[4][2];
    if (fichaEnDojoRojo &&
        fichaEnDojoRojo->getTipo() == 'R' &&
        fichaEnDojoRojo->getDueno() == 'a') {
       
        return true;
    }
    return false; // ningún rey ha llegado al dojo enemigo
}


void Tablero::mostrarMovimientosPosibles(int fila, int columna, const Carta* carta, char jugadorColor) const {
    bool marcados[Filas][Columnas] = {false};



    // Marcar los movimientos válidos desde la posición dada
    Ficha* fichaOrigen = getPosicionFicha(fila, columna);
    if (!fichaOrigen || fichaOrigen->getDueno() != jugadorColor) {
        std::cout << "[DEBUG] Posicion de origen invalida para el jugador " << jugadorColor << "\n";
        return;
    }


    for (int i = 0; i < carta->getCantidadMovimientos(); ++i) {
        Movimiento mov = carta->getMovimiento(i);
        int nuevaFila = fila + mov.dx;
        int nuevaColumna = columna + mov.dy;
        

        if (movimientoValidos(fila, columna, nuevaFila, nuevaColumna)) {
            marcados[nuevaFila][nuevaColumna] = true;
        }
    }

    // Mostrar la matriz con los posibles movimientos marcados
    std::cout << "\nMovimientos posibles marcados con 'x':\n";
    for (int i = 0; i < Filas; ++i) {
        for (int j = 0; j < Columnas; ++j) {
            if (marcados[i][j]) {
                std::cout << "x  ";
            } else if (casillas[i][j] == nullptr) {
                std::cout << ".  ";
            } else {
                std::cout << casillas[i][j]->getTipo() << casillas[i][j]->getDueno() << " ";
            }
        }
        std::cout << '\n';
    }
}



