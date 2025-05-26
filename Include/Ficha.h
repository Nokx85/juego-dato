#include <string>


class Ficha
{
private:

    char tipoFicha;
    int fila;
    int columna;
    bool capturaDojo;
    char dueno;
    
public:
    Ficha(char tipoFicha,int fila,int columna, bool capturaDojo, char dueno);


    void setPosicionFicha (int nuevaFila, int nuevaColumna);

    char   getTipo()     const;

    int    getFila()     const;

    int    getColumna()  const;

    bool   getCaptura()  const;

    char   getDueno()    const;



    std::string getNombre() const;

};