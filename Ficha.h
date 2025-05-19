#include <string>


class Ficha
{
private:

    char tipoFicha;
    int fila;
    int columna;
    
public:
    Ficha(char tipoFicha,int fila,int columna);
    

    void moverFicha (int nuevaFila, int nuevaColumna);


   

};
