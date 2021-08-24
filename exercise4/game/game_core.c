#include "game_core.h"

#include <linked_list.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

LIST_HEAD(players_list);

struct dice
{
    int throw_result;
    struct list_head dice_list;
};


struct player
{
    int id;
    struct list_head dice_list;
    struct list_head item_list;
};



void append_player_to_game(int player_id){

    player_t* player_to_append = malloc(sizeof (player_t));
    player_to_append->id = player_id;
    INIT_LIST_HEAD(&player_to_append->item_list);
    INIT_LIST_HEAD(&player_to_append->dice_list);

    list_add_tail(&player_to_append->item_list,&players_list);
}

void player_dtor(player_t* player_to_destruct)
{
    assert(player_to_destruct);
    dice_t* dice_it = NULL;
    list_for_each_entry_reverse(dice_it,&player_to_destruct->dice_list,dice_list)
    {
        dice_dtor(dice_it);
    }
    free(player_to_destruct);
}

void dice_dtor(dice_t* todestruct)
{
    assert(todestruct);
    free(todestruct);
}

void player_add_new_dice(player_t* player_to_process, int dice_result)
{
    assert(player_to_process);

    dice_t* new_dice = malloc(sizeof(struct dice));
    INIT_LIST_HEAD(&new_dice->dice_list);
    new_dice->throw_result = dice_result;

    list_add_tail(&new_dice->dice_list,&player_to_process->dice_list);
}

unsigned int player_get_dice_sum(player_t* player_to_process)
{
    int result_value = 0;
    dice_t* dice_it = NULL;
    list_for_each_entry(dice_it,&player_to_process->dice_list,dice_list)
    {
        result_value += dice_it->throw_result;
    }
    return result_value;
}


void print_player_history(player_t* player_to_process)
{
    assert(player_to_process);
    printf("History of player with ID: %d \n", player_to_process->id);

    dice_t* dice_it = NULL;
    list_for_each_entry(dice_it,&player_to_process->dice_list,dice_list)
    {
        printf("[%d]",dice_it->throw_result);
    }
    printf("\n END HISTORY \n");

}

void print_players()
{
    player_t* player_it = NULL;
    list_for_each_entry(player_it,&players_list,item_list)
    {
        printf("%d \n",player_it->id);
    }
}

void cleanup_players()
{
    player_t* player_it = NULL;
    list_for_each_entry_reverse(player_it,&players_list,item_list)
    {
        if(player_it)
            player_dtor(player_it);
    }
}

void print_scoretable()
{
    player_t* player_it = NULL;
    list_for_each_entry(player_it,&players_list,item_list)
    {
        int player_result = player_get_dice_sum(player_it);
        printf("Player ID: %d, result:%d \n",player_it->id, player_result);
    }
}

void print_players_history()
{
    player_t* player_it = NULL;
    list_for_each_entry(player_it,&players_list,item_list)
    {
        print_player_history(player_it);
    }
}


void append_players(int num_players)
{
    for(int i =0; i< num_players; ++i){
        append_player_to_game(i+ 1);
    }
}


static void init_random_engine()
{
    srand(time(NULL));
}

static int get_random_engine_value()
{
    return 1 + (rand() % 6);
}
static void play_game_iteration(int num_dices)
{
    player_t* player_it = NULL;
    list_for_each_entry(player_it,&players_list,item_list)
    {
        for(int i = 0; i< num_dices; ++i)
        {
            player_add_new_dice(player_it,get_random_engine_value());
        }
    }
}

static void end_game()
{
    print_scoretable();
    print_players_history();
    cleanup_players();
}

void play_game_iterations(int num_game_iterations, int num_dices)
{
    printf("game setup: iterations:%d, num_dices: %d\n",num_game_iterations,num_dices);
    for(int i = 0; i< num_game_iterations;++i)
    {
        play_game_iteration(num_dices);
    }
    end_game();
}

