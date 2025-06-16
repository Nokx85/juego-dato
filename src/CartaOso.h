#pragma once
#include "Carta.h"

class CartaOso : public Carta{

public:
    CartaOso();

    std::string getNombre() const override { return "Oso"; }

};