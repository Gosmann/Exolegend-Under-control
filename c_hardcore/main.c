
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aap.h"

#define K_C  10.0
#define K_D   0.0

void from_rocks_to_graph(char rocks[14][14] , maze_square maze[14][14] );
void   from_map_to_graph(char map[14][14]   , maze_square maze[14][14] );
void print_known_rocks(char rocks_arg[14][14]);
int get_num_of_rocks(char rocks_arg[14][14]);

void show_graph( maze_square maze[14][14] );
int look_for( maze_square maze[14][14], int_position start, int_position finish, int deep, int deep_w, int max_d);

int do_the_for( maze_square maze[14][14], int_position start, int_position finish, int deep, int deep_w, int max_d){
    int i;
    int dist;

    for(i = 0; i < max_d; i++){

        dist = look_for( maze, start, finish, deep, deep_w, i);

        if(dist != -1){
            break;
        }

    }
    return dist;
}

int look_for( maze_square maze[14][14], int_position start, int_position finish, int deep, int deep_w, int max_d){
    //printf("inside \n");
    
    int i; int ans = -1;
    
    int_position new = {0};

    //printf("[%d] [%d] [%d] [%d] -> [%d]/[%d] \n", start.x, start.y, finish.x, finish.y, deep, deep_w);
    if( start.x == finish.x && start.y == finish.y ){
        
        //printf("correct! [%d] \n", deep);        
        return deep;
    }
    else if( deep < max_d ){
        
        if( maze[start.x][start.y].north != NULL ){            
            new.x = start.x;
            new.y = start.y + 1 ; srand((unsigned int) time(NULL));
            ans = look_for( maze, new, finish, deep+1, deep_w, max_d);   
            //printf("[%d] - ans \n", ans);
        }

        if( maze[start.x][start.y].south != NULL && ans == -1 ){            
            new.x = start.x;
            new.y = start.y - 1;
            ans = look_for( maze, new, finish, deep+1, deep_w, max_d);        
        }

        if( maze[start.x][start.y].east != NULL && ans == -1 ){    
            new.x = start.x + 1;
            new.y = start.y;        
            ans = look_for( maze, new, finish, deep+1, deep_w, max_d);        
        }
        
        if( maze[start.x][start.y].west != NULL && ans == -1 ){            
            new.x = start.x - 1 ;
            new.y = start.y;
            ans = look_for( maze, new, finish, deep+1, deep_w, max_d);        
        }
        
        return ans;

    }

    else{
        return -1;    
    }
        
}

int look_for_rocks( maze_square maze[14][14], int_position start, int deep, char rocks[14][14], int max_d ){

    //printf("inside \n");
    int i; int ans = -1;
    
    int_position new = {0};

    //printf("[%d] [%d] [%d] [%d] -> [%d]/[%d] \n", start.x, start.y, finish.x, finish.y, deep, deep_w);
    if( maze[start.x][start.y].c.value > 0 ){
        rocks[start.x][start.y] = maze[start.x][start.y].c.value;
    }
    if( deep < max_d ){
        
        if( maze[start.x][start.y].north != NULL ){            
            new.x = start.x;
            new.y = start.y + 1 ; srand((unsigned int) time(NULL));
            
            ans = look_for_rocks( maze, new, deep+1, rocks, max_d);   
            //printf("[%d] - ans \n", ans);
        }

        if( maze[start.x][start.y].south != NULL && ans == -1 ){            
            new.x = start.x;
            new.y = start.y - 1;
            ans = look_for_rocks( maze, new, deep+1, rocks, max_d);   
        }

        if( maze[start.x][start.y].east != NULL && ans == -1 ){    
            new.x = start.x + 1;
            new.y = start.y;        
            ans = look_for_rocks( maze, new, deep+1, rocks, max_d);   
        }
        
        if( maze[start.x][start.y].west != NULL && ans == -1 ){            
            new.x = start.x - 1 ;
            new.y = start.y;
            ans = look_for_rocks( maze, new, deep+1, rocks, max_d);          
        }
        
        return ans;
    }

    else{
        return -1;    
    }

}

float calculate_cost(maze_square maze[14][14], int_position start, char rocks[14][14]){
    int i, j;

    float cost = 0;

    cost += ((float)rocks[start.x][start.y]) * -1 * K_C  ;
    //rocks[start.x][start.y] = 0;

    //printf("nooow \n");
    //print_known_rocks( rocks );

    for(i = 0 ; i < 14 ; i++ ){
        for(j = 0 ; j < 14 ; j++){
            
            if(rocks[i][j] != 0){
                //printf("%d %d \n", i, j);

                int_position finish = {0} ;
                finish.x = i;
                finish.y = j;
                int deep = 0; int deep_w = 0;


                cost += (do_the_for( maze, start, finish, deep, deep_w, 12) * K_D ) / ((float)rocks[i][j])  ;
                //printf("%f \n", cost);
            }
        }
    }
    
    return cost;
}

