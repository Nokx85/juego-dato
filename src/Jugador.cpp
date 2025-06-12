#include "Jugador.h"


Jugador::Jugador(char color): color(color){}

void Jugador::usarCarta(int indice, Carta*& cartaCentro) {
    if (indice < 0 || indice >= 2) return; // fuera de rango
    Carta* temp = cartas[indice];
    cartas[indice] = cartaCentro;
    cartaCentro = temp;
}

void Jugador::mostrarCartas() const {
    std::cout << "Cartas del jugador " << color << ":\n";
    for (int i = 0; i < 2; ++i) {
        std::cout << "Carta " << i + 1 << ":\n";
        cartas[i]->mostrarMovimientos();
        std::cout << std::endl;
    }
}

char Jugador::getColor() const {
    return color;
}