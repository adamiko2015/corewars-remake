#include "init.h"
#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "new_round.h"
#include "emulate_game.h"

int main() {
    init_usr_input();
    load_players();

    while (init_round()) // TODO: for testing purposes, in real game should run as a while loop condition.
        emulation_loop();
    
    // TODO: display score

    return 0;
}