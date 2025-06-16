#include <SFML/Graphics.hpp>
#include "mainMenu.h"
#include "BoardView.h"
#include "../src/CartaLeon.h"
#include "../src/CartaFenix.h"
#include "../src/CartaTigre.h"
#include "../src/CartaOso.h"
#include "../src/CartaDragon.h"
#include "../src/Rey.h"
#include "../src/Peon.h"
#include "../src/Tablero.h"
#include "../src/Jugador.h"
using namespace sf;

int main() {
    // Creamos la ventana principal
    RenderWindow MENU(VideoMode(960, 720), "Main Menu", Style::Default);
    MainMenu mainMenu(MENU.getSize().x, MENU.getSize().y);

    //tamaño de el menu 

    RectangleShape background ; 
    background.setSize(Vector2f(960,720));
    Texture Maintexture ; 
    Maintexture.loadFromFile("files/fondo.jpg");
    background.setTexture(&Maintexture);

    //foto del juego 
    RectangleShape Pbackground;
    Pbackground.setSize(Vector2f(960,720));
    Texture back_texture ; 
    back_texture.loadFromFile("files/fondo.jpg");
    Pbackground.setTexture(&back_texture);




    while (MENU.isOpen()) {
        Event event; 
        while (MENU.pollEvent(event)) {  // Cambié `polEvent` por `pollEvent`
            if (event.type == Event::Closed) {
                MENU.close();  // Cerramos la ventana si se recibe el evento de cierre
            }

            if (event.type == Event::KeyReleased) {
                if (event.key.code == Keyboard::Up) {
                    mainMenu.MoveUp();  // Llamamos al método MoveUp en el objeto mainMenu
                }
                if (event.key.code == Keyboard::Down) {
                    mainMenu.MoveDown();  // Llamamos al método MoveDown en el objeto mainMenu
                }
                
                if (event.key.code == Keyboard::Return) {
                    RenderWindow Play(VideoMode(960, 720), "Tablero", Style::Default);
                    RenderWindow OPTIONS(VideoMode(960, 720), "OPTIONS");
                    
                    int x = mainMenu.MainMenuPressed();
                    if (x == 0) {  // Jugar
                        // Inicializar tablero y jugadores
                        Tablero tablero;
                        Jugador jugadorAzul('a');
                        Jugador jugadorRojo('r');

                        jugadorAzul.cartas[0] = new CartaLeon();
                        jugadorAzul.cartas[1] = new CartaFenix();

                        jugadorRojo.cartas[0] = new CartaTigre();
                        jugadorRojo.cartas[1] = new CartaOso();

                        Carta* cartaCentro = new CartaDragon();

                        tablero.colocarFicha(new Rey('a'), 0, 2);
                        tablero.colocarFicha(new Rey('r'), 4, 2);
                        tablero.colocarFicha(new Peon('a'), 0, 1);
                        tablero.colocarFicha(new Peon('a'), 0, 3);
                        tablero.colocarFicha(new Peon('r'), 4, 1);
                        tablero.colocarFicha(new Peon('r'), 4, 3);

                        bool turnoRojo = true;

                        BoardView view;
                        view.loadResources();

                        while (Play.isOpen()) {
                            Event aevent;
                            while (Play.pollEvent(aevent)) {
                                if (aevent.type == Event::Closed) {
                                    Play.close();
                                }
                                if (aevent.type == Event::KeyPressed && aevent.key.code == Keyboard::Escape) {
                                    Play.close();
                                }
                                if (aevent.type == Event::KeyPressed && aevent.key.code == Keyboard::Space) {
                                    turnoRojo = !turnoRojo; // alternar turno para demostracion
                                }
                            }

                            const Jugador& jugadorActual = turnoRojo ? jugadorRojo : jugadorAzul;

                            Play.clear();
                            view.draw(Play, tablero, jugadorActual, cartaCentro);
                            Play.display();
                        }
                        delete cartaCentro;
                    }
                    if (x == 1) {  // Opciones
                        while (OPTIONS.isOpen()) {
                            Event aevent;
                            while (OPTIONS.pollEvent(aevent)) {
                                if (aevent.type == Event::Closed) {
                                    OPTIONS.close();
                                }
                                if (aevent.type == Event::KeyPressed) {
                                    if (aevent.key.code == Keyboard::Escape) {
                                        OPTIONS.close();
                                    }
                                }
                            }

                            Play.close();
                            OPTIONS.clear();
                            OPTIONS.display();
                        }
                    }
                    if (x == 2) {  // Salir
                        MENU.close();
                    }
                }
            }
        }

        MENU.clear();
        MENU.draw(background);
        mainMenu.draw(MENU);  // Dibuja el menú
        MENU.display();
    }

    return 0;
}
