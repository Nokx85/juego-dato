#include "mainMenu.h"
//con esto vemos el tipo de letra ue va a tener el menu
MainMenu::MainMenu(float width ,float height){
if (!font.loadFromFile("files/Minecraft.ttf"))
{
    cout <<"no encontradoo " ;
}
//Jugar
mainMenu[0].setFont(font);
mainMenu[0].setFillColor(Color::White);
mainMenu[0].setString("Jugar");
mainMenu[0].setCharacterSize(50);
mainMenu[0].setPosition(400,250);

//Opciones 
mainMenu[1].setFont(font);
mainMenu[1].setFillColor(Color::White);
mainMenu[1].setString("Opciones");
mainMenu[1].setCharacterSize(50);
mainMenu[1].setPosition(400,350);

//Salir del juego
mainMenu[2].setFont(font);
mainMenu[2].setFillColor(Color::White);
mainMenu[2].setString("Salir");
mainMenu[2].setCharacterSize(50);
mainMenu[2].setPosition(400,450);

MainMenuSelected = -1 ; 

}

MainMenu ::~MainMenu(){}

//dibujo 
void MainMenu :: draw(RenderWindow& window){
    for(int i = 0 ; i < Max_main_menu;++i){
        window.draw(mainMenu[i]);
    }
}

//mover arriba

void MainMenu ::MoveUp(){
    

        if (MainMenuSelected - 1 >= 0 )  {
            mainMenu[MainMenuSelected].setFillColor(Color::White);

            MainMenuSelected--;
            if (MainMenuSelected == -1)
            {
              MainMenuSelected = 2 ; 
            }
            mainMenu[MainMenuSelected].setFillColor(Color::Magenta);



        }

}

//mover Abajo

void MainMenu::MoveDown(){
if (MainMenuSelected + 1  <=  Max_main_menu)
{
    mainMenu[MainMenuSelected].setFillColor(Color::White);
    MainMenuSelected++;
    if (MainMenuSelected == 3)
    {
        MainMenuSelected = 0 ;
    }
    mainMenu[MainMenuSelected].setFillColor(Color::Magenta);


}


}