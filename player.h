#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);

      double Poda_AlfaBeta(Environment estado, int jugador, int prof_alfabeta, Environment::ActionType &accion, double alpha, double beta);
    private:
      int jugador_;
      Environment actual_;

      int nodos;
};
#endif
