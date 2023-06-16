#include "globals.h"
#include "structs_and_libraries.h"

void kill_current_player(Survivor* warrior) {printf("Not implemented yet %d", __line__);}

void run_current_opcode() {
    Survivor* warrior = &(teams_in_play[current_player.team_id]);
    //if (survivor.CS != 0) {kill_current_player; return;} //TODO: check if this line is needed
    int8_t opcode_byte = memory[0][warrior->IP];
    void* opcode
}

bool end_of_round() {
    if(commands_ran > commands_per_round) {
        int32_t alive_survivor_count = 0;
        int32_t alive_teams_index[teams_alive];

        int i = 0;
        int j = 0;
        for(Team* team = teams_in_play; team < teams_in_play + team_count; team++) {
            if((team->living_survivors[0] && !team->living_survivors[1]) || (!team->living_survivors[0] && team->living_survivors[1])) {
                alive_survivor_count++;
                alive_teams_index[j] = i;
                j++;
            }
            else if(team->living_survivors[0] && team->living_survivors[1]) {
                alive_survivor_count += 2;
                alive_teams_index[j] = i;
                j++;
            }
        
            i++;
        }

        double points_to_add = 1 / ((double) alive_survivor_count);

        for(int32_t* index = alive_teams_index; index < alive_teams_index + teams_alive; index++) {
            int alive_survivors_in_team = 0;
            
            if(teams)
        }

        return true;
    }
    else if(teams_alive == 1) {
        for(Team* team = teams_in_play; team < teams_in_play + teams_per_round; team++) {
            if(team->living_survivors[0] || team->living_survivors[1])
        }
    }
}
