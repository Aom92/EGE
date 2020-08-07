#ifndef SYSTEMGENERICCOLLITION_HPP
#define SYSTEMGENERICCOLLITION_HPP

#include <core/System.hpp>
#include <core/Alias.hpp>
#include <std/Position.tpp>

namespace EGE::STD::TERMINAL::WINDOWS{

    /**
     * @class Systema que verifica si un jugador choco con otras entidades.
     */
    template<typename m1, typename m2>
    class systemGenericCollition :public EGE::CORE::System<m1,m2>{
        public:
            /**
             * @brief Método que verifica si otras entidades chocaron con el jugador.
             * @param mPlayer Manager del jugador.
             * @param mEntities Manager de entidades.
             * @return True si choco el jugador en caso contrario false.
             */
            bool update(EGE::CORE::EntityId id,m1 *mPlayer, m2 *mEntities);
    };
};

#endif