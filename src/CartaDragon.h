#pragma once
#include "Carta.h"

class CartaDragon : public Carta{

public:
    CartaDragon();
    std::string getNombre() const override;

};