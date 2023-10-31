#include "globals.h"
#include "structs_libraries_and_macros.h"

bool choose_teams(void) {
    if (rounds_repeated >= num_of_rounds) return false;

    int carry = 1;
    for (int i=teams_per_round-1; i>=0 && carry; i--) {
        if (team_permutation[i] < team_permutation[i+1]-1) {
            team_permutation[i]++;
            carry = 0;

            for (uint32_t j=i+1; j<teams_per_round; j++) team_permutation[j] = team_permutation[j-1] + 1;
        }
    }

    if (carry) {
        rounds_repeated++;
        for (uint32_t i=0; i<teams_per_round; i++) team_permutation[i] = i;
    }

    for (uint32_t i=0; i<teams_per_round; i++) {
        teams_in_play[i] = &teams[team_permutation[i]];
    }

    for (uint32_t i=0; i<zombie_count; i++) {
        teams_in_play[i+teams_per_round] = &zombies[i];
    }

    teams_alive = teams_per_round;


    return true;
}

void resurrect_players(void) {
    for(Team** team = teams_in_play; team < teams_in_play + total_team_count; team++) {
        (*team)->living_survivors[0] = 1;
        (*team)->survivors[0].initialized = true;
        (*team)->survivors[0].registers = (Registers) {
            .SP = sizeof(Segment)-1,
            .SS = (*team)->survivors[0].stack_id*0x1000,
            .ES = (*team)->shared_memory_id*0x1000,
        };

        if ((*team)->survivors[1].initialized) {
            (*team)->living_survivors[1] = 1;
            (*team)->survivors[1].initialized = true;
            (*team)->survivors[1].registers = (Registers) {
                    .SP = sizeof(Segment)-1,
                    .SS = (*team)->survivors[1].stack_id*0x1000,
                    .ES = (*team)->shared_memory_id*0x1000,
            };
        }
        else {
            (*team)->living_survivors[1] = 0;
        }
    }
}

void reset_segments(void) {
    // TODO: Figure out which segments to actually reset

    if ((memset(&memory[0], 0xCC, sizeof(Segment))) == 0) exit_angrily
}

void insert_players(void) {
    // TODO: CHECK documentation for better implementation
    bool occupied[0x10000];
    if ((memset(occupied, false, sizeof(bool)*0x10000)) == 0) exit_angrily

    for(Team** team = teams_in_play; team < teams_in_play + total_team_count; team++) {
        for(uint16_t i = 0; i < 2; i++) {
            if(!((*team)->living_survivors[i])) continue;

            bool found = false;

            while(!found) {
                uint32_t location = rand() % (0x10000 - (*team)->survivors[i].code_size);

                for(uint16_t j = location; j < location + (*team)->survivors[i].code_size; j++) {
                    if(occupied[j]) goto skip;
                }

                found = true;
                
                // copy code to memory
                for(uint16_t k = 0; k < (*team)->survivors[i].code_size; k++) {
                    memory[0].values[location + k] = (*team)->survivors[i].code[k];
                    occupied[location + k] = true;
                }

                (*team)->survivors[i].registers.AX = location;
                (*team)->survivors[i].registers.IP = location;

                skip:
                    continue;
            }
        }
    }
}

bool init_round(void) {
    if (!choose_teams()) {return false;}

    resurrect_players();
    reset_segments();

    commands_ran = 0;

    current_player.team_id = 0;
    current_player.survivor_position = 0;

    insert_players();

    return true;
}