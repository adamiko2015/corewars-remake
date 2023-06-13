#include "structs_and_libraries.h"
#include "globals.h"

void init_survivor_and_team_count() {
    DIR* dir;
    struct dirent* entry;

    dir = opendir("./survivors");
    if(dir == NULL) {
        puts("Error reading survivors folder");
        exit(1);
    }

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

char* get_code(char* filename, char* dirname) {
    char tpath[512];
    strcpy(tpath, "./survivors/");
    char* path = strcat(strcat(strcat(tpath, dirname), "/"), filename);
    FILE* fptr = fopen(path, "rb");

    char tcode[0x1000];
    fseek(fptr, 0, SEEK_END); // seek to end of file
    uint16_t bin_size = ftell(fptr); // get current file pointer
    fseek(fptr, 0, SEEK_SET);
    if (bin_size == 0x1000) {printf("File %s is too large.", path); exit(1);}

    fread(tcode, bin_size, 1, fptr);
    fclose(fptr);


    char* code;
    if ((code = malloc(sizeof(char)*bin_size)) == 0) {printf("Not enough memory"); exit(1);}
    memcpy(code, tcode, sizeof(char)*bin_size);
    code[bin_size] = 0;

    return code;
}

void add_survivor_to_team(Team* team, Survivor survivor) {
    survivor.initiaized = true;
    if (!(team->survivors[0].initiaized)) {team->survivors[0] = survivor; return;}
    if (!(team->survivors[1].initiaized)) {team->survivors[1] = survivor; return;}
    printf("Please make sure there are at most 2 survivors per team."); exit(1);
}

void allocate_memory() {
    int segment_cnt = 1 + team_count + survivor_count;
    memory = malloc(sizeof(Segment)*segment_cnt);

    if (memory == 0) {
        printf("Not enough memory, try to reduce the number of survivors.");
        exit(1);
    }
}