#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include "linked_list.h"

const int args_count = 5; // -n num of cubes for player -p players_count
const int max_cubes = 6;


#define MAX_PLAYER_NAME_LEN 255

typedef struct dice_score_record
{
    time_t timestamp;
    int points;
    struct list_head list;
}dice_record_t;

typedef struct dice_player
{
    char player_name[MAX_PLAYER_NAME_LEN];
    dice_record_t dice_records;

    struct list_head list;
}dice_player_t;

void assign_player_name(dice_player_t* player,const char* const player_name){
    assert(player);
    if (!player)
        return;
    const int new_name_length = strlen(player_name);
    if (new_name_length > MAX_PLAYER_NAME_LEN)
        return;
    strcpy(player->player_name, player_name);
}

typedef void(*list_callback_t)(dice_player_t*);

void for_each_in_player_list(const dice_player_t* player_list, list_callback_t callback){
    struct list_head* p;
    dice_player_t* player_it;

    list_for_each(p, &player_list->list)
    {
        player_it = list_entry(p, dice_player_t, list);
        callback(player_it);
    }
}

dice_player_t* create_player(const char* const player_name)
{
    dice_player_t * player = malloc(sizeof(dice_player_t));
    
    if (!player)
        return NULL;

    INIT_LIST_HEAD(&player->dice_records.list);

    player->dice_records.points = 0;
    player->dice_records.timestamp = 0;

    assign_player_name(player, player_name);

    return player;
}

void player_add_result(const dice_player_t* player, int dice_result)
{
    dice_record_t* new_record = malloc(sizeof(dice_record_t));
    if (!new_record)
        return;

    new_record->points = dice_result;
    list_add_tail(new_record, &player->dice_records.list);

}

void print_player_name(const dice_player_t* player){
    printf(player->player_name);
    printf("\n");
}

void print_player_total_score(const dice_player_t* player) {
    printf("Score for player: ");
    print_player_name(player);
    
    int accumulated_result = 0;
    dice_record_t* it_dice_record = NULL;

    printf("\n");
}


void play_game_iteration(const dice_player_t* player)
{
    int new_result = get_random_dice_value();
    player_add_result(player, new_result);
}

void initialize_random_engine()
{
    srand(time(NULL));
}

int get_random_dice_value()
{
    return 1 + rand() % 6;
}

static LIST_HEAD(player_list);

int main(int argc, char** argv){

    const int players_count = 2;
    const int iterations_count = 10;


    for (int i = 0; i < players_count; ++i) {
        dice_player_t* player1 = create_player("player");
        player_add_result(player1, 33);
        list_add_tail(&player1->list, &player_list);
    }

    struct list_head* temp = NULL;
    temp = &player_list;
    while (temp != player_list.prev)
    {
        temp = temp->next;
        dice_player_t* dice_player = container_of(temp, dice_player_t, list);

        struct list_head* dice_record_it = NULL;
        dice_record_it = &dice_player->dice_records.list;
        while (dice_record_it != dice_player->dice_records.list.prev)
        {
            dice_record_it = dice_record_it->next;
            dice_record_t* p_record = container_of(dice_record_it, dice_record_t, list);
            int points = p_record->points;
            int a = 42;
        }


        printf("%s \n", dice_player->player_name);
    }


    /*if(argc < args_count){
        printf("Invalid argument count!\n Use -n for the number of cubes and -p for defining the number of players");
        return -1;
    }*/

    // parse options
    // create players
    // assign to them cubes
    return 0;
}