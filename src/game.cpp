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
#include <std/Windows/systemVisualizeEntity.tpp>
#include <std/Windows/systemDisplacementEntity.tpp>
#include <std/Windows/systemGenericCollition.tpp>
#include <std/Windows/systemKeyInverter.tpp>
#include <std/Windows/systemScore.hpp>
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
    systemVisualizeEntity<managerPacman> viewPacman;
    systemDisplacementEntity<managerPacman> disPacman;
    systemVisualizeEntity<managerCollectible> viewCoins;
    systemVisualizeEntity<managerTablero> viewBoard;
    systemScore<managerTablero> scoreboard;

    //Sistema de colision entre las entidades
    systemGenericCollition<managerPacman,managerTablero> colition;
    systemKeyInverter inverter;
    systemGenericCollition<managerPacman,managerCollectible> colition2;


    //Creacion de los managers de las entidades
    managerPacman player;
    managerTablero board;
    managerTablero coinboard;
    managerCollectible coins;

    //Creacion de las entidades como tal
    auto Pacman1 = player.addEntity();
    auto board1 = board.addEntity();
    int coins1 = coins.addEntity();



    Sprite coinlayout;
    coinlayout.spriteInitializer(40,"coinlayout",'*');
    std::vector<Pixel> temp = coinlayout.getSprite();
    std::vector<int> coinstash;

    int it=0;
    for(auto pixel : temp){
        coinstash.push_back(coins.addEntity());
        coins.spriteInitializer(coinstash.at(it),1,"coin");
        coins.positionInitializer(coinstash.at(it),pixel.getX()+1,pixel.getY()+1);
        it++;
    }



    //Agregar e inicializar componentes
    player.addComponent<Controller>(Pacman1);
    player.spriteInitializer(Pacman1,2,"pacman");
    player.positionInitializer(Pacman1,8,5);

    board.spriteInitializer(board1,40,"laberinto");
    board.positionInitializer(board1,1,1);

    coins.spriteInitializer(coins1,1,"coin");
    coins.positionInitializer(coins1,5,6);

    scoreboard.scoreInitializer(board1,&board);


    //Empezar la terminal y generar game loop
    terminal.terminalSetColor(gameterminal,5);
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
                scoreboard.update(1,board1,&board);
            }

        }

        
        //Dibujamos

        viewPacman.viewColor(Pacman1,&player,14);
        viewBoard.viewColor(board1,&board,15);
        
        for( auto moneda : coins.getEntities()){
            viewCoins.viewColor(moneda.first,&coins,10);
        }
        
        //Sleep(10);
        
    }



}