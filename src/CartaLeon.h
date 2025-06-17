#pragma once
#include "Carta.h"

class CartaLeon : public Carta{

public:
    CartaLeon();
        std::string getNombre() const override;
};