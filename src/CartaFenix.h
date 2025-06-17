#pragma once
#include "Carta.h"

class CartaFenix : public Carta{

public:
    CartaFenix();
     std::string getNombre() const override;
};