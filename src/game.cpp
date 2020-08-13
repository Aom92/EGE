#include <iostream>
#include <core/Entity.tpp>
#include <core/System.tpp>
#include <core/Component.tpp>
#include <core/Manager.tpp>
#include <std/Windows/WindowsTerminal.tpp>
#include <std/Windows/Sprite.tpp>
#include <std/Windows/Position.tpp>
#include <std/Windows/Controller.tpp>
#include <std/Windows/Input.tpp>
#include <std/Windows/visualizeEntity.tpp>
#include <std/Windows/displacementEntity.tpp>
#include <std/Windows/systemGenericCollition.tpp>
#include <std/Windows/systemKeyInverter.tpp>
#include <vector>


using namespace EGE::STD::TERMINAL::WINDOWS;

class pacman : public EGE::CORE::Entity<pacman>{
    public:
        pacman(int id) : Entity(id){

        }
};

class managerPacman :  public EGE::STD::TERMINAL::WINDOWS::mSprite<pacman>{

};

class tablero : public EGE::CORE::Entity<tablero>{
    public:
        tablero(EGE::CORE::EntityId id) : Entity(id){

        }
};

class managerTablero : public EGE::STD::TERMINAL::WINDOWS::mSprite<tablero>{

};

class collectible : public EGE::CORE::Entity<collectible>{
    public:
        collectible(EGE::CORE::EntityId id) : Entity(id){

        }
};

class managerCollectible : public EGE::STD::TERMINAL::WINDOWS::mSprite<collectible>{

};




int main(){
    
    //Inicializar tablero de juego
    mTerminal terminal;
    //Definir dimensiones
    auto gameterminal = terminal.addEntity(41,32);

    /// Creacion de los sistemas
    
    //Sistema de Entrada
    systemInput entrada;

    //Sistemas para las entidades
    visualizeEntity<managerPacman> viewPacman;
    displacementEntity<managerPacman> disPacman;
    visualizeEntity<managerCollectible> viewCoins;
    visualizeEntity<managerTablero> viewBoard;

    //Sistema de colision entre las entidades
    systemGenericCollition<managerPacman,managerTablero> colition;
    systemKeyInverter inverter;
    systemGenericCollition<managerPacman,managerCollectible> colition2;


    //Creacion de los managers de las entidades
    managerPacman player;
    managerTablero board;
    managerCollectible coins;

    //Creacion de las entidades como tal
    auto Pacman1 = player.addEntity();
    auto board1 = board.addEntity();
    int coins1 = coins.addEntity();

    std::vector<int> coinstash;
    for(int i; i<20; i++){

        coinstash.push_back(coins.addEntity());
        coins.spriteInitializer(coinstash.at(i),1,"coin");
        coins.positionInitializer(coinstash.at(i),5+i,6);
    }






    //Agregar e inicializar componentes
    player.addComponent<Controller>(Pacman1);
    player.spriteInitializer(Pacman1,2,"pacman");
    player.positionInitializer(Pacman1,8,5);

    board.spriteInitializer(board1,40,"laberinto");
    board.positionInitializer(board1,1,1);

    coins.spriteInitializer(coins1,1,"coin");
    coins.positionInitializer(coins1,5,6);

    //Empezar la terminal y generar game loop

    terminal.terminalPersonalized(gameterminal,'=','I');
    terminal.terminalDefault(gameterminal);
    char Tecla = 0;
    int id;
    while(Tecla != 'c'){

        //Leer input
        Tecla = entrada.update();

        // Mover al jugador
        disPacman.update(Tecla,Pacman1,&player,ARROWS);

        //Revisar colisiones
        if(Tecla != 0){
            if(colition.collition(Pacman1,&player,&board)){

                disPacman.update(inverter.update(Tecla,ARROWS),Pacman1,&player,ARROWS);

            }


            if(colition2.collitionId(Pacman1,&player,&coins,&id)){
                coins.destroyEntity(id);
            }

        }

        
        //Dibujamos

        viewPacman.update(Pacman1,&player);
        viewBoard.update(board1,&board);
        
        for( auto moneda : coins.getEntities()){
            viewCoins.update(moneda.first,&coins);
        }
        
        Sleep(3);
        
    }



}