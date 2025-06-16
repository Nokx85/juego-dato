#pragma once

#include "Carta.h"


class CartaTigre : public Carta{

public:
    CartaTigre();

    std::string getNombre() const override { return "Tigre"; }

};
