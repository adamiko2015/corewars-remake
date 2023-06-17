#include "structs_libraries_and_macros.h"
#include "globals.h"

void init_survivor_and_team_count() {
    DIR* dir;
    struct dirent* entry;

    dir = opendir("./survivors");
    if(dir == NULL) exit_angrily

    // Count amounts of survivors
    team_count = 0;
    zombie_count = 0;
    survivor_count = 0;

    while((entry = readdir(dir))) {
        if (entry->d_name[0] == '.') continue;

        if (entry->d_name[0] == 'Z') zombie_count++;
        else team_count++;

        char tpath[512];
        strcpy(tpath, "./survivors/");
        DIR* team_dir = opendir(strcat(tpath, entry->d_name));

        struct dirent* survivor_entry;
        while((survivor_entry = readdir(team_dir))) {
            if (survivor_entry->d_name[0] == '.') continue;
            survivor_count++;
        }

        closedir(team_dir);
    }

    closedir(dir);
}

void get_code(Survivor* survivor, char* filename, char* dirname) { // TODO: validate
    char tpath[512];
    strcpy(tpath, "./survivors/");
    char* path = strcat(strcat(strcat(tpath, dirname), "/"), filename);
    FILE* fptr = fopen(path, "rb");

    char tcode[0x1000];

    fseek(fptr, 0, SEEK_END); // voodoo magic to get code size
    uint16_t bin_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (bin_size == 0x100) exit_angrily

    fread(tcode, bin_size, 1, fptr);
    fclose(fptr);


    char* code;
    if ((code = malloc(sizeof(char)*bin_size)) == 0) exit_angrily
    memcpy(code, tcode, sizeof(char)*bin_size);

   survivor->code = code;
   survivor->code_size = bin_size;
}

void add_survivor_to_team(Team* team, Survivor survivor) {
    survivor.initialized = true;
    if (!(team->survivors[0].initialized)) {team->survivors[0] = survivor; return;}
    if (!(team->survivors[1].initialized)) {team->survivors[1] = survivor; return;}
    printf("Please make sure there are at most 2 survivors per team."); exit(1);
}

void allocate_memory() {
    int segment_cnt = 1 + team_count + survivor_count;
    memory = malloc(sizeof(Segment)*segment_cnt);

    if (memory == 0) exit_angrily
}

void init_opcode_table() {
    if ((opcode_lookup_table = calloc(0xFF, sizeof(opcode_ptr))) == 0) exit_angrily
    // TODO: initialize opcode table with functions in the right places
}