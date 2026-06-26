#ifndef SANGUCHE_H
#define SANGUCHE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Jugador.h"
#include "Marcador.h"
#include "Actuacion.h"


void imprimir_vector(const std::vector<int>& vector) {
        std::cout << "[ ";
        for (size_t i = 0; i < vector.size(); ++i)
            std::cout << vector[i] << (i == vector.size() - 1 ? ' ' : ',');
        std::cout << "]" << std::endl;
}

// Struct Mano: Mano que guarda dados, vueltas, ganadores, probabilidades, etc
struct Mano {
    std::vector<int> dados;
    int vueltasDisp;
    int ganador1 = 0;
    int ganador2 = 0;
    std::vector<int> aRelanzar;
    double probabilidad = 0.0;

    void display() {
        imprimir_vector(dados);
        std::cout << " vueltas disponibles = " << vueltasDisp;
        std::cout << "\n ganador1 = " << ganador1 << "\n ganador2 = " << ganador2;
        std::cout << "\n a relanzar:";
        imprimir_vector(aRelanzar);
        std::cout << " probabilidad =" << probabilidad << std::endl;
    }

    void definir_ganadores() {
        std::vector<int> frecuencia(7, 0), ganadores;

        for (int dado: dados)
            ++frecuencia[dado];
        
        int frecuenciaMayor = *std::max_element(frecuencia.begin() + 1, frecuencia.end());

        if (frecuenciaMayor > 1) {
            for (int i = 0; i <= 6; ++i)
                if (frecuencia[i] == frecuenciaMayor)
                    ganadores.push_back(i);
        
            ganador1 = ganadores[0];

            if (ganadores.size() > 1)
                ganador2 = ganadores[1];
        }
    }


    void probabilidad_maximo() {
        probabilidad = 0.0;
        aRelanzar.clear();
        if (ganador1 != 0) {
            for (size_t i = 0; i < dados.size(); ++i)
                if (dados[i] != ganador1)
                    aRelanzar.push_back(i);
        
            int n = aRelanzar.size();

            if (n == 0)
                return;

            if (vueltasDisp == 0)
                probabilidad = pow(0.17, n);
            else if (n == 1 && vueltasDisp == 1)
                probabilidad = 0.33;
            else if (n == 1 && vueltasDisp == 2)
                probabilidad = 0.17;
            else
                probabilidad = 0.33 * pow(0.17, n - 1);
        }
    }

    void probabilidad_numero(int val) {
        probabilidad = 0.0;
        aRelanzar.clear();
        if (val == ganador1 || val == ganador2) {
            for (size_t i = 0; i < dados.size(); ++i)
                if (dados[i] != val)
                    aRelanzar.push_back(i);
            
            int n = aRelanzar.size();

            if (n == 0)
                return;
        
            if (vueltasDisp == 0)
                probabilidad = pow(0.17, n);
            else if (n == 1 && vueltasDisp == 1)
                probabilidad = 0.33;
            else if (n == 1 && vueltasDisp == 2)
                probabilidad = 0.17;
            else
                probabilidad = 0.33 * pow(0.17, n - 1);
        }
    }

    void probabilidad_full() {
        probabilidad = 0.0;
        aRelanzar.clear();
        if (ganador2 != 0) {
            for (size_t i = 0; i < dados.size(); ++i)
                if (dados[i] != ganador1 && dados[i] != ganador2)
                    aRelanzar.push_back(i);
            
            if (vueltasDisp == 2 || vueltasDisp == 0)
                probabilidad = 0.33;
            else
                probabilidad = 0.67;
        } else if (ganador1 != 0) {
            for (size_t i = 0; i < dados.size(); ++i)
                if (dados[i] != ganador1)
                    aRelanzar.push_back(i);

            int n = aRelanzar.size();

            if (n == 0)
                return;

            if (n != 1) {
                if (vueltasDisp == 0)
                    probabilidad = pow(0.17, n);
                else
                    probabilidad = 0.33 * pow(0.17, n - 1);
            }
        }
    }


