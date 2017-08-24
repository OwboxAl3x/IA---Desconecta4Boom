#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){

    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;

}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){

    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);

}

// ------------------- Los tres metodos anteriores no se pueden modificar

void fichasAdyacentes(int jugador, const Environment &estado, int row, int col, int &juntasV, int &juntasH, int &juntasD){

	juntasV=juntasH=juntasD=0;


	//Comprobamos desde la fila de la ficha hacia arriba cuantas fichas del mismo jugador hay adyacentes verticales
	for(int i=1; row-i>=0; i++){

		if(estado.See_Casilla(row-i, col)!=jugador) break;
		else if(estado.See_Casilla(row-i, col)==jugador){

			juntasV++;

		}

	}
	//Comprobamos desde la fila de la ficha hacia abajo cuantas fichas del mismo jugador hay adyacentes verticales
	for(int i=1; row+i<7; i++){

		if(estado.See_Casilla(row+i, col)!=jugador) break;
		else if(estado.See_Casilla(row+i, col)==jugador){

			juntasV++;

		}

	}
	//Comprobamos desde la columna de la ficha hacia la izquierda cuantas fichas del mismo jugador hay adyacentes horizontales
	for(int i=1; col-i>=0; i++){

		if(estado.See_Casilla(row, col-i)!=jugador) break;
		else if(estado.See_Casilla(row, col-i)==jugador){

			juntasH++;

		}

	}
	//Comprobamos desde la columna de la ficha hacia la derecha cuantas fichas del mismo jugador hay adyacentes horizontales
	for(int i=1; col+i<7; i++){

		if(estado.See_Casilla(row, col+i)!=jugador) break;
		else if(estado.See_Casilla(row, col+i)==jugador){

			juntasH++;

		}

	}
	//Comprobamos desde la columna de la ficha hacia la derecha y abajo cuantas fichas del mismo jugador hay adyacentes diagonales
	for(int i=1; i<4; i++){

		if(row+i==7 or col+i==7) break;

		if(estado.See_Casilla(row+i, col+i)!=jugador) break;
		else if(estado.See_Casilla(row+i, i+i)==jugador){

			juntasD++;

		}

	}
	//Comprobamos desde la columna de la ficha hacia la derecha y arriba cuantas fichas del mismo jugador hay adyacentes diagonales
	for(int i=1; i<4; i++){

		if(row-i==-1 or col+i==7) break;

		if(estado.See_Casilla(row-i, col+i)!=jugador) break;
		else if(estado.See_Casilla(row-i, col+i)==jugador){

			juntasD++;

		}

	}
	//Comprobamos desde la columna de la ficha hacia la izquierda y abajo cuantas fichas del mismo jugador hay adyacentes diagonales
	for(int i=1; i<4; i++){

		if(row+i==7 or col-i==-1) break;

		if(estado.See_Casilla(row+i, col-i)!=jugador) break;
		else if(estado.See_Casilla(row+i, col-i)==jugador){

			juntasD++;

		}

	}
	//Comprobamos desde la columna de la ficha hacia la izquierda y arriba cuantas fichas del mismo jugador hay adyacentes diagonales
	for(int i=1; i<4; i++){

		if(row-i==-1 or col-i==-1) break;

		if(estado.See_Casilla(row-i, col-i)!=jugador) break;
		else if(estado.See_Casilla(row-i, col-i)==jugador){

			juntasD++;

		}

	}

}


double MiPuntuacion(int jugador, const Environment &estado){

	double acumulado = 0;
	int juntasV = 0, juntasH = 0, juntasD = 0;

	for (int i=0; i<7; i++)
    	for (int j=0; j<7; j++){

    		fichasAdyacentes(jugador, estado, i, j, juntasV, juntasH, juntasD);

    		acumulado += 100*juntasV + 100*juntasH + 100*juntasD;

    	}

    return acumulado;

}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){

	int ganador = estado.RevisarTablero();
	double valor=0;
	int enemigo;

	(jugador==1) ? enemigo=2 :enemigo=1;

    if (ganador==jugador){

       return 99999999.0; // Gana el jugador que pide la valoracion

    }else if (ganador!=0){

            return -99999999.0; // Pierde el jugador que pide la valoracion

    }else if (estado.Get_Casillas_Libres()==0){

            return 0;  // Hay un empate global y se ha rellenado completamente el tablero

    }else{

    	valor -= MiPuntuacion(jugador, estado);
    	valor += MiPuntuacion(enemigo, estado);

    	return valor;

    }

}


// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){

    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}


// Funcion del algoritmo Poda Alpha-Beta
double Player::Poda_AlfaBeta(Environment estado, int jugador, int profundidad, Environment::ActionType &accion, double alpha, double beta){

	bool aplicables[8];
    int acciones = estado.possible_actions(aplicables);

    double valor;
    Environment::ActionType last_action;

   	nodos++;

	if(profundidad==0 or estado.JuegoTerminado()){

		return Valoracion(estado, jugador);

	}

	int last_act = -1; //Acciones posibles en el estado actual

	Environment sucesor = estado.GenerateNextMove(last_act);

	if(profundidad%2==0){ // Si es MAX

		int i = 0;

		while(i<estado.possible_actions(aplicables)){

			valor = Poda_AlfaBeta(sucesor, jugador, profundidad-1, last_action, alpha, beta);

			if(valor>alpha){

				alpha=valor;
				accion = static_cast< Environment::ActionType > (last_act);

			}

			if(beta<=alpha){

				break; // Podamos Beta

			}

			sucesor = estado.GenerateNextMove(last_act);
			i++;

		}

		return alpha;

	}else{ // Si es MIN

		int i = 0;

		while(i<estado.possible_actions(aplicables)){

			valor = Poda_AlfaBeta(sucesor, jugador, profundidad-1, last_action, alpha, beta);

			if(valor<beta){

				beta=valor;
				accion = static_cast< Environment::ActionType > (last_act);

			}

			if(beta<=alpha){

				break; // Podamos Alpha

			}

			sucesor = estado.GenerateNextMove(last_act);

			i++;

		}

		return beta;

	}

	

}


// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    /* //--------------------- COMENTAR Desde aqui

    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //cout << Valoracion(actual_, jugador_) << endl;

    //--------------------- COMENTAR Hasta aqui */


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------

    nodos = 0;

    valor = Poda_AlfaBeta(actual_, jugador_, PROFUNDIDAD_ALFABETA, accion, menosinf, masinf);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl << nodos << " nodos explorados" << endl;

    return accion;

}
