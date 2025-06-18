#include "Carta.h"
#include "CartaDragon.h"
#include "CartaFenix.h"
#include "CartaLeon.h"
#include "CartaOso.h"
#include "CartaTigre.h"
#include "Ficha.h"
#include "Tablero.h"
#include "Rey.h"
#include "Peon.h"
#include "Jugador.h"
#include "IA.h"

#include <SFML/Graphics.hpp>
#include "mainMenu.h"


#include <iostream>
#include <vector>
#include <limits>

using namespace sf;

void inicializarTablero(Tablero& tablero) {
    // Ejemplo: coloca los reyes y peones; ajusta filas/columnas según tu lógica
    // Asume que Ficha tiene un constructor Ficha(char tipo, char dueno)
    tablero.colocarFicha(new Ficha ('R',false,'r'), 4, 2);   // Rey rojo
    tablero.colocarFicha(new Ficha('R',false,'a'), 0, 2);   // Rey azul
    // Coloca peones rojos
    for(int j=0; j<5; ++j)
        tablero.colocarFicha(new Ficha('P',false,'r'), 3, j);
    // Coloca peones azules
    for(int j=0; j<5; ++j)
        tablero.colocarFicha(new Ficha('P',false,'a'), 1, j);
    // ... y así con todas tus piezas y cartas
}



