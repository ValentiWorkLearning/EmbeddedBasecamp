#include <stdio.h>
#include "linked_list/linked_list.h"
#include "game_core.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int args_count = 5; // -n num of cubes for player -p players_count


int main(int argc, char** argv){

    if(argc < args_count){
        printf("Invalid argument count!\n Use -n for the number of cubes and -p for defining the number of players");
        return -1;
    }
    int c;
    int num_players = 0;
    int num_dices = 0;
    int num_game_iterations = 0;

    while ((c = getopt (argc, argv, "n:p:v:")) != -1)
       switch (c)
         {
         case 'n':
           num_dices = atoi(optarg);
           break;
         case 'p':
           num_players = atoi(optarg);
           break;
         case 'v':
           num_game_iterations = atoi(optarg);
           break;
         case '?':
           if (optopt == 'n')
             fprintf (stderr, "opion n specifies num of dices \n");
           else if (optopt == 'p')
             fprintf (stderr, "option p requires a number of players \n");
           else
             fprintf (stderr,
                      "Unknown option character `\\x%x'.\n",
                      optopt);
           return 1;
         default:
           abort ();
         }

    append_players(num_players);
    play_game_iterations(num_game_iterations,num_dices);
    return 0;
}
