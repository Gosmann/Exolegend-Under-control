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

void curva(Direction sentido, double baseSpeed);
Direction directionFromAngle(double angle);
double angleFromDirection(int sentido, float currentAngle);
double desired_pos(int i);

double motorPID_LEFT(double setpoint){
    double Kp = 2.244;

    double measure = abs(gladiator->robot->getData().vl)/0.8;
    double erro = setpoint-measure; 
    double resultado = Kp*erro;
    return resultado;
}

double motorPID_RIGHT(double setpoint){
    double Kp = 2.244;

    double measure = abs(gladiator->robot->getData().vr)/0.8;
    double erro = setpoint-measure; 
    double resultado = Kp*erro;
    return resultado;
}

void setWheelVelocity(WheelAxis ax, float vel){
    if(gladiator->robot->getData().speedLimit<0.5){
        vel = vel*10;
    }
    gladiator->control->setWheelSpeed(ax, vel);
    // if(ax==WheelAxis::RIGHT){
    //     gladiator->control->setWheelSpeed(ax, motorPID_RIGHT(vel));
    // }else{
    //     gladiator->control->setWheelSpeed(ax, motorPID_LEFT(vel));
    // }
}

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
    //setWheelVelocity(WheelAxis::LEFT, -0.05);
    //setWheelVelocity(WheelAxis::RIGHT, 0.05);

}

