#ifndef GAME_CORE_H
#define GAME_CORE_H
#include <stdint.h>

typedef struct player player_t;
typedef struct dice dice_t;

void append_player_to_game(int player_id);

void player_dtor(player_t* player_to_destruct);
void print_player_history(player_t* player_to_process);

void cleanup_players();

void print_players();
void print_scoretable();
void print_players_history();

void player_add_new_dice(player_t* player_to_process, int dice_result);

unsigned int player_get_dice_sum(player_t* player_to_process);


void dice_dtor(dice_t* todestruct);

void append_players(int num_players);

void play_game_iterations(int num_game_iterations, int num_dices);

#endif
