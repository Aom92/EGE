#include "..\..\include\core\Component.hpp"

namespace EGE{
    template<class Type>
    Component<Type>::Component(){
        /*Nada*/
    }

    template<class Type>
    Component<Type>::Component(ComponentType type){
        this -> idComponent = idEntity;
    }

    template<class Type>
    Component<Type>::~Component(){
        /*Nada*/
    }

    
    template<class Type>
    ComponentType Component<Type>::getComponentType(){

        static ComponentType _type = nextType++;

        return _type;
    }

    ComponentBase::ComponentBase(){
        /*Nada*/
    }

    ComponentBase::~ComponentBase(){
        /*Nada*/
    }
}