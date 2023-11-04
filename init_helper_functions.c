#include "structs_libraries_and_macros.h"
#include "globals.h"
#include "opcodes.h"
#include "opcode_helper_functions.h"

void init_survivor_and_team_count(void) {
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

void get_code(Survivor survivor[static 1], char* filename, char* dirname) { // TODO: make secure
    char tpath[512];
    strcpy(tpath, "./survivors/");
    char* path = strcat(strcat(strcat(tpath, dirname), "/"), filename);
    FILE* fptr = fopen(path, "rb");

    char tcode[MAX_CODE_SIZE];

    fseek(fptr, 0, SEEK_END); // voodoo magic to get code size
    uint16_t bin_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    if (bin_size > MAX_CODE_SIZE) {fprintf(stderr, "Code of survivor %s is too large! Max code size is %d Bytes!", tpath, MAX_CODE_SIZE); exit(1);}

    fread(tcode, bin_size, 1, fptr);
    fclose(fptr);


    char* code;
    if ((code = malloc(sizeof(char)*bin_size)) == 0) exit_angrily
    memcpy(code, tcode, sizeof(char)*bin_size);

   survivor->code = code;
   survivor->code_size = bin_size;
}

void add_survivor_to_team(Team team[static 1], Survivor survivor) {
    survivor.initialized = true;
    if (!(team->survivors[0].initialized)) {team->survivors[0] = survivor; return;}
    if (!(team->survivors[1].initialized)) {team->survivors[1] = survivor; return;}
    printf("Please make sure there are at most 2 survivors per team."); exit(1);
}

// Might be a difference between official and our implementation, here memory starts at 0, and Adam Grose proclaims
// it  should start at 0x7c00
void allocate_memory(void) {
    int segment_cnt = 1 + team_count + survivor_count + zombie_count;

    memory = malloc(sizeof(Segment)*segment_cnt);
    if (memory == 0) exit_angrily
}

void init_opcode_table(void) {
    opcode_ptr tmp_opcode_lookup_table[0x100] = {
            [0x00] = general_binary_operation,
            [0x01] = general_binary_operation,
            [0x02] = general_binary_operation,
            [0x03] = general_binary_operation,
            [0x04] = general_binary_operation,
            [0x05] = general_binary_operation,

            [0x06] = op_06,
            [0x07] = op_07,

            [0x08] = general_binary_operation,
            [0x09] = general_binary_operation,
            [0x0a] = general_binary_operation,
            [0x0b] = general_binary_operation,
            [0x0c] = general_binary_operation,
            [0x0d] = general_binary_operation,

            [0x0e] = op_0E,

            [0x10] = general_binary_operation,
            [0x11] = general_binary_operation,
            [0x12] = general_binary_operation,
            [0x13] = general_binary_operation,
            [0x14] = general_binary_operation,
            [0x15] = general_binary_operation,

            [0x16] = op_16,
            [0x17] = op_17,

            [0x18] = general_binary_operation,
            [0x19] = general_binary_operation,
            [0x1a] = general_binary_operation,
            [0x1b] = general_binary_operation,
            [0x1c] = general_binary_operation,
            [0x1d] = general_binary_operation,

            [0x1e] = op_1E,
            [0x1f] = op_1F,

            [0x20] = general_binary_operation,
            [0x21] = general_binary_operation,
            [0x22] = general_binary_operation,
            [0x23] = general_binary_operation,
            [0x24] = general_binary_operation,
            [0x25] = general_binary_operation,


            [0x28] = general_binary_operation,
            [0x29] = general_binary_operation,
            [0x2a] = general_binary_operation,
            [0x2b] = general_binary_operation,
            [0x2c] = general_binary_operation,
            [0x2d] = general_binary_operation,


            [0x30] = general_binary_operation,
            [0x31] = general_binary_operation,
            [0x32] = general_binary_operation,
            [0x33] = general_binary_operation,
            [0x34] = general_binary_operation,
            [0x35] = general_binary_operation,


            [0x38] = general_binary_operation,
            [0x39] = general_binary_operation,
            [0x3a] = general_binary_operation,
            [0x3b] = general_binary_operation,
            [0x3c] = general_binary_operation,
            [0x3d] = general_binary_operation,


            [0x40] = general_inc,
            [0x41] = general_inc,
            [0x42] = general_inc,
            [0x43] = general_inc,
            [0x44] = general_inc,
            [0x45] = general_inc,
            [0x46] = general_inc,
            [0x47] = general_inc,

            [0x48] = general_dec,
            [0x49] = general_dec,
            [0x4a] = general_dec,
            [0x4b] = general_dec,
            [0x4c] = general_dec,
            [0x4d] = general_dec,
            [0x4e] = general_dec,
            [0x4f] = general_dec,

            [0x50] = general_push_opcode,
            [0x51] = general_push_opcode,
            [0x52] = general_push_opcode,
            [0x53] = general_push_opcode,
            [0x54] = general_push_opcode,
            [0x55] = general_push_opcode,
            [0x56] = general_push_opcode,
            [0x57] = general_push_opcode,

            [0x58] = general_pop_opcode,
            [0x59] = general_pop_opcode,
            [0x5a] = general_pop_opcode,
            [0x5b] = general_pop_opcode,
            [0x5c] = general_pop_opcode,
            [0x5d] = general_pop_opcode,
            [0x5e] = general_pop_opcode,
            [0x5f] = general_pop_opcode,

            [0x70] = op_70,
            [0x71] = op_71,
            [0x72] = op_72,
            [0x73] = op_73,
            [0x74] = op_74,
            [0x75] = op_75,
            [0x76] = op_76,
            [0x77] = op_77,
            [0x78] = op_78,
            [0x79] = op_79,
            [0x7a] = op_7A,
            [0x7b] = op_7B,
            [0x7c] = op_7C,
            [0x7d] = op_7D,
            [0x7e] = op_7E,
            [0x7f] = op_7F,

            [0x80] = op_80,
            [0x81] = op_81,
            [0x82] = op_82,
            [0x83] = op_83,

            [0x84] = op_84,
            [0x85] = op_85,

            [0x86] = op_86,
            [0x87] = op_87,

            [0x88] = general_mov_op,
            [0x89] = general_mov_op,
            [0x8a] = general_mov_op,
            [0x8b] = general_mov_op,

            [0x8c] = op_8C,
            [0x8d] = op_8D,
            [0x8e] = op_8E,
            [0x8f] = op_8F,

            [0x90] = general_xchg_op,
            [0x91] = general_xchg_op,
            [0x92] = general_xchg_op,
            [0x93] = general_xchg_op,
            [0x94] = general_xchg_op,
            [0x95] = general_xchg_op,
            [0x96] = general_xchg_op,
            [0x97] = general_xchg_op,
            [0x98] = op_98,
            [0x99] = op_99,
            [0x9a] = op_9A,
            [0x9b] = op_9B,
            [0x9c] = op_9C,
            [0x9d] = op_9D,
            [0x9e] = op_9E,
            [0x9f] = op_9F,
            [0xa0] = op_A0,
            [0xa1] = op_A1,
            [0xa2] = op_A2,
            [0xa3] = op_A3,
            [0xa4] = op_A4,
            [0xa5] = op_A5,
            [0xa6] = op_A6,
            [0xa7] = op_A7,
            [0xa8] = op_A8,
            [0xa9] = op_A9,
            [0xaa] = op_AA,
            [0xab] = op_AB,
            [0xac] = op_AC,
            [0xad] = op_AD,
            [0xae] = op_AE,
            [0xaf] = op_AF,

            [0xb0] = op_B0to7,
            [0xb1] = op_B0to7,
            [0xb2] = op_B0to7,
            [0xb3] = op_B0to7,
            [0xb4] = op_B0to7,
            [0xb5] = op_B0to7,
            [0xb6] = op_B0to7,
            [0xb7] = op_B0to7,

            [0xb8] = op_B8toF,
            [0xb9] = op_B8toF,
            [0xba] = op_B8toF,
            [0xbb] = op_B8toF,
            [0xbc] = op_B8toF,
            [0xbd] = op_B8toF,
            [0xbe] = op_B8toF,
            [0xbf] = op_B8toF,

            [0xc2] = op_C2,
            [0xc3] = op_C3,
            [0xc4] = op_C4,
            [0xc5] = op_C5,
            [0xc6] = op_C6,
            [0xc7] = op_C7,
            [0xca] = op_CA,
            [0xcb] = op_CB,
            [0xcd] = op_CD,
            [0xcf] = op_CF,

            [0xd0] = op_D0,
            [0xd1] = op_D1,
            [0xd2] = op_D2,
            [0xd3] = op_D3,

            [0xd7] = op_D7,

            [0xe0] = op_E0,
            [0xe1] = op_E1,
            [0xe2] = op_E2,
            [0xe3] = op_E3,

            [0xe8] = op_E8,
            [0xe9] = op_E9,
            [0xea] = op_EA,
            [0xeb] = op_EB,

            [0xf2] = op_F2,
            [0xf3] = op_F3,

            [0xf5] = op_F5,
            [0xf6] = 0,
            [0xf7] = 0,
            [0xf8] = 0,
            [0xf9] = 0,
            [0xfa] = 0,
            [0xfb] = 0,
            [0xfc] = 0,
            [0xfd] = 0,
            [0xfe] = 0,
            [0xff] = 0
            };

    if ((opcode_lookup_table = malloc(sizeof(tmp_opcode_lookup_table))) == 0) exit_angrily
    memcpy(opcode_lookup_table, tmp_opcode_lookup_table, sizeof(tmp_opcode_lookup_table));

    operation_ptr tmp_operators[0b11111] = {
        general_add,
        general_or,
        general_adc,
        general_sbb,
        general_and,
        general_sub,
        general_xor,
        general_cmp
        };

    if ((operators = malloc(sizeof(tmp_operators))) == 0) exit_angrily
    memcpy(operators, tmp_operators, sizeof(tmp_operators));

    op_generalizer tmp_general_ops[6] = {
        [0] = general_op_0,
        [1] = general_op_1,
        [2] = general_op_2,
        [3] = general_op_3,
        [4] = general_op_4,
        [5] = general_op_5
    };

    if ((general_ops = malloc(sizeof(tmp_general_ops))) == 0) exit_angrily
    memcpy(general_ops, tmp_general_ops, sizeof(tmp_general_ops));
}