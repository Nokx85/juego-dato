#pragma once

#include <array>
#include <iostream>
#include "Carta.h"

class Jugador{
private:
    char color; // r rojo o azul

public:
    std::array<Carta*, 2> cartas;

    Jugador(char color);

    void usarCarta(int indice, Carta*& cartaCentro);

    void mostrarCartas()const;

    char getColor()const;

};