int main() {

    
     RenderWindow MENU(VideoMode(960, 720), "Main Menu", Style::Default);
    MainMenu mainMenu(MENU.getSize().x, MENU.getSize().y);

    // Fondo del menú (igual que antes)...
    RectangleShape background(Vector2f(960,720));
    Texture mainTexture; 
    mainTexture.loadFromFile("files/fondo.jpg");
    background.setTexture(&mainTexture);

    while (MENU.isOpen()) {
        Event event;
        while (MENU.pollEvent(event)) {
            if (event.type == Event::Closed)
                MENU.close();

            if (event.type == Event::KeyReleased) {
                if (event.key.code == Keyboard::Up)
                    mainMenu.MoveUp();
                if (event.key.code == Keyboard::Down)
                    mainMenu.MoveDown();
                if (event.key.code == Keyboard::Return) {
                    int x = mainMenu.MainMenuPressed();
                    if (x == 0) {  // **Jugar**
                        // Cerramos el menú y abrimos la ventana de juego
                        MENU.close();
                        RenderWindow gameWindow(VideoMode(700, 500), "Onitama");

                        // Preparar tablero y jugadores
                        Tablero tablero;
                        inicializarTablero(tablero);

                        Jugador jugadorAzul('a');
                        Jugador jugadorRojo('r');

                        IA ia;                       // instancia de la IA
                        char colorIA = 'a';          // la IA jugará con las fichas azules
                        bool juegoTerminado = false; // para detectar fin de partida

                        jugadorAzul.cartas[0] = new CartaLeon();
                        jugadorAzul.cartas[1] = new CartaFenix();
                        jugadorRojo.cartas[0] = new CartaTigre();
                        jugadorRojo.cartas[1] = new CartaOso();
                        Carta* cartaCentro = new CartaDragon();

                        const float boardSize = 500.f;
                        const float tileSize = boardSize / Tablero::Filas;

                                                Texture boardTexture;
                        Sprite boardSprite;
                        bool boardLoaded = boardTexture.loadFromFile("files/fondoTablero.jpg");
                        if(boardLoaded){
                            boardSprite.setTexture(boardTexture);
                            boardSprite.setScale(boardSize / boardTexture.getSize().x,
                                                 boardSize / boardTexture.getSize().y);
                        }

                        Texture tPawnRed, tPawnBlue, tKingRed, tKingBlue;
                        bool pawnRedOk  = tPawnRed.loadFromFile("files/peonRojo.png");
                        bool pawnBlueOk = tPawnBlue.loadFromFile("files/peonAzul.png");
                        bool kingRedOk  = tKingRed.loadFromFile("files/reyRojo.png");
                        bool kingBlueOk = tKingBlue.loadFromFile("files/reyAzul.png");

                        Font font;
                        font.loadFromFile("files/Minecraft.ttf");

                        bool turnoRojo = true;
                        int selectedCard = -1;
                        int selRow = -1, selCol = -1;
                        std::vector<std::pair<int,int>> validMoves;

                        auto calcularMovimientos = [&](int fila, int col, Carta* carta){
                            validMoves.clear();
                            for(int i=0;i<carta->getCantidadMovimientos();++i){
                                Movimiento m = carta->getMovimiento(i);
                                int nf = fila + m.dx;
                                int nc = col + m.dy;
                                if(tablero.movimientoValidos(fila,col,nf,nc))
                                    validMoves.push_back({nf,nc});
                            }
                        };

                        while (gameWindow.isOpen()) {
                            Event gevent;
                            while (gameWindow.pollEvent(gevent)) {
                                if (gevent.type == Event::Closed)
                                    gameWindow.close();
                                if (gevent.type == Event::KeyPressed && gevent.key.code == Keyboard::Escape)
                                    gameWindow.close();
                                if (gevent.type == Event::MouseButtonPressed && gevent.mouseButton.button == Mouse::Left) {
                                    Vector2f mp(gevent.mouseButton.x, gevent.mouseButton.y);
                                    if(mp.x < boardSize && mp.y < boardSize) {
                                        int row = mp.y / tileSize;
                                        int col = mp.x / tileSize;
                                        char colorTurno = turnoRojo ? 'r' : 'a';
                                        if(selRow == -1) {
                                            if(selectedCard != -1) {
                                                Ficha* f = tablero.getPosicionFicha(row,col);
                                                if(f && f->getDueno()==colorTurno){
                                                    selRow=row; selCol=col;
                                                    Carta* c = turnoRojo ? jugadorRojo.cartas[selectedCard] : jugadorAzul.cartas[selectedCard];
                                                    calcularMovimientos(row,col,c);
                                                }
                                            }
                                        } else {
                                            for(auto mv: validMoves){
                                                if(mv.first==row && mv.second==col){
                                                    bool fin=false;
                                                    tablero.moverFicha(selRow,selCol,row,col,fin);
                                                    Jugador& jug = turnoRojo ? jugadorRojo : jugadorAzul;
                                                    jug.usarCarta(selectedCard,cartaCentro);
                                                    turnoRojo = !turnoRojo;
                                                    selectedCard = -1;
                                                    selRow = selCol = -1;
                                                    validMoves.clear();

                                                    // Si es turno de la IA, ejecutar su movimiento
                                                    if(!juegoTerminado && ((colorIA=='r' && turnoRojo) || (colorIA=='a' && !turnoRojo))){
                                                        Jugador& iaPlayer = (colorIA=='r') ? jugadorRojo : jugadorAzul;
                                                        Jugador& humano    = (colorIA=='r') ? jugadorAzul : jugadorRojo;
                                                        MovimientoIA mejor = ia.obtenerMejorMovimiento(tablero, iaPlayer.cartas, humano.cartas, 2, colorIA);
                                                        tablero.moverFicha(mejor.filaOrigen, mejor.columnaOrigen, mejor.filaDestino, mejor.columnaDestino, juegoTerminado);
                                                        iaPlayer.usarCarta(mejor.indiceCarta, cartaCentro);
                                                        turnoRojo = !turnoRojo;
                                                    }


                                                    break;
                                                }
                                            }
                                        }
                                    } else {
                                        // Seleccion de carta
                                        FloatRect cardAreas[3] = {
                                            {520, 40, 160, 60},
                                            {520, 120, 160, 60},
                                            {520, 200, 160, 60}
                                        };
                                        for(int i=0;i<2;++i){
                                            if(cardAreas[i].contains(mp)){
                                                selectedCard = i;
                                                selRow = selCol = -1;
                                                validMoves.clear();
                                            }
                                        }
                                    }
                                }
                            }

                            // Si el turno corresponde a la IA y no se ha terminado el juego,
                            // ejecuta su movimiento automáticamente
                            if(!juegoTerminado && ((colorIA=='r' && turnoRojo) || (colorIA=='a' && !turnoRojo))){
                                Jugador& iaPlayer = (colorIA=='r') ? jugadorRojo : jugadorAzul;
                                Jugador& humano    = (colorIA=='r') ? jugadorAzul : jugadorRojo;
                                MovimientoIA mejor = ia.obtenerMejorMovimiento(tablero, iaPlayer.cartas, humano.cartas, 2, colorIA);
                                tablero.moverFicha(mejor.filaOrigen, mejor.columnaOrigen, mejor.filaDestino, mejor.columnaDestino, juegoTerminado);
                                iaPlayer.usarCarta(mejor.indiceCarta, cartaCentro);
                                turnoRojo = !turnoRojo;
                            }


                            gameWindow.clear();
                            // --- Dibujar cuadrícula ---
                             if(boardLoaded)
                                gameWindow.draw(boardSprite);

                            for (int i = 0; i < Tablero::Filas; ++i) {
                                for (int j = 0; j < Tablero::Columnas; ++j) {
                                    RectangleShape square(Vector2f(tileSize, tileSize));
                                    square.setPosition(j * tileSize, i * tileSize);

                                       if(!boardLoaded){
                                        if ((i + j) % 2 == 0)
                                            square.setFillColor(Color(200, 200, 200));
                                        else
                                            square.setFillColor(Color(100, 100, 100));
                                    }else{
                                        square.setFillColor(Color::Transparent);
                                    }

                                    if(i==selRow && j==selCol){
                                        square.setOutlineThickness(3);
                                        square.setOutlineColor(Color::Yellow);
                                    }
                                    gameWindow.draw(square);

                                    Ficha* f = tablero.getPosicionFicha(i, j);
                                    if (f) {
                                         bool drewSprite = false;
                                        Sprite sp;
                                        if(f->getTipo()=='P'){
                                            if(f->getDueno()=='r' && pawnRedOk){
                                                sp.setTexture(tPawnRed);
                                                drewSprite=true;
                                            }else if(f->getDueno()=='a' && pawnBlueOk){
                                                sp.setTexture(tPawnBlue);
                                                drewSprite=true;
                                            }
                                        }else if(f->getTipo()=='R'){
                                            if(f->getDueno()=='r' && kingRedOk){
                                                sp.setTexture(tKingRed);
                                                drewSprite=true;
                                            }else if(f->getDueno()=='a' && kingBlueOk){
                                                sp.setTexture(tKingBlue);
                                                drewSprite=true;
                                            }
                                        }
                                        if(drewSprite){
                                            sp.setPosition(j*tileSize, i*tileSize);
                                            sp.setScale(tileSize / sp.getTexture()->getSize().x,
                                                        tileSize / sp.getTexture()->getSize().y);
                                            gameWindow.draw(sp);
                                        }else{
                                            CircleShape piece(tileSize / 2.f - 8.f);
                                            piece.setPosition(j * tileSize + 8.f, i * tileSize + 8.f);
                                            if (f->getDueno() == 'r')
                                                piece.setFillColor(Color::Red);
                                            else
                                                piece.setFillColor(Color::Blue);
                                            gameWindow.draw(piece);

                                            Text label;
                                            label.setFont(font);
                                            label.setString(std::string(1, f->getTipo()));
                                            label.setCharacterSize(24);
                                            label.setFillColor(Color::White);
                                            label.setPosition(j * tileSize + tileSize/2 - 8,
                                                              i * tileSize + tileSize/2 - 12);
                                            gameWindow.draw(label);
                                        }
                                    }
                                }
                            }

                            // mostrar movimientos validos
                            for(auto mv: validMoves){
                                CircleShape hint(tileSize/2 - 20);
                                hint.setFillColor(Color(0,255,0,120));
                                hint.setPosition(mv.second*tileSize+20, mv.first*tileSize+20);
                                gameWindow.draw(hint);
                            }

                            // Dibujar cartas
                            FloatRect cardAreas[3] = {
                                {520, 40, 160, 60},
                                {520, 120, 160, 60},
                                {520, 200, 160, 60}
                            };
                            Jugador& jugadorAct = turnoRojo ? jugadorRojo : jugadorAzul;
                            Carta* cartasMostrar[3] = {jugadorAct.cartas[0], jugadorAct.cartas[1], cartaCentro};
                            for(int i=0;i<3;++i){
                                RectangleShape rect(Vector2f(cardAreas[i].width, cardAreas[i].height));
                                rect.setPosition(cardAreas[i].left, cardAreas[i].top);
                                rect.setFillColor(Color(150,150,150));
                                rect.setOutlineThickness((i==selectedCard)?3:1);
                                rect.setOutlineColor((i==selectedCard)?Color::Yellow:Color::Black);
                                gameWindow.draw(rect);

                                Text t;
                                t.setFont(font);
                                t.setString(cartasMostrar[i]->getNombre());
                                t.setCharacterSize(20);
                                t.setFillColor(Color::Black);
                                t.setPosition(cardAreas[i].left + 10, cardAreas[i].top + 20);
                                gameWindow.draw(t);
                            }

                            gameWindow.display();
                        }

                        return 0;
                    }
                    else if (x == 2) { // Salir
                        MENU.close();
                        return 0;
                    }
                    // Opciones quedan igual por ahora
                    
                }
            }
        }

        MENU.clear();
        MENU.draw(background);
        mainMenu.draw(MENU);
        MENU.display();
    }

    std::cout << "\nGracias por jugar.\n";
    return 0;
 
}






 
