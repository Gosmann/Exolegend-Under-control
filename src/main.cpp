#include "gladiator.h"
#include "user_utils.h"
#include <math.h>
#include <vector>
#include <algorithm>

Gladiator* gladiator;
using namespace std;

enum Direction {
  EAST=0, NORTH=1, WEST=2, SOUTH=3
};

int currentDirection = Direction::EAST;
vector<int> visitedPlaces = {};
int initial_i = 0;
int initial_j = 0;

void reset();
void setup() {
    //instanciation de l'objet gladiator
    gladiator = new Gladiator();
    //enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset);
}

void reset() {
    //fonction de reset:
    //initialisation de toutes vos variables avant le début d'un match
    gladiator->log("Appel de la fonction de reset");
    gladiator->log("Ang");
    RobotData data = gladiator->robot->getData();
    Position pos = data.position;
    MazeSquare current = gladiator->maze->getNearestSquare();
    initial_i = current.i;
    initial_j = current.j;
    double ang = pos.a;
    if((ang>=0&&ang<=M_PI/4)||(ang>=(7/4)*M_PI&&ang<=2*M_PI)){
        currentDirection = Direction::EAST;
    }else if(ang>=M_PI/4&&ang<=(3/4)*M_PI){
        currentDirection = Direction::NORTH;
    }else if(ang>=(3/4)*M_PI&&ang<=(5/4)*M_PI){
        currentDirection = Direction::WEST;
    }else{
        currentDirection = Direction::SOUTH;
    }
    gladiator->log(to_string(currentDirection).c_str());
    gladiator->log("Ang");
}



void noventa_graus(bool antiHorario){
    RobotData data = gladiator->robot->getData();
    Position pos = data.position;
    MazeSquare square = gladiator->maze->getNearestSquare();

    visitedPlaces.insert(visitedPlaces.end(),square.i*10+square.j);

    double ang = pos.a;
    if(antiHorario){
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, -0.05);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.05);
        currentDirection+=1;
    }else{
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.05);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -0.05);
        currentDirection-=1;
    }
    if(currentDirection==-1){
        currentDirection = Direction::SOUTH;
    }
    if(currentDirection==4){
        currentDirection = Direction::EAST;
    }
    gladiator->log("Inicial");
    while(abs(data.position.a-ang)<0.87*(3.14/2)){
        data = gladiator->robot->getData();
    }
    gladiator->log("Final");
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);
    delay(1000);
}

void frente(){

    RobotData data = gladiator->robot->getData();
    Position pos = data.position;
    double ang = pos.a;

    // if(currentDirectionn==Direction::NORTH){
    //     ang=M_PI/2;
    // }
    // if(currentDirection==Direction::EAST){
    //     ang=0;
    // }
    // if(currentDirection==Direction::WEST){
    //     ang=M_PI;
    // }
    // if(currentDirection==Direction::SOUTH){
    //     ang=3*M_PI/2;
    // }

    MazeSquare square = gladiator->maze->getNearestSquare();
    MazeSquare current_square = square;
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.05);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.05);

    double erro = 0;
    double Kp = 0.07;
    while(current_square.i==square.i&&current_square.j==square.j){
        erro = gladiator->robot->getData().position.a-ang;

        gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.05+Kp*erro);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.05-Kp*erro);

        current_square = gladiator->maze->getNearestSquare();
    }
    delay(1200);
    visitedPlaces.insert(visitedPlaces.end(),current_square.i*10+current_square.j);
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);
}

int notVisitedSquare(MazeSquare* square){
    return 1-std::count(visitedPlaces.begin(), visitedPlaces.end(), square->i*10+square->j);
}

void loop() {
    if(gladiator->game->isStarted()) { //tester si un match à déjà commencer
        //code de votre stratégie
        gladiator->log("Le jeu a commencé");
        MazeSquare currentSquare = gladiator->maze->getNearestSquare();
        if(currentSquare.j>=initial_j){
            if(currentSquare.southSquare!=NULL&&notVisitedSquare(currentSquare.southSquare)){
                while(currentDirection!=Direction::SOUTH){
                    noventa_graus(false);
                }
                frente();
            }else if(currentSquare.eastSquare!=NULL&&notVisitedSquare(currentSquare.eastSquare)){
                while(currentDirection!=Direction::EAST){
                    noventa_graus(false);
                }
                frente();
            }else if(currentSquare.northSquare!=NULL&&notVisitedSquare(currentSquare.northSquare)){
                while(currentDirection!=Direction::NORTH){
                    noventa_graus(true);
                }
                frente();
            }
        }else{
            if(currentSquare.northSquare!=NULL&&notVisitedSquare(currentSquare.northSquare)){
                while(currentDirection!=Direction::NORTH){
                    noventa_graus(true);
                }
                frente();
            } else if(currentSquare.eastSquare!=NULL&&notVisitedSquare(currentSquare.eastSquare)){
                while(currentDirection!=Direction::EAST){
                    noventa_graus(false);
                }
                frente();
            }else if(currentSquare.southSquare!=NULL&&notVisitedSquare(currentSquare.southSquare)){
                while(currentDirection!=Direction::SOUTH){
                    noventa_graus(false);
                }
                frente();
            }
        }
    } else {
        gladiator->log("Le jeu n'a pas encore commencé");
    }
    delay(300);
}