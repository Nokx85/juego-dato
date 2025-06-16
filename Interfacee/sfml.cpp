#include <SFML/Graphics.hpp>
#include "mainMenu.h"
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
                    RenderWindow Play(VideoMode(960, 720), "game_video");
                    RenderWindow OPTIONS(VideoMode(960, 720), "OPTIONS");
                    
                    int x = mainMenu.MainMenuPressed();
                    if (x == 0) {  // Jugar
                        while (Play.isOpen()) {
                            Event aevent; 
                            while (Play.pollEvent(aevent)) {
                                if (aevent.type == Event::Closed) {
                                    Play.close();
                                }
                                if (aevent.type == Event::KeyPressed) {
                                    if (aevent.key.code == Keyboard::Escape) {
                                        Play.close();
                                    }
                                }
                            }

                            OPTIONS.close();
                            Play.clear();
                            Play.display();
                        }
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
