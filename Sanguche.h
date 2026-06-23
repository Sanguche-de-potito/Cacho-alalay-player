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
        if (ganador1 != 0) {
            for (size_t i = 0; i < dados.size(); ++i)
                if (dados[i] != ganador1)
                    aRelanzar.push_back(i);
        
            int n = aRelanzar.size();

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
        if (val == ganador1 || val == ganador2) {
            for (size_t i = 0; i < dados.size(); ++i)
                if (dados[i] != val)
                    aRelanzar.push_back(i);
            int n = aRelanzar.size();
            
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

            if (n != 1) {
                if (vueltasDisp == 0)
                    probabilidad = pow(0.17, n);
                else
                    probabilidad = 0.33 * pow(0.17, n - 1);
            }
        }
    }


    void probabilidad_escalera() {
        return;
    }

};

// Mi jugador
class Sanguche : public Jugador {
    private:
    std::string nombreEstudiante;

    public:

    Sanguche() {
        nombre = "Sanguche";
        nombreEstudiante = "Benjamin Mardones";
    }

    int jugar(const std::map<std::string, Marcador>& marcadores, const std::vector<Actuacion>& actuacionesPosibles, const std::vector<int>& dados, const Anotacion& resultadoPrevio) override {


        
        marcadores.at(nombre).display();

        // busca dormida
        for (size_t i = 0; i < actuacionesPosibles.size(); ++i)
            if (actuacionesPosibles[i].accion == "dormida") {
                return i;
            }


        // busca la mayor jugada >= 30
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
    
        // no hay buenas opciones de anotacion, hay que lanzar dados


        // revisa las jugadas disponibles
        bool puedo_lanzar = false;

        for (size_t i = 0; i < actuacionesPosibles.size(); ++i)
            if (actuacionesPosibles[i].accion == "lanzar") {
                puedo_lanzar = true;
                break;
            }

        std::vector<int> disponibles;

        for (const auto& [juego, puntos] : marcadores.at(nombre).puntajes)
            if (puntos == -1)
                disponibles.push_back(prioridad(juego));

        std::sort(disponibles.begin(), disponibles.end());

        // genera la probabilidad de cada jugada

        std::vector<std::vector<Mano>> matriz_manos = generar_manos(dados, disponibles.size());
        for (size_t i = 0; i < disponibles.size(); ++i) {
            if (disponibles[i] == 0) {
                for (size_t j = 0; j < matriz_manos[i].size(); ++j)
                    matriz_manos[i][j].probabilidad_escalera();
            }
            else if (disponibles[i] <= 6) {
                for (size_t j = 0; j < matriz_manos[i].size(); ++j)
                    matriz_manos[i][j].probabilidad_numero(disponibles[i]);
            }
            else if (disponibles[i] == 7) {
                for (size_t j = 0; j < matriz_manos[i].size(); ++j)
                    matriz_manos[i][j].probabilidad_full();
            }
            else if (disponibles[i] >= 8) {
                for (size_t j = 0; j < matriz_manos[i].size(); ++j)
                    matriz_manos[i][j].probabilidad_maximo();
            }
        }

        double probabilidad_max = 0.0;
        Mano max_mano;

        for (auto& manos : matriz_manos)
            for (auto& mano : manos) {
                if (mano.probabilidad >= probabilidad_max) {
                    probabilidad_max = mano.probabilidad;
                    max_mano = mano;
                }
            }

        std::cout << "Mejores chances" << std::endl;
        max_mano.display();
        std::cin >> opcion;
        return opcion;
    }

    std::vector<std::vector<Mano>> generar_manos (const std::vector<int> dados, int cant) {
        std::vector<std::vector<Mano>> result;

        for (int i = 0; i < cant; ++i) {
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

            result.push_back(manos);
        }

        return result;
    }

    int prioridad(std::string juego) {
        if (juego == "escalera")
            return 0;
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
        if (juego == "full")
            return 7;
        if (juego == "poker")
            return 8;
        if (juego == "grande") 
            return 9;
        if (juego == "grande2")
            return 10;
        return -1;
    }

    std::string getNombreEstudiante() const {
        return nombreEstudiante;
    }
};

#endif