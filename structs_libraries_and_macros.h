#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdbool.h>
#include <inttypes.h>
#include <dirent.h>
#include <errno.h>

#ifndef COREWARSINTERPRETER_INIT_GLOBALS_H
#define COREWARSINTERPRETER_INIT_GLOBALS_H

#define total_team_count (teams_per_round+zombie_count)
#define exit_angrily {fprintf(stderr, "Error in file " __FILE__ " on line: %d. %s.\r\n", __LINE__, strerror(errno)); \
                        exit(1);}
#define debug_print_statement     printf("Hit file " __FILE__ " line %d\r\n", __LINE__);

typedef struct  {
    bool initialized;
    uint16_t stack_id;
    uint16_t AX, BX, CX, DX, SI, DI, BP, SP, CS, DS, SS, ES, IP, Energy, Flags; // Registers
    uint8_t INT86h_used, INT87h_used;
    char* code;
    uint16_t code_size;
} Survivor;

typedef struct {
    Survivor survivors[2];
    int8_t living_survivors[2];
    uint16_t shared_memory_id;
    uint16_t team_id; // ID of team in teams array
    char* name;
    double points;
    bool is_zombie;
} Team;

typedef struct {
    char values[0x10000];
} Segment;

struct Player_Tracker {
    int32_t team_id;
    int32_t survivor_position;
};

typedef bool (*opcode_ptr)(Survivor*);

#endif //COREWARSINTERPRETER_INIT_GLOBALS_H
