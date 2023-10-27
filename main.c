#include "init.h"
#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "new_round.h"
#include "emulate_game.h"

int main(void) {
    setbuf(stdout, 0);

    init_usr_input();
    load_players();


    while (init_round()) {
        emulation_loop();
    }


    // TODO: display score

    return 0;
}