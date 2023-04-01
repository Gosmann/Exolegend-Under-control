
#include <stdio.h>
#include "aap.h"

void from_map_to_graph(char map[14][14] , maze_square maze[14][14] );
void show_graph( maze_square maze[14][14] );

int main(){
    
    maze_square maze[14][14] = {0} ;
    //maze_square * init[14][14] = &(maze[0][0]);

    char map[14][14] = {
    // 0   1   2   3   4   5   6   7   8   9  10  11  12  13 
    { 11, 10, 03, 10, 03, 14, 03, 10, 07, 10, 03, 10, 03, 11}, // 0
    {  8, 05,  8, 05,  8, 03,  9,  9, 10, 01, 12, 01, 12, 01}, // 1
    {  8, 03, 12, 02, 01, 12, 01,  8, 05,  8, 02, 05, 10, 01}, // 2
    { 13,  8, 03,  9,  9, 10, 04, 04, 03,  9,  9, 10, 01, 13}, // 3
    { 10, 05,  8, 01, 13,  8, 06, 06, 01, 13,  8, 01, 12, 03}, // 4
    {  8, 02, 05, 12, 06, 01, 11, 11,  8, 06, 05, 12, 02, 01}, // 5
    { 13,  8, 06, 03, 10, 04, 00, 00, 04, 03, 10, 06, 01, 12}, // 6
    { 11,  8, 06, 05, 12, 02, 00, 00, 02, 05, 12, 06, 01, 11}, // 7
    {  8, 04, 03, 10, 06, 01, 13, 13,  8, 06, 03, 10, 04, 01}, // 8
    { 12, 03,  8, 01, 11,  8, 06, 06, 01, 11,  8, 01, 10, 05}, // 9
    { 11,  8, 05,  9,  9, 12, 02, 02, 05,  9,  9, 12, 01, 11}, // A
    {  8, 05, 10, 04, 01, 10, 01,  8, 03,  8, 04, 03, 12, 01}, // B
    {  8, 03,  8, 03,  8, 05,  9,  9, 12, 01, 10, 01, 10, 01}, // C
    { 13, 12, 05, 12, 05, 14, 05, 12, 07, 12, 05, 12, 05, 13} 
    } ;

    int i, j;
    
    from_map_to_graph( map, maze ) ;
    
    printf("FINISHED! \n");

    show_graph( maze );
    
    //look_for(position start, position finish, maze_square maze);
    position start = {0};
    start.x = 0;
    start.y = 7;

    position finish = {0};
    finish.x = 0;
    finish.y = 8;

    char looked[14][14] = {0} ;

    int deep = 0;
    


}   


void show_graph( maze_square maze[14][14] ){
    int i, j;

    for(i = 0; i < 14 ; i++){
        for(j = 0; j < 14; j++){
            maze_square * init = &  (maze)[i][j] ; 
            printf("[%p] i:[%2d] j:[%2d] N:[%p] S:[%p] E:[%p] W:[%p] \n", init,
                init[0].i, init[0].j, init[0].north, init[0].south, init[0].east, init[0].west );
        
        }
    }
}

void from_map_to_graph(char map[14][14] , maze_square maze[14][14] ){
    int i, j;

    //printf("%d", sizeof(maze));

    for(i = 0 ; i < 14 ; i++){
        for(j = 0 ; j < 14; j++){

            char wall = map[j][i];

            (maze)[i][j].i = i;
            (maze)[i][j].j = j;
                               
            if(wall == 0){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            
            else if(wall == 1){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            else if(wall == 2){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            else if(wall == 3){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            else if(wall == 4){
                maze[i][j].north = NULL ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            else if(wall == 5){
                maze[i][j].north = NULL ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            else if(wall == 6){
                maze[i][j].north = NULL ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            else if(wall == 7){
                maze[i][j].north = NULL ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = & maze[i-1][j] ;
            }
            else if(wall == 8){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = NULL ;
            }
            else if(wall == 9){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = NULL ;
            }
            else if(wall == 10){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = NULL ;
            }
            else if(wall == 11){
                maze[i][j].north = & maze[i][j+1] ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = NULL ;
            }
            else if(wall == 12){
                maze[i][j].north = NULL ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = NULL ;
            }
            else if(wall == 13){
                maze[i][j].north = NULL ; 
                maze[i][j].south = & maze[i][j-1] ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = NULL ;
            }
            else if(wall == 14){
                maze[i][j].north = NULL ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = & maze[i+1][j] ;
                maze[i][j].west  = NULL ;
            }
            else if(wall == 15){
                maze[i][j].north = NULL ; 
                maze[i][j].south = NULL ;
                maze[i][j].east  = NULL ;
                maze[i][j].west  = NULL ;
            }
            
            
            
        }
    }
}