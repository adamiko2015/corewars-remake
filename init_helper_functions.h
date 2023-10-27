#include "structs_libraries_and_macros.h"
#include "globals.h"

#ifndef COREWARSINTERPRETER_INIT_HELPER_FUNCTIONS_H
#define COREWARSINTERPRETER_INIT_HELPER_FUNCTIONS_H

void init_survivor_and_team_count(void);
void get_code(Survivor survivor[static 1], char* filename, char* dirname);
void add_survivor_to_team(Team team[static 1], Survivor survivor);
void allocate_memory(void);
void init_opcode_table(void);

#endif //COREWARSINTERPRETER_INIT_HELPER_FUNCTIONS_H
