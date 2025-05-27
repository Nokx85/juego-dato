#pragma once

#include <iostream>
#include "Carta.h"

#include <queue>

class Jugador{
private:
    char color; // r rojo o azul
    std::queue<Carta*> cartas;

public:
    

    Jugador(char color);

    void agregarCarta(Carta* carta);

    void usarCarta(Carta*& cartaCentro);

    void mostrarCartas()const;

    char getColor()const;


    Carta* getCartaFrente() const; 

};