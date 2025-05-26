

struct Movimiento{  //para crear mov como otra clase para tener mas mov posilble en 1 carta 
    int dx;
    int dy;         // le damos un dato como (x,y)
};

class Carta{
protected:  // protected porque solo vamos a crear la carta no cambiar atributos
    Movimiento movimientos[4];
    int cantidadMovimientos;

public:
    Carta();

    int getCantidadMovimientos()const; // movimientos posibles de la carta max 4 por carta 
    Movimiento getMovimiento(int index)const; // cual movimiento de los posibles se eligio
                                              // devuelve un dato tipo movimiento osea (int x,int y) sirve al momento de querer mover la ficha 
    void mostrarMovimientos()const; // un for que imprime los posibles movimientos de la carta 


};