    void probabilidad_escalera() {
        probabilidad = 0.0;
        aRelanzar.clear();

        std::vector<std::vector<int>> escalera(3);

        std::vector<int> frecuencia(7, 0), ganadores;

        for (int i = 0; i < dados.size(); ++i)
            if (dados[i] == 6)
                escalera[0].push_back(i);
            else if (frecuencia[dados[i]] > 0)
                escalera[0].push_back(i);
            else
                ++frecuencia[dados[i]];

        std::fill(frecuencia.begin() + 1, frecuencia.end(), 0);

        for (int i = 0; i < dados.size(); ++i)
            if (dados[i] == 1)
                escalera[1].push_back(i);
            else if (frecuencia[dados[i]] > 0)
                escalera[1].push_back(i);
            else
                ++frecuencia[dados[i]];
        
        std::fill(frecuencia.begin() + 1, frecuencia.end(), 0);

        for (int i = 0; i < dados.size(); ++i)
            if (dados[i] == 2)
                escalera[2].push_back(i);
            else if (frecuencia[dados[i]] > 0)
                escalera[2].push_back(i);
            else
                ++frecuencia[dados[i]];

        int tamaños[3] = {escalera[0].size(), escalera[1].size(), escalera[2].size()};
        int min = 5, indice = 0;

        for (int i = 0; i < 3; ++i)
            if (tamaños[i] <= min) {
                min = tamaños[i];
                indice = i;
            }

        aRelanzar = escalera[indice];

        int n = aRelanzar.size();

        if (n == 0)
            return;

        if (vueltasDisp == 0)
            probabilidad = pow(0.17, n);
        else if (n == 1 && vueltasDisp == 2)
            probabilidad = 0.17;
        else if (n == 1 && vueltasDisp == 1)
            probabilidad = 0.33;
        else
            probabilidad = 0.33 * pow(0.17, n - 1);
    }

};

//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================
class Sanguche : public Jugador {
    private:
    std::string nombreEstudiante;

    public:

    Sanguche() {
        nombre = "Sanguche";
        nombreEstudiante = "Benjamin Mardones";
    }

    int jugar(const std::map<std::string, Marcador>& marcadores, const std::vector<Actuacion>& actuacionesPosibles, const std::vector<int>& dados, const Anotacion& resultadoPrevio) override {

        // busca dormida
        for (size_t i = 0; i < actuacionesPosibles.size(); ++i)
            if (actuacionesPosibles[i].accion == "dormida") {
                return i;
            }

        std::vector<int> indicesAnotar;

        for (size_t i = 0; i < actuacionesPosibles.size(); ++i)
            if (actuacionesPosibles[i].accion == "anotar" || actuacionesPosibles[i].accion == "tachar")
                indicesAnotar.push_back(i);

        // busca jugadas de mano
        int max_auxiliar = 0;
        int opcion = -1;

        for (int i : indicesAnotar) {
            const auto& a = actuacionesPosibles[i].anotacion;

            if (a.juego == "grande" && a.puntos != 0)
                return i;
            
            
            if ((a.juego == "escalera" && a.puntos == 25) || (a.juego == "full" && a.puntos == 35) || (a.juego == "poquer" && a.puntos == 45))
                if (a.puntos > max_auxiliar) {
                    max_auxiliar = a.puntos;
                    opcion = i;
                }
        }

        if (opcion != -1)
            return opcion;
        
        // busca jugadas grandes
        for (int i : indicesAnotar) {
            int puntaje = actuacionesPosibles[i].anotacion.puntos;

            if (puntaje >= 30 && puntaje > max_auxiliar) {
                max_auxiliar = puntaje;
                opcion = i;
            }
        }

        if (opcion != -1) 
            return opcion;
    
        // jugada numerica >= 4 * numero rango 5 y 6
        for (int i : indicesAnotar) {
            const auto& a = actuacionesPosibles[i].anotacion;
            int n = numerico(a.juego);

            if (n != 0 && n >= 5 && a.puntos >= 4 * n) 
                if (a.puntos > max_auxiliar) {
                    max_auxiliar = a.puntos;
                    opcion = i;
                }
        }

        if (opcion != -1)
            return opcion;
        
        // escalera
        for (int i : indicesAnotar) {
            const auto& a = actuacionesPosibles[i].anotacion;

            if (a.juego == "escalera" && a.puntos > 0) 
                return i;
            
        }

        // jugada numerica >= 4 * numero rango 1 y 4
        for (int i : indicesAnotar) {
            const auto& a = actuacionesPosibles[i].anotacion;
            int n = numerico(a.juego);

            if (n != 0 && n <= 4 && a.puntos >= 4 * n) 
                if (a.puntos > max_auxiliar) {
                    max_auxiliar = a.puntos;
                    opcion = i;
                }
        }

        if (opcion != -1) 
            return opcion;
        
        // no hay buenas opciones de anotacion, hay que lanzar dados
        // revisa las jugadas disponibles
        std::vector<int> disponibles, indicesRelanzamiento;

        for (const auto& [juego, puntos] : marcadores.at(nombre).puntajes)
            if (puntos == -1)
                disponibles.push_back(prioridad(juego));

        std::sort(disponibles.begin(), disponibles.end());

        for (size_t i = 0; i < actuacionesPosibles.size(); ++i) {
            if (actuacionesPosibles[i].accion == "lanzar")
                indicesRelanzamiento.push_back(i);
        }

        if (indicesRelanzamiento.size() > 0) {
            // genera la probabilidad de cada jugada
            std::vector<Mano> manos_posibles = generar_manos(dados);
            Mano max_mano;

            for (size_t i = 0; i < disponibles.size(); ++i) {
                if (disponibles[i] <= 3 || disponibles[i] == 5 || disponibles[i] == 6)
                    for (auto& mano : manos_posibles)
                        mano.probabilidad_numero(numerico(prioridad_inversa(disponibles[i])));
                else if (disponibles[i] == 4) 
                    for (auto& mano: manos_posibles)
                        mano.probabilidad_escalera();
                else if (disponibles[i] == 7) 
                    for (auto& mano : manos_posibles)
                        mano.probabilidad_full();
                else 
                    for (auto& mano : manos_posibles) 
                        mano.probabilidad_maximo();
                    
                for (auto& mano: manos_posibles)
                    if (mano.probabilidad >= max_mano.probabilidad)
                        max_mano = mano;       
            }

            for (int i : indicesRelanzamiento)
                if (actuacionesPosibles[i].indiceDados == max_mano.aRelanzar)
                    return i;

            return indicesRelanzamiento.back();
        }

        // PARTE MALA: NO HAY JUGADAS BUENAS

        // busca alguna jugada numerica >= 3* numero en el rango de 1 y 4 y se queda con la mas pequeña
        int min_auxiliar = 50;

        for (int i : indicesAnotar) {
            const auto& a = actuacionesPosibles[i].anotacion;

            if (numerico(a.juego) != 0 && numerico(a.juego) <= 4 && a.puntos >= 3 * numerico(a.juego)) 
                if (a.puntos < min_auxiliar) {
                    min_auxiliar = a.puntos;
                    opcion = i;
                }
        }

        if (opcion != -1) 
            return opcion;
        
        //elige no hacer nada
        for (size_t i = 0; i < actuacionesPosibles.size(); ++i) 
            if (actuacionesPosibles[i].accion == "nada")
                return i;
        
        //marca la jugada mas chica
        min_auxiliar = 9;

        for (int i : indicesAnotar) {
            const auto& a = actuacionesPosibles[i].anotacion;

            if (prioridad(a.juego) - disponibles[0] < min_auxiliar) {
                min_auxiliar = prioridad(a.juego) - disponibles[i];
                opcion = i;
            }
        }
        
        return opcion;
    }

