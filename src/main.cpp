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
Direction directionFromAngle(double angle);
double angleFromDirection(int sentido, float currentAngle);

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
    currentDirection = directionFromAngle(ang);
    gladiator->log(to_string(currentDirection).c_str());
    gladiator->log("Ang");
    //gladiator->control->setWheelSpeed(WheelAxis::LEFT, -0.05);
    //gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.05);

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

    ang = angleFromDirection(currentDirection,ang);

    MazeSquare square = gladiator->maze->getNearestSquare();
    MazeSquare current_square = square;
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.2);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.2);

    double erro = 0;
    double Kp = 0.1;
    double current_angle = gladiator->robot->getData().position.a;
    while(current_square.i==square.i&&current_square.j==square.j){
        current_angle = gladiator->robot->getData().position.a;
        ang = angleFromDirection(currentDirection,current_angle);
        erro = current_angle-ang;

        gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.2+Kp*erro);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.2-Kp*erro);

        current_square = gladiator->maze->getNearestSquare();
    }
    delay(200);
    visitedPlaces.insert(visitedPlaces.end(),current_square.i*10+current_square.j);
    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);
}

int notVisitedSquare(MazeSquare* square){
    return 1-std::count(visitedPlaces.begin(), visitedPlaces.end(), square->i*10+square->j);
}

Direction directionFromAngle(double angle){
    if(angle<M_PI_4&&angle>-M_PI_4){
        return Direction::EAST;
    }
    if(angle>M_PI_4&&angle<(M_PI_2+M_PI_4)){
        return Direction::NORTH;
    }
    if((angle>(M_PI_2+M_PI_4)&&angle<M_PI)||(angle<-(M_PI_2+M_PI_4)&&angle>-M_PI)){
        return Direction::WEST;
    }
    if(angle<-M_PI_4&&angle>-(M_PI_2+M_PI_4)){
        return Direction::NORTH;
    }
}

double angleFromDirection(int sentido, float currentAngle=M_PI){
    if(sentido==Direction::EAST){
        return 0;
    }
    if(sentido==Direction::NORTH){
        return M_PI_2;
    }
    if(sentido==Direction::WEST){
        if(currentAngle>0){
            return M_PI;
        }else{
            return -M_PI;
        }
    }
    if(sentido==Direction::SOUTH){
        return -M_PI_2;
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

bool shouldUseAntihorario(Direction sentido){
    if(currentDirection==Direction::NORTH){
        if(sentido==Direction::EAST){
            return false;
        }else{
            return true;
        }
    }

    if(currentDirection==Direction::EAST){
        if(sentido==Direction::SOUTH){
            return false;
        }else{
            return true;
        }        
    }

    if(currentDirection==Direction::SOUTH){
        if(sentido==Direction::WEST){
            return false;
        }else{
            return true;
        }        
    }

    if(currentDirection==Direction::WEST){
        if(sentido==Direction::NORTH){
            return false;
        }else{
            return true;
        }        
    }
}

void curva(Direction sentido){
    RobotData data = gladiator->robot->getData();
    double Kp = 0.25;
    //5% de velocidade, Kp=0.15
    if(currentDirection==sentido){
        return;
    }

    bool antiHorario = shouldUseAntihorario(sentido);

    if(antiHorario){
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, -0.2);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0.2);
    }else{
        gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0.2);
        gladiator->control->setWheelSpeed(WheelAxis::RIGHT, -0.2);
    }

    double currentAngle = gladiator->robot->getData().position.a;

    if(sentido==Direction::NORTH){
        while(!(currentAngle>M_PI/4&&currentAngle<3*M_PI/4)){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido)-currentAngle;
        while(abs(erro)>0.01){
            erro = angleFromDirection(sentido)-currentAngle;
            currentAngle = gladiator->robot->getData().position.a;
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, -Kp*erro);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    if(sentido==Direction::SOUTH){
        while(!(currentAngle<-M_PI/4&&currentAngle>-3*M_PI/4)){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido)-currentAngle;
        while(abs(erro)>0.01){
            erro = angleFromDirection(sentido)-currentAngle;
            currentAngle = gladiator->robot->getData().position.a;
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, -Kp*erro);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    if(sentido==Direction::EAST){
        while(!(currentAngle<M_PI/4&&currentAngle>-M_PI/4)){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido)-currentAngle;
        while(abs(erro)>0.01){
            erro = angleFromDirection(sentido)-currentAngle;
            currentAngle = gladiator->robot->getData().position.a;
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, -Kp*erro);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    if(sentido==Direction::WEST){
        while(!((currentAngle>2.35619&&currentAngle<3.14159)||(currentAngle>-3.14159&&currentAngle<-2.35619))){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido,currentAngle)-currentAngle;
        while(abs(erro)>0.01){
            erro = angleFromDirection(sentido,currentAngle)-currentAngle;
            currentAngle = gladiator->robot->getData().position.a;
            gladiator->control->setWheelSpeed(WheelAxis::LEFT, -Kp*erro);
            gladiator->control->setWheelSpeed(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    gladiator->control->setWheelSpeed(WheelAxis::LEFT, 0);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, 0);   
    currentDirection = sentido;
}

void loop() {
    if(gladiator->game->isStarted()) { //tester si un match à déjà commencer
        //code de votre stratégie
        gladiator->log("Le jeu a commencé");
        currentDirection = Direction::EAST;
        // curva(Direction::NORTH);
        // delay(1000);
        curva(Direction::NORTH);
        frente();
        curva(Direction::EAST);
        frente();
        frente();
        curva(Direction::NORTH);
        frente();
        curva(Direction::EAST);
        frente();
        curva(Direction::SOUTH);
        frente();
        curva(Direction::EAST);
        frente();
        frente();
        curva(Direction::NORTH);
        frente();
        curva(Direction::EAST);
        frente();
        frente();
        frente();
        curva(Direction::SOUTH);
        frente();
        frente();
        curva(Direction::EAST);
        frente();
        curva(Direction::SOUTH);
        frente();
        curva(Direction::WEST);
        frente();
        frente();
        frente();
        frente();
        frente();
        curva(Direction::NORTH);
        frente();
        curva(Direction::EAST);
        frente();
        frente();
        frente();
        // delay(1000);
        // curva(Direction::SOUTH);
        // delay(1000);
        // curva(Direction::EAST);
        delay(1000000);
    } else {
        gladiator->log("Le jeu n'a pas encore commencé");
    }
    delay(300);
}