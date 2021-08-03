#include <stdio.h>

const int args_count = 5; // -n num of cubes for player -p players_count
const int max_cubes = 6;


int main(int argc, char** argv){

    if(argc < args_count){
        printf("Invalid argument count!\n Use -n for the number of cubes and -p for defining the number of players");
        return -1;
    }

    // parse options
    // create players
    // assign to them cubes

    printf("Hello World \n");
    return 0;
}