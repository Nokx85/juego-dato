#include <SFML/Graphics.hpp>
#include "mainMenu.h"
#
// Incluimos la lógica del juego
#include "../src/Tablero.h"
#include "../src/Ficha.h"   
#include "../src/Carta.h"

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

    return 0;
}
