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
                        bool pawnRedOk  = tPawnRed.loadFromFile("files/pawn_red.png");
                        bool pawnBlueOk = tPawnBlue.loadFromFile("files/pawn_blue.png");
                        bool kingRedOk  = tKingRed.loadFromFile("files/reyRojo.png");
                        bool kingBlueOk = tKingBlue.loadFromFile("files/king_blue.png");

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







    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    Tablero tablero;

    // Crear jugadores y cartas
    Jugador jugadorAzul('a');
    Jugador jugadorRojo('r');

    jugadorAzul.cartas[0] = new CartaLeon();
    jugadorAzul.cartas[1] = new CartaFenix();

    jugadorRojo.cartas[0] = new CartaTigre();
    jugadorRojo.cartas[1] = new CartaOso();

    Carta* cartaCentro = new CartaDragon();

    // Colocar fichas en el tablero (solo 2 peones por equipo)
    tablero.colocarFicha(new Rey('a'), 0, 2);
    tablero.colocarFicha(new Rey('r'), 4, 2);

    tablero.colocarFicha(new Peon('a'), 0, 1);
    tablero.colocarFicha(new Peon('a'), 0, 3);
    tablero.colocarFicha(new Peon('r'), 4, 1);
    tablero.colocarFicha(new Peon('r'), 4, 3);

    bool juegoTerminado = false;
    bool turnoRojo = true;

    while (!juegoTerminado) {
        tablero.mostrarTablero();
        std::cout << "\nCarta en el centro (bonus):\n";
        cartaCentro->mostrarMovimientos();

        Jugador* jugador;
        if (turnoRojo) {
            std::cout << "\nTurno del jugador ROJO\n";
            jugador = &jugadorRojo;
        } else {
            std::cout << "\nTurno del jugador AZUL\n";
            jugador = &jugadorAzul;
        }

        jugador->mostrarCartas();

        std::cout << "Elige una carta (0 o 1): ";
        int indiceCarta;
        while (!(std::cin >> indiceCarta) || (indiceCarta != 0 && indiceCarta != 1)) {
            std::cout << "Esa carta no es valida. Intenta otra vez: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        Carta* cartaUsada = jugador->cartas[indiceCarta];

        std::cout << "Movimientos posibles de la carta elegida:\n";
        cartaUsada->mostrarMovimientos();

        std::vector<std::pair<int, int>> posiciones;
        std::cout << "Fichas disponibles para mover:\n";
        tablero.mostrarFichasJugador(jugador->getColor(), posiciones);


        int eleccionFicha = -1;
        std::cout << "Elige el numero de la ficha que quieres mover: ";
        while (!(std::cin >> eleccionFicha) ||
               eleccionFicha < 0 || eleccionFicha >= posiciones.size()) {
            std::cout << "Numero invalido. Intenta otra vez: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int filaOrigen = posiciones[eleccionFicha].first;
        int colOrigen = posiciones[eleccionFicha].second;

        Ficha* ficha = tablero.getPosicionFicha(filaOrigen, colOrigen);
        if (ficha == nullptr || ficha->getDueno() != jugador->getColor()) {
            std::cout << "No puedes mover esa ficha. Intenta de nuevo.\n";
            continue;
        }

        tablero.mostrarMovimientosPosibles(filaOrigen, colOrigen, cartaUsada, jugador->getColor());

        // Mostrar y guardar los movimientos válidos numerados
        std::vector<std::pair<int, int>> movimientosValidos;
        std::cout << "\nMovimientos posibles desde esa ficha:\n";
        for (int i = 0; i < cartaUsada->getCantidadMovimientos(); ++i) {
            Movimiento mov = cartaUsada->getMovimiento(i);
            int filaDestino = filaOrigen + mov.dx;
            int colDestino = colOrigen + mov.dy;

            if (tablero.movimientoValidos(filaOrigen, colOrigen, filaDestino, colDestino)) {
                std::cout << movimientosValidos.size() << ": a (" << filaDestino << "," << colDestino << ")\n";
                movimientosValidos.push_back({filaDestino, colDestino});
            }
        }

        if (movimientosValidos.empty()) {
            std::cout << "\nNo hay movimientos válidos para esta ficha. Elige otra.\n";
            continue;
        }

        std::cout << "Elige el numero del movimiento que quieres usar: ";
        int movIndex;
        while (!(std::cin >> movIndex) || movIndex < 0 || movIndex >= movimientosValidos.size()) {
            std::cout << "Movimiento invalido. Intenta otra vez: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int filaDestino = movimientosValidos[movIndex].first;
        int colDestino = movimientosValidos[movIndex].second;

        tablero.moverFicha(filaOrigen, colOrigen, filaDestino, colDestino, juegoTerminado);

        if (tablero.verificarReyEnDojo()) {
            std::cout << "\nEl juego ha terminado porque un rey llego al dojo enemigo.\n";
            juegoTerminado = true;
            continue;
        }

        turnoRojo = !turnoRojo;
        jugador->usarCarta(indiceCarta, cartaCentro);
    }

    std::cout << "\nGracias por jugar.\n";
    return 0;
}
