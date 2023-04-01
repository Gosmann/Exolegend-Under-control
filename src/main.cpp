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

void curva(Direction sentido);

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
    //gladiator->control->setWheelSpeed(WheelAxis::LEFT, -0.05);
    //gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.05);

}

double angleFromDirection(Direction sentido){
    if(sentido==Direction::EAST){
        return 0;
    }
    if(sentido==Direction::NORTH){
        return M_PI_2;
    }
    if(sentido==Direction::WEST){
        return M_PI;
    }
    if(sentido==Direction::SOUTH){
        return -M_PI;
    }
}

bool equalSign(double a, double b){
    if(a!=0&&b!=0){
        int sign_a = a/abs(a);
        int sign_b = b/abs(b);
        return (sign_a==sign_b);
    }
    if(a==0&&b==0){
        return true;
    }
    return false;
}

void curva(Direction sentido){
    RobotData data = gladiator->robot->getData();
    double Kp = 0.15;
    if(currentDirection==sentido){
        return;
    }
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, -0.05);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.05);

    double currentAngle = gladiator->robot->getData().position.a;

    if(sentido==Direction::NORTH||sentido==Direction::SOUTH){
        while(!equalSign(sin(currentAngle),sin(angleFromDirection(sentido)))){
            currentAngle = gladiator->robot->getData().position.a;
        }
        while(cos(currentAngle)<cos(angleFromDirection(sentido)*0.9)||cos(currentAngle)>cos(angleFromDirection(sentido)*0.9)){

        }
        double erro = angleFromDirection(sentido)-currentAngle;
        while(abs(erro)>0.01){
            currentAngle = gladiator->robot->getData().position.a;
            erro = angleFromDirection(sentido)-currentAngle;
            if(sentido==Direction::NORTH){
                gladiator->control->setWheelSpeed(WheelAxis::LEFT, -Kp*erro);
                gladiator->control->setWheelSpeed(WheelAxis::RIGHT, Kp*erro);
            }else{
                gladiator->control->setWheelSpeed(WheelAxis::LEFT, Kp*erro);
                gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -Kp*erro);
            }
        }
    }
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);   
    currentDirection = sentido;
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

    if(currentDirection==Direction::NORTH){
        ang=M_PI/2;
    }
    if(currentDirection==Direction::EAST){
        ang=0;
    }
    if(currentDirection==Direction::WEST){
        ang=M_PI;
    }
    if(currentDirection==Direction::SOUTH){
        ang=3*M_PI/2;
    }

    MazeSquare square = gladiator->maze->getNearestSquare();
    MazeSquare current_square = square;
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.05);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.05);

    double erro = 0;
    double Kp = 0.1;
    double current_angle = gladiator->robot->getData().position.a;
    while(current_square.i==square.i&&current_square.j==square.j){
        current_angle = gladiator->robot->getData().position.a;
        if(currentDirection==Direction::EAST&&current_angle>(M_PI+M_PI_2)){
            current_angle=current_angle-(2*M_PI);
        }
        erro = current_angle-ang;

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
        //curva(Direction::SOUTH);
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.01);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -0.01);
        gladiator->log(to_string(gladiator->robot->getData().position.a).c_str());
        // curva(Direction::NORTH);
        // delay(5000);
        // // curva(Direction::WEST);
        // // delay(5000);
        // curva(Direction::SOUTH);
        // delay(5000);
    } else {
        gladiator->log("Le jeu n'a pas encore commencé");
    }
    delay(300);
}