#ifndef BP1_FILE_FUNCTIONS_H
#define BP1_FILE_FUNCTIONS_H

#include <stdio.h>

typedef enum event_type {
    ERROR = 0,
    INFO,
    WARNING
} event_t;

#define date_size 20
#define user_size 11
#define event_name_size 21

typedef struct tuple {
    long long event_num;
    char date[date_size];
    event_t type;
    char user[user_size];
    char event_name[event_name_size];
    char active;
} tuple_t;

#define block_size 3

typedef struct block {
    tuple_t tuples[3];
} block_t;

char create_serial_file(const char* file_path, FILE* file);
char write_file_contents(FILE* file);
tuple_t search_for_tuple(FILE* file, long long key);
char input_new_tuple(FILE* file, tuple_t* new_tuple);
char logic_delete_tuple(FILE* file, long long key);
char update_tuple(FILE* file, tuple_t* new_tuple);

void create_random_data(const char* file_path, int data_amount);
void print_tuple(const tuple_t* const t);

#endif //BP1_FILE_FUNCTIONS_H