void noventa_graus(bool antiHorario){
    RobotData data = gladiator->robot->getData();
    Position pos = data.position;
    MazeSquare square = gladiator->maze->getNearestSquare();

    visitedPlaces.insert(visitedPlaces.end(),square.i*10+square.j);

    double ang = pos.a;
    if(antiHorario){
        setWheelVelocity(WheelAxis::LEFT, -0.05);
        setWheelVelocity(WheelAxis::RIGHT, 0.05);
        currentDirection+=1;
    }else{
        setWheelVelocity(WheelAxis::LEFT, 0.05);
        setWheelVelocity(WheelAxis::RIGHT, -0.05);
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
    setWheelVelocity(WheelAxis::LEFT, 0);
    setWheelVelocity(WheelAxis::RIGHT, 0);
    delay(1000);
}

void frente_older(){

    RobotData data = gladiator->robot->getData();
    Position pos = data.position;
    double ang = pos.a;

    ang = angleFromDirection(currentDirection,ang);

    MazeSquare square = gladiator->maze->getNearestSquare();
    MazeSquare current_square = square;
    setWheelVelocity(WheelAxis::LEFT, 0.2);
    setWheelVelocity(WheelAxis::RIGHT, 0.2);

    double erro = 0;
    double Kp = 0.1;
    double current_angle = gladiator->robot->getData().position.a;
    while(current_square.i==square.i&&current_square.j==square.j){
        current_angle = gladiator->robot->getData().position.a;
        ang = angleFromDirection(currentDirection,current_angle);
        erro = current_angle-ang;

        setWheelVelocity(WheelAxis::LEFT, 0.2+Kp*erro);
        setWheelVelocity(WheelAxis::RIGHT, 0.2-Kp*erro);

        current_square = gladiator->maze->getNearestSquare();
    }
    delay(200);
    visitedPlaces.insert(visitedPlaces.end(),current_square.i*10+current_square.j);
    setWheelVelocity(WheelAxis::LEFT, 0);
    setWheelVelocity(WheelAxis::RIGHT, 0);
}

void frente_old(){
    RobotData data = gladiator->robot->getData();
    Position pos = data.position;
    double ang = pos.a;

    ang = angleFromDirection(currentDirection,ang);

    MazeSquare square = gladiator->maze->getNearestSquare();
    MazeSquare current_square = square;
    setWheelVelocity(WheelAxis::LEFT, 0.2);
    setWheelVelocity(WheelAxis::RIGHT, 0.2);

    double erro = 0;
    double Kp = 0.1;
    double current_angle = gladiator->robot->getData().position.a;
    while(current_square.i==square.i&&current_square.j==square.j){
        current_angle = gladiator->robot->getData().position.a;
        ang = angleFromDirection(currentDirection,current_angle);
        erro = current_angle-ang;

        setWheelVelocity(WheelAxis::LEFT, 0.2+Kp*erro);
        setWheelVelocity(WheelAxis::RIGHT, 0.2-Kp*erro);

        current_square = gladiator->maze->getNearestSquare();
    }
    
    Kp = 2;
    if(currentDirection==Direction::NORTH){
        pos=gladiator->robot->getData().position;
        while(desired_pos(current_square.j)-pos.y>0.01){
            pos = gladiator->robot->getData().position;
            erro = desired_pos(current_square.j)-pos.y;
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    if(currentDirection==Direction::SOUTH){
        pos=gladiator->robot->getData().position;
        while(pos.y-desired_pos(current_square.j)>0.01){
            pos = gladiator->robot->getData().position;
            erro = pos.y-desired_pos(current_square.j);
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    if(currentDirection==Direction::EAST){
        pos=gladiator->robot->getData().position;
        while(desired_pos(current_square.i)-pos.x>0.01){
            pos = gladiator->robot->getData().position;
            erro = desired_pos(current_square.i)-pos.x;
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    if(currentDirection==Direction::WEST){
        pos=gladiator->robot->getData().position;
        while(pos.x-desired_pos(current_square.i)>0.01){
            pos = gladiator->robot->getData().position;
            erro = pos.x-desired_pos(current_square.i);
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    setWheelVelocity(WheelAxis::LEFT, 0);
    setWheelVelocity(WheelAxis::RIGHT, 0);
}

double integral_PI_frente_LEFT = 0;
double last_error_LEFT = -20;
double correcaoPI_frenteLEFT(double erro){
    double Kp = 0.5;
    double Ki = 0.000000004;
    double Kd = 0.3;
    double correcaoKD = 0;
    if(last_error_LEFT==-20){
        last_error_LEFT = erro;
    }else{
        correcaoKD = (erro-last_error_LEFT)*Kd;
        last_error_LEFT = erro;
    }
    integral_PI_frente_LEFT+=erro;
    if(integral_PI_frente_LEFT*Ki>0.5){
        integral_PI_frente_LEFT=0;
    }
    double correcao = Kp*erro+Ki*integral_PI_frente_LEFT+correcaoKD;
    return correcao;
}

double integral_PI_frente_RIGHT = 0;
double last_error_RIGHT = -20;
double correcaoPI_frenteRIGHT(double erro){
    double Kp = 0.5;
    double Ki = 0.000000004;
    double Kd = 0.3;
    double correcaoKD = 0;
    if(last_error_RIGHT==-20){
        last_error_RIGHT = erro;
    }else{
        correcaoKD = (erro-last_error_RIGHT)*Kd;
        last_error_RIGHT = erro;
    }
    integral_PI_frente_RIGHT+=erro;
    if(integral_PI_frente_RIGHT*Ki>0.5){
        integral_PI_frente_RIGHT=0;
    }
    double correcao = Kp*erro+Ki*integral_PI_frente_RIGHT+correcaoKD;
    return correcao;
}

void frente(int squares = 1, double baseSpeed = 0.4){
    //base = 0.4, KP 0.05
    RobotData data = gladiator->robot->getData();
    Position pos = data.position;
    MazeSquare square = gladiator->maze->getNearestSquare();
    MazeSquare current_square = square;

    double set_y = pos.y;
    double set_x = pos.x;

    setWheelVelocity(WheelAxis::LEFT, baseSpeed);
    setWheelVelocity(WheelAxis::RIGHT, baseSpeed);

    double erro = 0;
    double Kp = 0.05;
    double tol = 0.01;
    double tol_derro = 0.005;

    int num_of_squares = 0;

    if(currentDirection==Direction::EAST){
        double current_y = gladiator->robot->getData().position.y;
        set_y = desired_pos(current_square.j);
        while(num_of_squares<squares){
            while(current_square.i==square.i&&current_square.j==square.j){
                current_y = gladiator->robot->getData().position.y;
                erro = set_y-current_y;

                setWheelVelocity(WheelAxis::LEFT, baseSpeed-correcaoPI_frenteLEFT(erro));
                setWheelVelocity(WheelAxis::RIGHT, baseSpeed+correcaoPI_frenteRIGHT(erro));

                current_square = gladiator->maze->getNearestSquare();
            }
            num_of_squares++;
            square = current_square;     
        }
    }

    if(currentDirection==Direction::WEST){
        double current_y = gladiator->robot->getData().position.y;
        set_y = desired_pos(current_square.j);
        while(num_of_squares<squares){
            while(current_square.i==square.i&&current_square.j==square.j){
                current_y = gladiator->robot->getData().position.y;
                erro = set_y-current_y;

                setWheelVelocity(WheelAxis::LEFT, baseSpeed+correcaoPI_frenteLEFT(erro));
                setWheelVelocity(WheelAxis::RIGHT, baseSpeed-correcaoPI_frenteRIGHT(erro));

                current_square = gladiator->maze->getNearestSquare();
            }
            num_of_squares++;
            square = current_square;
        }
    }

    if(currentDirection==Direction::NORTH){
        double current_x = gladiator->robot->getData().position.x;
        set_x = desired_pos(current_square.i);
        while(num_of_squares<squares){
            while(current_square.i==square.i&&current_square.j==square.j){
                current_x = gladiator->robot->getData().position.x;
                erro = set_x-current_x;

                setWheelVelocity(WheelAxis::LEFT, baseSpeed+correcaoPI_frenteLEFT(erro));
                setWheelVelocity(WheelAxis::RIGHT, baseSpeed-correcaoPI_frenteRIGHT(erro));

                current_square = gladiator->maze->getNearestSquare();
            }
            num_of_squares++;
            square = current_square;
        }
    }

    if(currentDirection==Direction::SOUTH){
        double current_x = gladiator->robot->getData().position.x;
        set_x = desired_pos(current_square.i);
        while(num_of_squares<squares){
            while(current_square.i==square.i&&current_square.j==square.j){
                current_x = gladiator->robot->getData().position.x;
                erro = set_x-current_x;

                setWheelVelocity(WheelAxis::LEFT, baseSpeed-correcaoPI_frenteLEFT(erro));
                setWheelVelocity(WheelAxis::RIGHT, baseSpeed+correcaoPI_frenteRIGHT(erro));

                current_square = gladiator->maze->getNearestSquare();
            }
            num_of_squares++;
            square = current_square;
        }
        
    }
    
    Kp = 1;
    if(currentDirection==Direction::NORTH){
        pos=gladiator->robot->getData().position;
        double derro=0;
        double ultimo_erro=0;
        while(abs(desired_pos(current_square.j)-pos.y)>tol||abs(derro)){
            pos = gladiator->robot->getData().position;
            erro = desired_pos(current_square.j)-pos.y;
            derro = erro-ultimo_erro;
            ultimo_erro = erro;
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    if(currentDirection==Direction::SOUTH){
        pos=gladiator->robot->getData().position;
        double derro=0;
        double ultimo_erro=0;
        while(abs(pos.y-desired_pos(current_square.j))>tol||abs(derro)){
            pos = gladiator->robot->getData().position;
            erro = pos.y-desired_pos(current_square.j);
            derro = erro-ultimo_erro;
            ultimo_erro = erro;
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    if(currentDirection==Direction::EAST){
        pos=gladiator->robot->getData().position;
        double derro=0;
        double ultimo_erro=0;
        while(abs(desired_pos(current_square.i)-pos.x)>tol||abs(derro)){
            pos = gladiator->robot->getData().position;
            erro = desired_pos(current_square.i)-pos.x;
            derro = erro-ultimo_erro;
            ultimo_erro = erro;
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    if(currentDirection==Direction::WEST){
        pos=gladiator->robot->getData().position;
        double derro=0;
        double ultimo_erro=0;
        while(abs(pos.x-desired_pos(current_square.i))>tol||abs(derro)){
            pos = gladiator->robot->getData().position;
            erro = pos.x-desired_pos(current_square.i);
            derro = erro-ultimo_erro;
            ultimo_erro = erro;
            setWheelVelocity(WheelAxis::LEFT, Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);
        }
    }

    setWheelVelocity(WheelAxis::LEFT, 0);
    setWheelVelocity(WheelAxis::RIGHT, 0);
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

bool shouldUseAntihorario(int sentido){
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

void curva(int sentido, double baseSpeed = 0.2){
    RobotData data = gladiator->robot->getData();
    double Kp = 0.4; //0.2vel = 0.4KP
    double tol = 0.005;
    double tol_derro = 0.001;
    //5% de velocidade, Kp=0.15
    if(currentDirection==sentido){
        return;
    }

    bool antiHorario = shouldUseAntihorario(sentido);

    if(antiHorario){
        setWheelVelocity(WheelAxis::LEFT, -baseSpeed);
        setWheelVelocity(WheelAxis::RIGHT, baseSpeed);
    }else{
        setWheelVelocity(WheelAxis::LEFT, baseSpeed);
        setWheelVelocity(WheelAxis::RIGHT, -baseSpeed);
    }

    double currentAngle = gladiator->robot->getData().position.a;

    if(sentido==Direction::NORTH){
        while(!(currentAngle>M_PI/4&&currentAngle<3*M_PI/4)){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido)-currentAngle;
        double derro = 0;
        double ultimo_erro = -20;
        while(abs(erro)>tol||abs(derro)>tol_derro){
            erro = angleFromDirection(sentido)-currentAngle;
            derro = ultimo_erro-erro;
            ultimo_erro = erro;
            currentAngle = gladiator->robot->getData().position.a;
            setWheelVelocity(WheelAxis::LEFT, -Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    if(sentido==Direction::SOUTH){
        while(!(currentAngle<-M_PI/4&&currentAngle>-3*M_PI/4)){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido)-currentAngle;
        double derro = 0;
        double ultimo_erro = -20;
        while(abs(erro)>tol||abs(derro)>tol_derro){
            erro = angleFromDirection(sentido)-currentAngle;
            derro = ultimo_erro-erro;
            ultimo_erro = erro;
            currentAngle = gladiator->robot->getData().position.a;
            setWheelVelocity(WheelAxis::LEFT, -Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    if(sentido==Direction::EAST){
        while(!(currentAngle<M_PI/4&&currentAngle>-M_PI/4)){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido)-currentAngle;
        double derro = 0;
        double ultimo_erro = -20;
        while(abs(erro)>tol||abs(derro)>tol_derro){
            erro = angleFromDirection(sentido)-currentAngle;
            derro = ultimo_erro-erro;
            ultimo_erro = erro;
            currentAngle = gladiator->robot->getData().position.a;
            setWheelVelocity(WheelAxis::LEFT, -Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    if(sentido==Direction::WEST){
        while(!((currentAngle>2.35619&&currentAngle<3.14159)||(currentAngle>-3.14159&&currentAngle<-2.35619))){
            currentAngle = gladiator->robot->getData().position.a;
        }
        double erro = angleFromDirection(sentido,currentAngle)-currentAngle;
        double derro = 0;
        double ultimo_erro = -20;
        while(abs(erro)>tol){
            erro = angleFromDirection(sentido,currentAngle)-currentAngle;
            derro = ultimo_erro-erro;
            ultimo_erro = erro;
            currentAngle = gladiator->robot->getData().position.a;
            setWheelVelocity(WheelAxis::LEFT, -Kp*erro);
            setWheelVelocity(WheelAxis::RIGHT, Kp*erro);   
        }
    }

    setWheelVelocity(WheelAxis::LEFT, 0);
    setWheelVelocity(WheelAxis::RIGHT, 0);   
    currentDirection = sentido;
}

double desired_pos(int i){
    return i*gladiator->maze->getSquareSize()+(gladiator->maze->getSquareSize()/2);
}

void standard_function(){
    curva(Direction::NORTH);
    frente();
    curva(Direction::EAST);
    frente(2);
    curva(Direction::NORTH);
    frente();
    curva(Direction::EAST);
    frente();
    curva(Direction::SOUTH);
    frente();
    curva(Direction::EAST);
    frente(2);
    curva(Direction::NORTH);
    frente();
    curva(Direction::EAST);
    frente(3);
    curva(Direction::SOUTH);
    frente(2);
    curva(Direction::EAST);
    frente();
    curva(Direction::SOUTH);
    frente();
    curva(Direction::WEST);
    frente(1);
    curva(Direction::SOUTH);
    frente(2);
    curva(Direction::WEST);
    frente(3);
    curva(Direction::NORTH);
    frente(2);
    curva(Direction::WEST);
    frente();
    curva(Direction::NORTH);
    frente();
    curva(Direction::EAST);
    frente(3);
    curva(Direction::SOUTH);
    frente();
    curva(Direction::WEST);
    frente();
}

void farmar_pontos(){
    frente(7,0.5);
    curva(Direction::SOUTH);
    frente(1,0.5);
    curva(Direction::WEST);
    frente(6,0.5);
    curva(Direction::SOUTH);
    frente(1,0.5);
    curva(Direction::EAST);
    frente(6,0.5);
    curva(Direction::SOUTH);
    frente(1,0.5);
    curva(Direction::WEST);
    frente(3,0.5);
    curva(Direction::SOUTH);
    frente();
    curva(Direction::WEST);
    frente();
}

void follow_directions(int *directions, int len){
    int last_direction=-1;
    for(int i=0;i<len;i++){
        if(directions[i]!=last_direction){
            curva(directions[i]);
        }
        last_direction=currentDirection;
        int counter = 0;
        for(int j=0;(j+i)<len;j++){
            if(directions[i+j]==directions[i]){
                counter+=1;
            }else{
                j=len;
            }
        }
        i+=(counter-1);
        frente(counter);
    }
}

void loop() {
    if(gladiator->game->isStarted()) { //tester si un match à déjà commencer
        //code de votre stratégie
        gladiator->log("Le jeu a commencé");
        // setWheelVelocity(WheelAxis::RIGHT,-0.05);
        // setWheelVelocity(WheelAxis::LEFT,-0.05);
        // delay(800);
        // setWheelVelocity(WheelAxis::RIGHT,0);
        // setWheelVelocity(WheelAxis::LEFT,0);
        
        int dire[] = {1,0,0,1,0,3,0,0,1,0,0,0,3,3,0,3,2,3,3,2,2,2,1,1,2,1,0,0,0,3,2,2,3,2,2,3,3,3,0,1,1,3,0,0,0,0,0,1,1,0,3,3};
        follow_directions(&dire[0],52);
        // standard_function();
        // farmar_pontos();

        // curva(Direction::NORTH);
        // frente();
        // curva(Direction::EAST);
        // frente(2);
        // curva(Direction::WEST);
        // setWheelVelocity(WheelAxis::RIGHT,0.05);
        // setWheelVelocity(WheelAxis::LEFT,0.05);

        delay(100000);
    } else {
        gladiator->log("Le jeu n'a pas encore commencé");
    }
    delay(300);
}