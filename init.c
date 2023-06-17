#include "init_helper_functions.h"
#include "init.h"
#include "structs_libraries_and_macros.h"
#include "globals.h"

void init_usr_input() {
    printf("Please enter the number of survivors per round: ");
    scanf("%" SCNd32, &teams_per_round);
    printf("Please enter the number of rounds: ");
    scanf("%" SCNd32, &num_of_rounds);
}

void load_players() {
    // Init survivors
    init_survivor_and_team_count();
    if ((teams = malloc(sizeof(Team)*team_count)) == 0) exit_angrily
    if ((zombies = malloc(sizeof(Team)*zombie_count)) == 0) exit_angrily

    DIR* dir;
    struct dirent* entry;

    dir = opendir("./survivors");
    if(dir == NULL) exit_angrily

    int32_t current_segment = 1;
    int32_t current_team = 0;
    int32_t current_zombie_team = 0;

    while((entry = readdir(dir))) {
        if (entry->d_name[0] == '.') continue;
        DIR* team_dir;
        struct dirent* survivor_entry;

        char tpath[512];
        strcpy(tpath, "./survivors/");
        team_dir = opendir(strcat(tpath, entry->d_name));

        Team team;
        team.survivors[0].initialized = false;
        team.survivors[1].initialized = false;

        team.living_survivors[0] = false;
        team.living_survivors[1] = false;


        team.name = malloc(sizeof(char)*(strlen(entry->d_name)+1));
        memcpy(team.name, entry->d_name, strlen(entry->d_name)+1);


        team.points = 0;

        team.shared_memory_id = current_segment++;

        while((survivor_entry = readdir(team_dir))) {
            if (survivor_entry->d_name[0] == '.') continue;
            Survivor survivor;

            get_code(&survivor, survivor_entry->d_name, entry->d_name);

            survivor.stack_id = current_segment++;
            survivor.CS = 0;

            add_survivor_to_team(&team, survivor);
        }

        if(entry->d_name[0] == 'Z') {
            team.is_zombie = true;
            zombies[current_zombie_team++] = team;
        }
        else {
            team.is_zombie = false;
            teams[current_team] = team;
            teams[current_team].team_id = current_team;
            current_team++;
        }
    }

    closedir(dir);

    allocate_memory();

    if ((teams_in_play = malloc(sizeof(Team)*(teams_per_round+zombie_count))) == 0) exit_angrily

    if ((teams_per_round > team_count)) exit_angrily
    if ((team_permutation = malloc(sizeof(int32_t)*(teams_per_round+1))) == 0) exit_angrily
    for (int i=0; i<teams_per_round; i++) team_permutation[i] = i;
    team_permutation[teams_per_round] = team_count;

    init_opcode_table();
}