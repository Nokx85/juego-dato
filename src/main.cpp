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
                        RenderWindow gameWindow(VideoMode(500, 500), "Onitama");
                        
                        // 1) Instanciamos y rellenamos el tablero
                        Tablero tablero;
                        inicializarTablero(tablero);

                        // 2) Preparamos tamaño de cada casilla
                        const float tileSize = 500.f / Tablero::Filas;

                        // Bucle principal del juego
                        while (gameWindow.isOpen()) {
                            Event gevent;
                            while (gameWindow.pollEvent(gevent)) {
                                if (gevent.type == Event::Closed)
                                    gameWindow.close();
                                if (gevent.type == Event::KeyPressed &&
                                    gevent.key.code == Keyboard::Escape)
                                    gameWindow.close();
                            }

                            gameWindow.clear();

                            // --- Dibujar cuadrícula ---
                            for (int i = 0; i < Tablero::Filas; ++i) {
                                for (int j = 0; j < Tablero::Columnas; ++j) {
                                    RectangleShape square(Vector2f(tileSize, tileSize));
                                    square.setPosition(j * tileSize, i * tileSize);
                                    // Tablero a cuadros claro/oscuro
                                    if ((i + j) % 2 == 0)
                                        square.setFillColor(Color(200, 200, 200));
                                    else
                                        square.setFillColor(Color(100, 100, 100));
                                    gameWindow.draw(square);

                                    // --- Si hay ficha, la dibujamos encima ---
                                    Ficha* f = tablero.getPosicionFicha(i, j);
                                    if (f) {
                                        // Podemos usar un círculo para representar
                                        CircleShape piece(tileSize / 2.f - 8.f);
                                        piece.setPosition(j * tileSize + 8.f,
                                                          i * tileSize + 8.f);
                                        // Color según dueño
                                        if (f->getDueno() == 'r')
                                            piece.setFillColor(Color::Red);
                                        else
                                            piece.setFillColor(Color::Blue);
                                        gameWindow.draw(piece);

                                        // Opcional: dibujar el tipo de pieza
                                        Text label;
                                        static Font font;
                                        if (font.getInfo().family.empty())
                                            font.loadFromFile("files/Minecraft.ttf");
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

                            gameWindow.display();
                        }
                    }
                    // ... Opciones y Salir quedan igual
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
