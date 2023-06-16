#include "globals.h"
#include "structs_and_libraries.h"

void kill_current_player()
{
    teams_in_play[current_player.team_id].living_survivors[current_player.survivor_position] = 0;

    if(!teams_in_play[current_player.team_id].living_survivors[(current_player.survivor_position + 1) % 2]) teams_alive--;
}

void run_current_opcode() {
    debug_print_statement

    Survivor* warrior = &(teams_in_play[current_player.team_id]);
    //if (survivor.CS != 0) {kill_current_player; return;} //TODO: check if this line is needed
    int8_t opcode_lookup_value = memory[0][warrior->IP];
    
    bool (*opcode)(Survivor* warrior) = opcode_lookup_table[opcode_lookup_value];
    if (opcode == 0) { kill_current_player(warrior); return; }

    if (!opcode(warrior)) { kill_current_player(warrior); return; }

    commands_ran++;

    debug_print_statement
}

bool end_of_round() {
    // TODO: What about zombies?
    if(commands_ran > commands_per_round) {
        int32_t alive_survivor_count = 0;
        int32_t alive_teams_index[teams_alive];

        debug_print_statement

        int i = 0;
        int j = 0;
        for(Team* team = teams_in_play; team < teams_in_play + team_count; team++) {
            alive_survivor_count += team->living_survivors[0] + team->living_survivors[1];
            alive_teams_index[j] = i * (team->living_survivors[0] | team->living_survivors[1]);
            j += team->living_survivors[0] | team->living_survivors[1];

            i++;
        } 

        debug_print_statement

        double points_to_add = (double) 1 / alive_survivor_count;

        debug_print_statement

        for(int32_t* index = alive_teams_index; index < alive_teams_index + teams_alive; index++) {
            teams[teams_in_play[*index].team_id].points += points_to_add * (team->living_survivors[0] + team->living_survivors[1]);
        }

        debug_print_statement
        
        return true;
    }
    else if(teams_alive == 1) {
        for(Team* team = teams_in_play; team < teams_in_play + team_count; team++) {
            if (team->living_survivors[0] | team->living_survivos[1]) teams[team->team_id].points += 1;
        }

        return true;
    }
    
    return false;
}

void advance_player_tracker() {
    debug_print_statement
    
    do {current_player.team_id = (current_player.survivor_position + current_player.team_id) % (teams_per_round+zombie_count);
    current_player.survivor_position = (current_player.survivor_position + 1) & 1;} while (!(teams_in_play[team_id].living_survivors[0] | teams_in_play[team_id].living_survivors[1]))

    debug_print_statement
}