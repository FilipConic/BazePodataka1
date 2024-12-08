#include "file_work.h"

#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#elif _linux_
#include <unistd.h>
#endif

const game_t null_game = {-1, 0, "", "", 0, "", 0};

void clear_game(game_t* g) {
    g->ticket_num = 0;
    g->game_num = 0;
    g->type = 0;
    memset(g->date1, 0, arr_size);
    memset(g->name, 0, arr_size);
    memset(g->date2, 0, arr_size);
}
void print_game(game_t *g) {
    printf(ordering_in_file "\n", g->ticket_num, g->game_num, g->date1, g->name, g->type, g->date2);
}
void print_whole_file(FILE* file, char has_to_be_active) {
    game_t g;
    rewind(file);
    fread(&g, sizeof(g), 1, file);
    while (g.ticket_num != -1 && !feof(file)) {
        if (g.active || !has_to_be_active)
            print_game(&g);
        fread(&g, sizeof(g), 1, file);
    }
}

#define skip_four_rows() \
    int count = 0; \
    char read_c; \
    while ((read_c = fgetc(file)) != EOF) {if (read_c == '\n') ++count; if (count == 4) break;}
#define set_at_neg_one(file) fseek(file, -(long int)sizeof(null_game), SEEK_END)
#define input_empty_row(file) fseek(file, 0, SEEK_END); fwrite(&null_game, sizeof(null_game), 1, file)

char search_for_value(FILE* file, int value) {
    rewind(file);
    game_t g;

    fread(&g, sizeof(g), 1, file);
    while (g.ticket_num != -1) {
        if (g.ticket_num == value) return 1;
        fread(&g, sizeof(g), 1, file);
    }
    return 0;
}
char get_value(FILE* file, game_t* g, int value) {
    rewind(file);

    fread(g, sizeof(game_t), 1, file);
    while (g->ticket_num != -1) {
        if (g->ticket_num == value) return 1;
        fread(g, sizeof(game_t), 1, file);
    } 
    return 0;
}

char write_new_value(FILE* file, game_t* g) {
    if (!search_for_value(file, g->ticket_num)) {
        set_at_neg_one(file);
        fwrite(g, sizeof(game_t), 1, file);
        input_empty_row(file);
        return 1;
    } else return 0;
}
char write_over_value(FILE* file, game_t* g) {
    if (!search_for_value(file, g->ticket_num)) return 0;
    fseek(file, -(long int)sizeof(game_t), SEEK_CUR);
    fwrite(g, sizeof(game_t), 1, file);
    return 1;
}

int count_values(FILE* file) {
    rewind(file);
    game_t g;
    int counter = 0;

    fread(&g, sizeof(game_t), 1, file);
    while(g.ticket_num != -1 && !feof(file)) {
        ++counter;
        fread(&g, sizeof(game_t), 1, file);
    }

    return counter;
}
void truncate_deleted_values(FILE* file) {
    rewind(file);
    int counter = 0, curr_pos = 0;
    game_t g;
    fread(&g, sizeof(game_t), 1, file);
    while (g.ticket_num != -1 && !feof(file)) {
        if (counter) {
            fseek(file, -(long int)sizeof(game_t) * (counter + 1), SEEK_CUR);
            fwrite(&g, sizeof(game_t), 1, file);
            
            fseek(file, (long int)sizeof(game_t) * counter, SEEK_CUR);
        }

        if (!g.active) ++counter;
        fread(&g, sizeof(game_t), 1, file);
    }
    fseek(file, -(long int)sizeof(game_t) * (counter + 1), SEEK_CUR);
    fwrite(&null_game, sizeof(game_t), 1, file);

    #ifdef _WIN32
        _chsize_s(fileno(file), (count_values(file) + 1) * sizeof(game_t));
    #elif _linux_
        ftruncate(fileno(file), (count_values(file) + 1) * sizeof(game_t));
    #endif
}
char logic_delete_value(FILE* file, int value) {
    game_t g;
    
    if (!search_for_value(file, value)) return 0;
    fseek(file, -(long int)sizeof(game_t), SEEK_CUR);
    fread(&g, sizeof(game_t), 1, file);
    if (!g.active) return 0;
    g.active = 0;
    fseek(file, -(long int)sizeof(game_t), SEEK_CUR);
    fwrite(&g, sizeof(game_t), 1, file);

    truncate_deleted_values(file);
    return 1;
}

FILE* read_file(const char* input_file_name) {
    FILE* file = fopen(input_file_name, "r");
    if (file == NULL) return 0;
    FILE* save = fopen("data.bin", "wb+");
    if (save == NULL) return 0;

    game_t g;
    g.active = 1;
    skip_four_rows();
    input_empty_row(save);
    while (fscanf(file, ordering_in_file, &g.ticket_num, &g.game_num, &g.date1, &g.name, &g.type, &g.date2) != EOF) {
        write_new_value(save, &g); 
        clear_game(&g);
    }

    fclose(file);
    return save;
}

