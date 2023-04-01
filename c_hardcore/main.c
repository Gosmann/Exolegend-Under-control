
#include <stdio.h>

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



int main(){
    
    maze_square maze[14][14] = {0} ;

    char map[14][14] = {
    // 0   1   2   3   4   5   6   7   8   9  10  11  12  13 
    { 11, 10, 03, 10, 03, 14, 03, 10, 07, 10, 03, 10, 03, 11}, // 0
    { 08, 05, 08, 05, 08, 03, 09, 09, 10, 01, 12, 01, 12, 01}, // 1
    { 08, 03, 12, 02, 01, 12, 01, 08, 05, 08, 02, 05, 10, 01}, // 2
    { 13, 08, 03, 09, 09, 10, 04, 04, 03, 09, 09, 10, 01, 13}, // 3
    { 10, 05, 08, 01, 13, 08, 06, 06, 01, 13, 08, 01, 12, 03}, // 4
    { 08, 02, 05, 12, 06, 01, 11, 11, 08, 06, 05, 12, 02, 01}, // 5
    { 13, 08, 06, 03, 10, 04, 00, 00, 04, 03, 10, 06, 01, 12}, // 6
    { 11, 08, 06, 05, 12, 02, 00, 00, 02, 05, 12, 06, 01, 11}, // 7
    { 08, 04, 03, 10, 06, 01, 13, 13, 08, 06, 03, 10, 04, 01}, // 8
    { 12, 03, 08, 01, 11, 08, 06, 06, 01, 11, 08, 01, 10, 05}, // 9
    { 11, 08, 05, 09, 09, 12, 02, 02, 05, 09, 09, 12, 01, 11}, // A
    { 08, 05, 10, 04, 01, 10, 01, 08, 03, 08, 04, 03, 12, 01}, // B
    { 08, 03, 08, 03, 08, 05, 09, 09, 12, 01, 10, 01, 10, 01}, // C
    { 13, 12, 05, 12, 05, 14, 05, 12, 07, 12, 05, 12, 05, 13} 
    } ;

    int i, j;

    for(i = 0 ; i < 14 ; i++){
        for(j = 0 ; j < 14: j++){

            int wall = map[i][j];

            if(wall == 0){

            }
            else if(wall == 1){

            }
            else if(wall == 2){
                
            }
            else if(wall == 3){
                
            }
            else if(wall == 4){
                
            }
            else if(wall == 5){
                
            }
            else if(wall == 6){
                
            }
            else if(wall == 7){
                
            }
            else if(wall == 8){
                
            }
            else if(wall == 9){
                
            }
            else if(wall == 10){
                
            }
            else if(wall == 11){
                
            }


        }
    }


}
