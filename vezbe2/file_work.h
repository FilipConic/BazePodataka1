#ifndef BP1_FILE_WORK_H
#define BP1_FILE_WORK_H

#include <stdio.h>

#define arr_size 50
#define ordering_in_file "%d %d %s %s %c %s"

typedef struct game {
    int ticket_num, game_num;
    char date1[arr_size];
    char name[arr_size];
    char type;
    char date2[arr_size];
    int active;
} game_t;

void clear_game(game_t* g);
void print_game(game_t *g);
void print_whole_file(FILE* file, char has_to_be_active);

char search_for_value(FILE* file, int value);
char get_value(FILE* file, game_t* g, int value);

char write_new_value(FILE* file, game_t* g);
char write_over_value(FILE* file, game_t* g);

int count_values(FILE* file);
void truncate_deleted_values(FILE* file);
char logic_delete_value(FILE* file, int value);

FILE* read_file(const char* input_file_name);

#endif //BP1_FILE_WORK_H