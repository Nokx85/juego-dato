#include "Jugador.h"


Jugador::Jugador(char color) : color(color) {}

void Jugador::agregarCarta(Carta* carta){
    cartas.push(carta);
}

void Jugador::usarCarta(Carta*& cartaCentro) {
    if (cartas.empty()) return;

    Carta* temp = cartas.front();
    cartas.pop();
    cartas.push(cartaCentro);
    cartaCentro = temp;
}


void Jugador::mostrarCartas() const {
    std::cout << "Cartas del jugador " << color << ":\n";

    std::queue<Carta*> temp = cartas;
    int i = 1;
    while (!temp.empty()) {
        std::cout << "Carta " << i++ << ":\n";
        temp.front()->mostrarMovimientos();
        temp.pop();
    }
}

char Jugador::getColor() const {
    return color;
}

Carta* Jugador::getCartaFrente() const {
    if (!cartas.empty()) {
        return cartas.front();
    }
    return nullptr;
}
