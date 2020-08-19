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
#include <std/Windows/systemScore.tpp>
#include <vector>

#include <Windows.h>
#include <fstream>
#include <thread>

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


//Funciones Extras

double PCfreq = 0.0;
__int64 CounterStart = 0;

/**
 * @brief Empieza el Contador del Sistema
 * @author @Aom92
*/
void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li)){
        std::cout << "QueryPerfomanceFrequency Fallo!" << std::endl;
    }

    PCfreq = double(li.QuadPart)/1000.0;
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

/**
 * @brief Retorna el tiempo en milisegundos que han pasado desde que StartCounter() fue llamado
 * @author @Aom92
*/
double GetCounter(){
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart)/ PCfreq;
}




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

    //Inicio del Game Loop

    int ups =30.0; int fps = 30.0;

    StartCounter();
    double TiempoInicial = GetCounter();
    double timeU = 1000 / ups;
    double timeF = 1000 / fps;

    double deltaU = 0;
    double deltaF = 0;

    int frames = 0;
    int ticks = 0;

    double timer = GetCounter();


//INICIALIZAMOS LOS SPRITES QUE SOLO SE DIBUJAN 1 VEZ
    viewBoard.viewColor(board1,&board,15);

    terminal.terminalSetColor(gameterminal,10);
    for( auto moneda : coins.getEntities()){
        viewCoins.view(moneda.first,&coins);
    }

    while(Tecla != 'c'){
        
        double TiempoActual = GetCounter();

        deltaU += (TiempoActual - TiempoInicial ) / timeU;
        deltaF += (TiempoActual - TiempoInicial) / timeF;
        TiempoInicial = TiempoActual;

        //Leer input
        Tecla = entrada.update();


        if(deltaU >= 0.1){
            Sleep(10);
            
            //Updates
            // Mover al jugador
            disPacman.update(Tecla,Pacman1,&player,ARROWS);
            

            if(colition.collition(Pacman1,&player,&board)){

                    disPacman.update(inverter.update(Tecla,ARROWS),Pacman1,&player,ARROWS);
                    viewBoard.viewColor(board1,&board,15);

            }

            viewPacman.viewColor(Pacman1,&player,14);
        
        
        ticks++;
        deltaU--;

        }

        if(deltaF >= 0.1){

            Sleep(7);
            //Revisar colisiones

            if(Tecla != 0){
            
                if(colition2.collitionId(Pacman1,&player,&coins,&id)){
                    int tmp = id;
                    
                    coins.destroyEntity(id);
                    viewPacman.viewColor(Pacman1,&player,14);
                }

            }
        frames++;
        deltaF--;
        }

        if(GetCounter() - timer > 1000){
            
            scoreboard.update(frames,board1,&board);
            viewBoard.viewColor(board1,&board,15);
        
            frames=0;
            timer+= 1000;
            

        }
                     
    }

}