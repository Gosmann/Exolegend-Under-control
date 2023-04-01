#ifndef HEADER
#define HEADER

typedef struct position{

    double x;
    double y;
    double a;

} position ;

typedef struct coin{
    
    char value;
    position p;

} coin ;

typedef struct robot{
    
    position p;
    

} robot ;


typedef struct maze_square{
    
    char i;
    char j;

    struct maze_square* north;
    struct maze_square* south;
    struct maze_square* east;
    struct maze_square* west;

} maze_square ;

#endif