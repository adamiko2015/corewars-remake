#include "globals.h"
#include "structs_libraries_and_macros.h"

void kill_current_player()
{
    teams_in_play[current_player.team_id]->living_survivors[current_player.survivor_position] = 0;

    if(!teams_in_play[current_player.team_id]->living_survivors[(current_player.survivor_position + 1) % 2]
        && !teams_in_play[current_player.team_id]->is_zombie) teams_alive--;
}

void run_current_opcode() {
    Survivor* survivor = &(teams_in_play[current_player.team_id]->survivors[current_player.survivor_position]);

    uint8_t opcode_lookup_value = memory[0].values[survivor->registers.IP];

    opcode_ptr opcode = opcode_lookup_table[opcode_lookup_value];
    if (opcode == 0) { kill_current_player(); return; }

    bool opcode_res = opcode(survivor, teams_in_play[current_player.team_id]->shared_memory_id);
    if (!opcode_res) {kill_current_player(); return;}
    commands_ran++;
}

bool round_end_check() {
    // TODO: What about zombies?
    if(commands_ran > commands_per_round) {
        int32_t alive_survivor_count = 0;

        debug_print_statement

        for(Team** team = teams_in_play; team < teams_in_play + team_count; team++) {
            alive_survivor_count += (*team)->living_survivors[0] + (*team)->living_survivors[1];
        }

        debug_print_statement

        double points_to_add = (double) 1 / alive_survivor_count;

        debug_print_statement

        for(Team** team = teams_in_play; team < teams_in_play + team_count; team++) {
            teams[(*team)->team_id].points += points_to_add * ((*team)->living_survivors[0] + (*team)->living_survivors[1]);
        }

        debug_print_statement
        
        return true;
    }
    else if(teams_alive == 1) {
        for(Team** team = teams_in_play; team < teams_in_play + team_count; team++) {
            if ((*team)->living_survivors[0] | (*team)->living_survivors[1]) (*team)->points += 1;
        }

        return true;
    }
    
    return false;
}

void advance_player_tracker() {
    do {
        current_player.team_id = (current_player.survivor_position + current_player.team_id) % (teams_per_round+zombie_count);
        current_player.survivor_position = (current_player.survivor_position + 1) & 1;}
    while (!(teams_in_play[current_player.team_id]->living_survivors[current_player.survivor_position]));
}

void emulation_loop() {
    while (!round_end_check()) {
        run_current_opcode();
        advance_player_tracker();
    }
}