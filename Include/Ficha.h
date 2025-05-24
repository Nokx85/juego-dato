#include <string>


class Ficha
{
private:

    char tipoFicha;
    int fila;
    int columna;
    bool capturaDojo;
    
public:
    Ficha(char tipoFicha,int fila,int columna, bool capturaDojo);

    

    

    void moverFicha (int nuevaFila, int nuevaColumna);


   

};