    std::vector<Mano> generar_manos(const std::vector<int>& dados) {
        std::vector<Mano> manos;
  
        Mano nuevo;
        nuevo.dados = dados;
        nuevo.vueltasDisp = 2;
        nuevo.definir_ganadores();

        manos.push_back(nuevo);

        for (int i = 0; i < 5; ++i) {
            Mano nuevo;
            nuevo.dados = dados;
            nuevo.dados[i] = 7 - nuevo.dados[i];
            nuevo.vueltasDisp = 1;
            nuevo.definir_ganadores();

            manos.push_back(nuevo);
        }

        for (int i = 0; i < 5; ++i)
            for (int j = i + 1; j < 5; ++j) {
                Mano nuevo;
                nuevo.dados = dados;
                nuevo.dados[i] = 7 - nuevo.dados[i];
                nuevo.dados[j] = 7 - nuevo.dados[j];
                nuevo.vueltasDisp = 0;
                nuevo.definir_ganadores();

                manos.push_back(nuevo);
            }

        return manos;
    }

    int prioridad(const std::string& juego) {
        if (juego == "balas")
            return 0;
        if (juego == "tontos")
            return 1;
        if (juego == "trenes")
            return 2;
        if (juego == "cuadras")
            return 3;
        if (juego == "escalera")
            return 4;
        if (juego == "quinas")
            return 5;
        if (juego == "senas")
            return 6;
        if (juego == "full")
            return 7;
        if (juego == "poker")
            return 8;
        if (juego == "grande" || juego == "grande2")
            return 9; 
        return -1;
    }

    int numerico(std::string juego) {
        if (juego == "balas")
            return 1;
        if (juego == "tontos")
            return 2;
        if (juego == "trenes")
            return 3;
        if (juego == "cuadras")
            return 4;
        if (juego == "quinas")
            return 5;
        if (juego == "senas")
            return 6;
        return 0;
    }
    
    std::string prioridad_inversa(int num) {
        switch (num)
        {
        case 0:
            return "balas";
            break;
        case 1:
            return "tontos";
            break;
        case 2:
            return "trenes";
            break;
        case 3:
            return "cuadras";
            break;
        case 4:
            return "escalera";
            break;
        case 5:
            return "quinas";
            break;
        case 6:
            return "senas";
            break;
        case 7:
            return "full";
            break;
        case 8:
            return "poker";
            break;
        case 9:
            return "grande";
            break;
        default:
            return NULL;
            break;
        }
    }

    std::string getNombreEstudiante() const {
        return nombreEstudiante;
    }
};

#endif