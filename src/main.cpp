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

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "mainMenu.h"


#include <iostream>
#include <vector>
#include <limits>

using namespace sf;

bool musicEnabled = true;
Music music;
void inicializarTablero(Tablero& tablero) {
    // Ejemplo: coloca los reyes y peones; ajusta filas/columnas según tu lógica
    // Asume que Ficha tiene un constructor Ficha(char tipo, char dueno)
    tablero.colocarFicha(new Ficha ('R',false,'r'), 4, 2);   // Rey rojo
    tablero.colocarFicha(new Ficha('R',false,'a'), 0, 2);    // Rey azul

    // Solo dos peones al lado de cada rey
    tablero.colocarFicha(new Ficha('P',false,'r'), 4, 1);
    tablero.colocarFicha(new Ficha('P',false,'r'), 4, 3);
    tablero.colocarFicha(new Ficha('P',false,'a'), 0, 1);
    tablero.colocarFicha(new Ficha('P',false,'a'), 0, 3);
}



int main() {

    
    RenderWindow MENU(VideoMode(960, 720), "Main Menu", Style::Default);
    MainMenu mainMenu(MENU.getSize().x, MENU.getSize().y);

    //musiquita
     if(music.openFromFile("files/cancion.wav")){
        music.setLoop(true);
        if(musicEnabled)
            music.play();
    }else{
        std::cout << "No se pudo cargar la musica" << std::endl;
    }



    // Fondo del menú 
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
                    //aqui pa cambiar el texto del fondo del tablero al presionar jugar donde tan las cartas 
                        Texture gameBgTex;
                        Sprite gameBg;
                        if(gameBgTex.loadFromFile("files/fondoAtras.jpg")){
                            gameBg.setTexture(gameBgTex);
                            gameBg.setScale(
                                gameWindow.getSize().x / (float)gameBgTex.getSize().x,
                                gameWindow.getSize().y / (float)gameBgTex.getSize().y);
                        }


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

                        const float margin = 20.f;
                        float boardSize = std::min(gameWindow.getSize().y - 2*margin,gameWindow.getSize().x * 0.6f);
                        const float tileSize = boardSize / Tablero::Filas;
                        float cardWidth  = gameWindow.getSize().x - boardSize - 3*margin;
                        float cardHeight = (boardSize - 4*margin) / 3.f;

                                                Texture boardTexture;
                        Sprite boardSprite;
                        bool boardLoaded = boardTexture.loadFromFile("files/fondoTablero.jpg");
                        if(boardLoaded){
                            boardSprite.setTexture(boardTexture);
                            boardSprite.setScale(boardSize / boardTexture.getSize().x,
                                                 boardSize / boardTexture.getSize().y);
                            boardSprite.setPosition(margin, margin);
                                                }

                        Texture tPawnRed, tPawnBlue, tKingRed, tKingBlue;
                        bool pawnRedOk  = tPawnRed.loadFromFile("files/peonRojo.png");
                        bool pawnBlueOk = tPawnBlue.loadFromFile("files/peonAzul.png");
                        bool kingRedOk  = tKingRed.loadFromFile("files/reyRojo.png");
                        bool kingBlueOk = tKingBlue.loadFromFile("files/reyAzul.png");

    // Texturas de cartas (opcionalmente puede que no existan)
                        Texture cardLeon, cardFenix, cardTigre, cardOso, cardDragon;
                        bool cardLeonOk   = cardLeon.loadFromFile("files/Leon.png");
                        bool cardFenixOk  = cardFenix.loadFromFile("files/Fenix.png");
                        bool cardTigreOk  = cardTigre.loadFromFile("files/Tigre.png");
                        bool cardOsoOk    = cardOso.loadFromFile("files/Oso.png");
                        bool cardDragonOk = cardDragon.loadFromFile("files/Dragon.png");

                    

//pa la letra q sale en el menu (nosostros pusimos la de minecraft )
                        Font font;
                        font.loadFromFile("files/Minecraft.ttf");

                        bool turnoRojo = true;
                        int selectedCard = -1;
                        int selRow = -1, selCol = -1;
                        std::vector<std::pair<int,int>> validMoves;
                       
                        std::string mensajeGanador;

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
                                    if(mp.x >= margin && mp.x < margin + boardSize &&
                                       mp.y >= margin && mp.y < margin + boardSize) {
                                        int row = (mp.y - margin) / tileSize;
                                        int col = (mp.x - margin) / tileSize;
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
                                                    char ganador = tablero.moverFicha(selRow,selCol,row,col,fin);
                                                    Jugador& jug = turnoRojo ? jugadorRojo : jugadorAzul;
                                                    jug.usarCarta(selectedCard,cartaCentro);
                                                    if(fin && ganador!='0'){
                                                        juegoTerminado = true;
                                                        mensajeGanador = (ganador=='r')?"Gana el jugador ROJO":"Gana el jugador AZUL";
                                                    }else{
                                                        turnoRojo = !turnoRojo;
                                                    }
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
                                           FloatRect cardAreas[3];
                                        for(int i=0;i<3;++i){
                                            cardAreas[i] = {margin + boardSize + margin,
                                                           margin + i*(cardHeight + margin),
                                                           cardWidth,
                                                           cardHeight};
                                        }
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
                              if(gameBgTex.getSize().x>0)
                                gameWindow.draw(gameBg);
                            // --- Dibujar cuadrícula ---
                             if(boardLoaded)
                                gameWindow.draw(boardSprite);

                            for (int i = 0; i < Tablero::Filas; ++i) {
                                for (int j = 0; j < Tablero::Columnas; ++j) {
                                    RectangleShape square(Vector2f(tileSize, tileSize));
                                    square.setPosition(margin + j * tileSize, margin + i * tileSize);

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
                                            sp.setPosition(margin + j*tileSize, margin + i*tileSize);
                                            sp.setScale(tileSize / sp.getTexture()->getSize().x,
                                                        tileSize / sp.getTexture()->getSize().y);
                                            gameWindow.draw(sp);
                                        }else{
                                            CircleShape piece(tileSize / 2.f - 8.f);
                                              piece.setPosition(margin + j * tileSize + 8.f, margin + i * tileSize + 8.f);
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
                                                                        label.setPosition(margin + j * tileSize + tileSize/2 - 8,
                                                              margin + i * tileSize + tileSize/2 - 12);
                                            gameWindow.draw(label);
                                        }
                                    }
                                }
                            }

                            // mostrar movimientos validos
                            for(auto mv: validMoves){
                                CircleShape hint(tileSize/2 - 20);
                                hint.setFillColor(Color(0,255,0,120));
                                hint.setPosition(margin + mv.second*tileSize+20,
                                                 margin + mv.first*tileSize+20);
                                gameWindow.draw(hint);
                            }

                            // Dibujar cartas
                            FloatRect cardAreas[3];
                            for(int i=0;i<3;++i){
                                cardAreas[i] = {margin + boardSize + margin,
                                                margin + i*(cardHeight + margin),
                                                cardWidth,
                                                cardHeight};
                            }
                            Jugador& jugadorAct = turnoRojo ? jugadorRojo : jugadorAzul;
                            Carta* cartasMostrar[3] = {jugadorAct.cartas[0], jugadorAct.cartas[1], cartaCentro};
                            for(int i=0;i<3;++i){
                                RectangleShape rect(Vector2f(cardAreas[i].width, cardAreas[i].height));
                                rect.setPosition(cardAreas[i].left, cardAreas[i].top);
                                rect.setFillColor(Color(150,150,150));
                                rect.setOutlineThickness((i==selectedCard)?3:1);
                                rect.setOutlineColor((i==selectedCard)?Color::Yellow:Color::Black);
                                gameWindow.draw(rect);

                                Texture* cardTex = nullptr;
                                std::string nombre = cartasMostrar[i]->getNombre();
                                if(nombre == "Leon" && cardLeonOk)       cardTex = &cardLeon;
                                else if(nombre == "Fenix" && cardFenixOk) cardTex = &cardFenix;
                                else if(nombre == "Tigre" && cardTigreOk) cardTex = &cardTigre;
                                else if(nombre == "Oso" && cardOsoOk)     cardTex = &cardOso;
                                else if(nombre == "Dragon" && cardDragonOk) cardTex = &cardDragon;

                                if(cardTex){
                                    Sprite sp(*cardTex);
                                    sp.setPosition(cardAreas[i].left, cardAreas[i].top);
                                    sp.setScale(cardAreas[i].width / cardTex->getSize().x,
                                                cardAreas[i].height / cardTex->getSize().y);
                                    gameWindow.draw(sp);
                                }else{
                                    Text t;
                                    t.setFont(font);
                                    t.setString(nombre);
                                    t.setCharacterSize(20);
                                    t.setFillColor(Color::Black);
                                    t.setPosition(cardAreas[i].left + 10, cardAreas[i].top + 20);
                                    gameWindow.draw(t);
                                }
                            } Text turnoTxt;
                            turnoTxt.setFont(font);
                            turnoTxt.setString(turnoRojo ? "Turno: ROJO" : "Turno: AZUL");
                            turnoTxt.setCharacterSize(24);
                            turnoTxt.setFillColor(turnoRojo ? Color::Red : Color::Blue);
                            turnoTxt.setPosition(margin, margin/2);
                            gameWindow.draw(turnoTxt);

                            if(juegoTerminado){
                                RectangleShape overlay(Vector2f(gameWindow.getSize()));
                                overlay.setFillColor(Color(0,0,0,150));
                                gameWindow.draw(overlay);

                                Text finTxt;
                                finTxt.setFont(font);
                                finTxt.setString(mensajeGanador);
                                finTxt.setCharacterSize(32);
                                finTxt.setFillColor(Color::White);
                                FloatRect bounds = finTxt.getLocalBounds();
                                finTxt.setPosition(gameWindow.getSize().x/2 - bounds.width/2,
                                                   gameWindow.getSize().y/2 - bounds.height/2);
                                gameWindow.draw(finTxt);}

                            gameWindow.display();
                        }

                        return 0;
                    }
                              else if (x == 1) { // Opciones
                        RenderWindow optWindow(VideoMode(400,200), "Opciones");
                        Font fontOpt;
                        fontOpt.loadFromFile("files/Minecraft.ttf");
                        Text msg;
                        msg.setFont(fontOpt);
                        msg.setCharacterSize(24);

                        while(optWindow.isOpen()){
                            Event ev;
                            while(optWindow.pollEvent(ev)){
                                if(ev.type==Event::Closed)
                                    optWindow.close();
                                if(ev.type==Event::KeyPressed){
                                    if(ev.key.code==Keyboard::Escape)
                                        optWindow.close();
                                    if(ev.key.code==Keyboard::Return){
                                        musicEnabled = !musicEnabled;
                                        if(musicEnabled)
                                            music.play();
                                        else
                                            music.stop();
                                    }
                                }
                            }
                            optWindow.clear();
                            msg.setString(string("Musica: ") + (musicEnabled?"ON":"OFF") + "\nEnter para cambiar\nEsc para volver");
                            msg.setPosition(20,80);
                            optWindow.draw(msg);
                            optWindow.display();
                        }
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






    
