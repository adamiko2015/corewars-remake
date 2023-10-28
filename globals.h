#include "structs_libraries_and_macros.h"

#ifndef COREWARSINTERPRETER_GLOBALS_H
#define COREWARSINTERPRETER_GLOBALS_H

extern struct Player_Tracker current_player;

extern uint32_t team_count;
extern uint32_t zombie_count;
extern uint32_t survivor_count;

extern uint32_t commands_ran;

extern uint32_t num_of_rounds;
extern uint32_t teams_per_round;

extern Segment* memory;

extern Team* teams;
extern Team* zombies;

extern Team** teams_in_play;

extern uint32_t teams_alive;
extern uint32_t* team_permutation;

#define commands_per_round 0x10000

extern opcode_ptr* opcode_lookup_table;

extern uint32_t rounds_repeated;

#endif //COREWARSINTERPRETER_GLOBALS_H
