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

struct Mano {
    std::vector<int> dados;
    int vueltasDisp;
    int ganador1;
    int ganador2;
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
};


class Sanguche : public Jugador {
    private:
    std::string nombreEstudiante;

    public:

    Sanguche(std::string nom = "Sanguche") : Jugador(nom) {
        nombreEstudiante = "Benjamin Mardones";
    }

    int jugar(const std::map<std::string, Marcador>& marcadores, const std::vector<Actuacion>& actuacionesPosibles, const std::vector<int>& dados, const Anotacion& resultadoPrevio) override {

        marcadores.at(nombre).display();

        for (size_t i = 0; i < actuacionesPosibles.size(); ++i)
            if (actuacionesPosibles[i].accion == "dormida") {
                std::cout << "ME DORMI, mi mano es ";
                imprimir_vector(dados);

                return i;
            }

        int max_jugada_mayor = 0;
        int opcion = -1;

        for (size_t i = 0; i < actuacionesPosibles.size(); ++i)
            if (actuacionesPosibles[i].accion == "anotar") {
                int puntaje = actuacionesPosibles[i].anotacion.puntos;
                if (puntaje >= 30 && puntaje > max_jugada_mayor) {
                max_jugada_mayor = puntaje;
                opcion = i;
                }
            }

        if (opcion != -1) {
            std::cout << "elegi una jugada alta, mi mano es ";
            imprimir_vector(dados);
            return opcion;
        }

        
    // no hay buenas opciones de anotacion, hay que lanzar dados

const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string GREEN = "\033[32m";
    const std::string BLUE = "\033[34m";
    const std::string RED = "\033[31m";
    const std::string YELLOW = "\033[33m";

    std::cout << "\n"
              << BOLD << "========================================" << RESET
              << std::endl;
    std::cout << BOLD << "TURNO DE: " << GREEN << nombre << RESET << std::endl;
    std::cout << BOLD << "Dados actuales: " << YELLOW;
    for (int d : dados)
      std::cout << "[" << d << "] ";
    std::cout << RESET << std::endl;

    std::cout << "         Index: ";
    for (int i = 0; i < 5; ++i)
      std::cout << " (" << i << ") ";
    std::cout << std::endl;

    std::cout << BOLD << "Intento: " << intento
              << " | Lanzamiento: " << lanzamiento << RESET << std::endl;
    std::cout << BOLD << "========================================" << RESET
              << std::endl;

    // Categorize options
    std::vector<int> scoringIdx;
    std::vector<int> rerollIdx;
    std::vector<int> otherIdx;

    for (size_t i = 0; i < actuacionesPosibles.size(); ++i) {
      const std::string &acc = actuacionesPosibles[i].accion;
      if (acc == "anotar" || acc == "tachar" || acc == "dormida" ||
          acc == "sobre") {
        scoringIdx.push_back(i);
      } else if (acc == "lanzar") {
        rerollIdx.push_back(i);
      } else {
        otherIdx.push_back(i);
      }
    }

    if (!scoringIdx.empty()) {
      std::cout << BOLD << BLUE << "\n--- OPCIONES DE ANOTACIÓN ---" << RESET
                << std::endl;
      for (int i : scoringIdx) {
        const auto &a = actuacionesPosibles[i];
        if (a.accion == "dormida") {
          std::cout << "[" << i << "] " << GREEN << BOLD
                    << "DORMIDA! (Gana el juego)" << RESET << std::endl;
        } else if (a.accion == "sobre") {
          std::cout << "[" << i << "] " << BLUE << BOLD
                    << "RESERVAR (Sobre): " << RESET << BOLD
                    << a.anotacion.juego << RESET << " (+" << a.anotacion.puntos
                    << " pts)" << std::endl;
        } else {
          bool esTachar = (a.anotacion.puntos == 0);
          std::cout << "[" << i << "] " << (esTachar ? RED : GREEN)
                    << (esTachar ? "Tachar " : "Anotar ") << BOLD
                    << a.anotacion.juego << RESET << " (+" << a.anotacion.puntos
                    << " pts)" << std::endl;
        }
      }
    }

    if (!rerollIdx.empty()) {
      std::cout << BOLD << YELLOW << "\n--- OPCIONES DE RELANZAMIENTO ---"
                << RESET << std::endl;
      for (int i : rerollIdx) {
        const auto &a = actuacionesPosibles[i];
        std::cout << "[" << i << "] " << BLUE << "Relanzar índices: " << RESET
                  << "[ ";
        for (int d : a.indiceDados)
          std::cout << d << " ";
        std::cout << "]" << (a.seRespeta ? GREEN + " (Respetando)" + RESET : "")
                  << std::endl;
      }
    }

    if (!otherIdx.empty()) {
      std::cout << BOLD << RED << "\n--- OTRAS OPCIONES ---" << RESET
                << std::endl;
      for (int i : otherIdx) {
        const auto &a = actuacionesPosibles[i];
        if (a.accion == "nada") {
          std::cout << "[" << i << "] " << RED << BOLD
                    << "NO HACER NADA (Pasar)" << RESET << std::endl;
        } else {
          std::cout << "[" << i << "] " << a.accion << std::endl;
        }
      }
    }
    
        std::vector<Mano> manos = manos_posibles(dados);
        double max_probabilidad = 0.0;

        std::cout << "mis manos posibles" << std::endl;
        for (size_t i = 0; i < manos.size(); ++i) {
            manos[i].display();
            if (manos[i].probabilidad >= max_probabilidad) {
                max_probabilidad = manos[i].probabilidad;
                opcion = i;
            }
        }

        std::cout << "==================================\n La mano con la mayor probabilidad es:\n";
        manos[opcion].display();
        std::cout << "elige tu opcion:" << std::endl;
        std::cin >> opcion;

        return opcion;
    }