float get_best_cost(maze_square maze[14][14], int_position start, int deep, char rocks[14][14], int max_d, int sec[10], float best, float cost[10]){
    
    //print_known_rocks( rocks );

    //printf("inside \n");
    int i; int ans = -1;
    
    int_position new = {0};

    float now_cost = calculate_cost(maze, start, rocks);
    
    int prev = rocks[start.x][start.y] ;
    rocks[start.x][start.y] = 0;

    //printf("[%d] [%d] -> [%d]/[%d] -> [%8.2f][%8.2f][%8.2f][%8.2f][%8.2f][%8.2f] -> [%8.2f] \n", 
    //    start.x, start.y, deep, 5, cost[0],cost[1],cost[2],cost[3],cost[4],cost[5], now_cost);
    //printf("%f \n", now_cost);
    cost[deep] = now_cost;

    if( deep < max_d ){
        

        if( maze[start.x][start.y].north != NULL ){            
            new.x = start.x;
            new.y = start.y + 1 ; 
            sec[deep] = 'N';
            
            get_best_cost(maze, new, deep+1, rocks, max_d, sec, best, cost);
            //ans = look_for_rocks( maze, new, deep+1, rocks, max_d);   
            //printf("[%d] - ans \n", ans);
        }

        if( maze[start.x][start.y].south != NULL && ans == -1 ){            
            new.x = start.x;
            new.y = start.y - 1;
            sec[deep] = 'S';

            get_best_cost(maze, new, deep+1, rocks, max_d, sec, best, cost);
        }

        if( maze[start.x][start.y].east != NULL && ans == -1 ){    
            new.x = start.x + 1;
            new.y = start.y;   
            sec[deep] = 'E';

            get_best_cost(maze, new, deep+1, rocks, max_d, sec, best, cost);
        }
        
        if( maze[start.x][start.y].west != NULL && ans == -1 ){            
            new.x = start.x - 1 ;
            new.y = start.y;
            sec[deep] = 'W';

            get_best_cost(maze, new, deep+1, rocks, max_d, sec, best, cost);
        }
        
        rocks[start.x][start.y] = prev;
        return (0);
    }

    else{
        //printf("[%8.2f][%8.2f][%8.2f][%8.2f][%8.2f][%8.2f][%8.2f] - ", 
        //    cost[0], cost[1], cost[2], cost[3], cost[4], cost[5], cost[6] + now_cost);
        

        float acc = 0;

        for(i = 0 ; i < 10 ; i++){
            printf("%c ", sec[i]);
            acc += cost[i] ;
        }
        printf("acc[%8.2f] \n", acc);

        rocks[start.x][start.y] = prev;
        return (0);    
    }

    

}




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

    char rocks[14][14] = {
    { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1}, // 0
    { 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1}, // 1
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 2
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // 3
    { 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1}, // 4
    { 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1}, // 5
    { 0, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1}, // 6
    { 1, 1, 1, 1, 2, 1, 2, 2, 1, 2, 1, 1, 1, 0}, // 7
    { 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1}, // 8
    { 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1}, // 9
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // A
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // B
    { 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1}, // C
    { 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1}  // D
    } ; 

    int i, j;
    
    from_map_to_graph( map, maze ) ;
    from_rocks_to_graph( rocks, maze ) ; 

    // maze now has rocks values

    //show_graph( maze );
    
    //look_for(position start, position finish, maze_square maze);
    int_position start = {0};
    start.x = 0;
    start.y = 0;

    //int_position finish = {0};
    //finish.x = 7;
    //finish.y = 7;

    int deep = 0;
    int deep_w = 0;
    int sec[10] = {0};
    //int max_d = 100;
    
    char rocks_arg[14][14] = {0} ;
    look_for_rocks(maze, start, 0, rocks_arg, 8);
    print_known_rocks( rocks_arg );

    int num_rocks = get_num_of_rocks( rocks_arg);
    printf("[%d] \n", num_rocks);
    
    //float calculate_cost(maze_square maze[14][14], int_position start, char rocks[14][14]){    
    /*
    start.x = 6;
    start.y = 4;
    float cost = calculate_cost(maze, start, rocks_arg);
    printf("cost: [%f] \n", cost );
    
    start.x = 5;
    start.y = 3;
    cost = calculate_cost(maze, start, rocks_arg);
    printf("cost: [%f] \n", cost );
    */

    float cost_now[10] = {0};
    float cost = get_best_cost(maze, start, deep, rocks_arg, 5, sec, 0, cost_now);

    //int dist = do_the_for( maze, start, finish, deep, deep_w, max_d);
    //do_the_for( maze, start, finish, deep, deep_w, 8);
    //dist = 0;
    //printf("%d \n", dist);

    /*    
    srand((unsigned int) time(NULL));
    for(i = 0 ; i < 1000 ; i++){
        //printf("rand: %d   ", rand() % 14);
        
        srand(i);
        start.x = (rand() % 14); 
        start.y = (rand() % 14); 
        finish.x = (rand() % 14); 
        finish.y = (rand() % 14); 
        printf("%d %d %d %d \n", start.x, start.y, finish.x, finish.y);

        int dist = do_the_for( maze, start, finish, deep, deep_w, 8);
        printf("[%d] \n", dist);
    }
    */

}   

int get_num_of_rocks(char rocks_arg[14][14]){
    int i, j;
    int count = 0;

    for(i = 0 ; i < 14 ; i++){
        for(j = 0 ; j < 14 ; j++){
            if(rocks_arg[i][j] != 0 )count++;
        }
    }

    return count;
}

void print_known_rocks(char rocks_arg[14][14]){
    int i, j;

    for(i = 0 ; i < 14 ; i++){
        for(j = 0 ; j < 14 ; j++){
            printf("%d ", rocks_arg[i][j]);
        }
        printf(" \n");
    }
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

void from_rocks_to_graph(char rocks[14][14] , maze_square maze[14][14] ){
    int i, j;

    //printf("%d", sizeof(maze));

    for(i = 0 ; i < 14 ; i++){
        for(j = 0 ; j < 14; j++){
            (maze)[i][j].c.value = rocks[i][j];            
        }
    }
}