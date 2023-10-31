#include "structs_libraries_and_macros.h"

uint32_t team_count;
uint32_t zombie_count;
uint32_t survivor_count;

uint32_t commands_ran;

//User input. TODO: change method of input when implementing GUI.
uint32_t teams_per_round;
uint32_t num_of_rounds;

Segment* memory;

Team* teams;
Team* zombies;

Team** teams_in_play; // Includes zombies.

uint32_t teams_alive; // Doesn't include zombies.
uint32_t* team_permutation;

// globals.h also defines commands_per_round

uint8_t* opcode_lookup_table;

uint32_t rounds_repeated = 0;

struct Player_Tracker current_player;

operation_ptr* operators;
op_generalizer* general_ops;