    /*std::vector<Mano> manos_posibles(const std::vector<int>& dados) {
        std::vector<Mano> result;
        int n = dados.size();

        //no vuelta
        result.push_back(definir_ganadores(dados, 2));

        //una vuelta
        for (int i = 0; i < n; ++i) {
            std::vector<int> nuevo = dados;
            nuevo[i] = 7 - nuevo[i];
            result.push_back(definir_ganadores(nuevo, 1));
        }

        //dos vueltas
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; j++) {
                std::vector<int> nuevo = dados;
                nuevo[i] = 7 - nuevo[i];
                nuevo[j] = 7 - nuevo[j];

                result.push_back(definir_ganadores(nuevo, 0));
        }

        return result;
    }
*/
    void definir_ganadores(Mano& m) {
        std::vector<int> frecuencia(7, 0), ganadores;

        m.ganador1 = 0;
        m.ganador2 = 0;

        for (int dado: m.dados)
            ++frecuencia[dado];
        
        int frecuenciaMayor = *std::max_element(frecuencia.begin() + 1, frecuencia.end());

        if (frecuenciaMayor > 1) {
            for (int i = 0; i <= 6; ++i)
                if (frecuencia[i] == frecuenciaMayor)
                    ganadores.push_back(i);
        
            m.ganador1 = ganadores[0];

            if (ganadores.size() > 1)
                m.ganador2 = ganadores[1];
        }
    }

    void probabilidad_maximo(Mano &m) {
        if (m.ganador1 != 0) {
            for (size_t i = 0; i < m.dados.size(); ++i)
                if (m.dados[i] != m.ganador1)
                    m.aRelanzar.push_back(i);
        
            int n = m.aRelanzar.size();
            if (m.vueltasDisp == 0)
                m.probabilidad = pow(0.17, n);
            else if (n == 1 && m.vueltasDisp == 1)
                m.probabilidad = 0.33;
            else if (n == 1 && m.vueltasDisp == 2)
                m.probabilidad = 0.17;
            else
                m.probabilidad = 0.33 * pow(0.17, n - 1);
        }
    }
    /* Probabilidad_numero: calcula la probabilidad de que una mano maximice el valor val,
    si val no coincide con ninguno de los dos ganadores, la descarta (probabilidad 0)*/
    void probabilidad_numero(Mano& m, int val) {
        if (val == m.ganador1 || val == m.ganador2) {
            for (size_t i = 0; i < m.dados.size(); ++i)
                if (m.dados[i] != val)
                    m.aRelanzar.push_back(i);
            int n = m.aRelanzar.size();
            
            if (m.vueltasDisp == 0)
                m.probabilidad = pow(0.17, n);
            else if (n == 1 && m.vueltasDisp == 1)
                m.probabilidad = 0.33;
            else if (n == 1 && m.vueltasDisp == 2)
                m.probabilidad = 0.17;
            else
                m.probabilidad = 0.33 * pow(0.17, n - 1);
        }
    }


    /* probabilidad full: calcula la probabilidad de que una mano obtenga un full*/
    void probabilidad_full(Mano& m) {
        if (m.ganador2 != 0) {
            for (size_t i = 0; i < m.dados.size(); ++i)
                if (m.dados[i] != m.ganador1 && m.dados[i] != m.ganador2)
                    m.aRelanzar.push_back(i);
            
            if (m.vueltasDisp == 2 || m.vueltasDisp == 0)
                m.probabilidad = 0.33;
            else
                m.probabilidad = 0.67;
        } else
            m.probabilidad = 0.0;
    }

    void probabilidad_escalera(Mano& m) {
        if (m.ganador1 = 0) {
            std::vector<int> frecuencia(7, 0);

            for (int dado : m.dados)
                ++frecuencia[dado];
            
            
        }
    }

    std::string getNombreEstudiante() const {
        return nombreEstudiante;
    }
};

#endif