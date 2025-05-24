#include "Ficha.h"

class Rey : public Ficha{

public:
    Rey(int fila, int columna);

    void moverFicha(int nuevaFila, int nuevaColumna);

};