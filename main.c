#include "init.h"
#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "new_round.h"
#include "emulate_game.h"

int main() {
    bool debug = true;
    
    setbuf(stdout, 0);

    init_usr_input();
    load_players();

    if(!debug) {
        while (init_round()) { // TODO: for testing purposes, in real game should run as a while loop condition.
            emulation_loop();
        }
    } else {
        char* input[256];
        
        while(strcmp(input, "q") != 0) {
            printf("> ");
            scanf("%s", &input);
            
            // split input
            int i = 0;
            char** words[256][30];
            char* token = strtok(input, " ");
            while(token != NULL) {
                words[i] = token;
                token = strtok(NULL, " ");
                i++;
            }

            if(strcmp(words[0], "next")) {
                if(debug_progress_simulation()) { continue; }
                else {
                    printf("Round has ended.\n");
                }
            }
        }
    }

    // TODO: display score

    return 0;
}