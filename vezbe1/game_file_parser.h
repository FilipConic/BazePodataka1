#ifndef BP1_GAME_FILE_PARSER_H
#define BP1_GAME_FILE_PARSER_H

#include <stdio.h>
#include "game.h"

/* 
    Return values:
    0 - Word ended with space as expected
    1 - EOF reached
    2 - Word ended with \n
*/
char get_word(FILE* file, char* word) {
    int count = 0;
    char read_c = fgetc(file);
    while (read_c != EOF && read_c != '\n' && read_c != ' ' && count < team_name_len) {
        word[count++] = read_c;
        read_c = fgetc(file);
    }
    word[count] = '\0';
    switch(read_c) {
        case ' ' : return 0;
        case EOF : return 1;
        case '\n': return 2;
    }
}

#define read_date_num(reading_str, holder_str, counter, in_word_counter, read_char, read_number) \
while ((read_char = reading_str[in_word_counter++]) != '/' && \
        read_char != '_' && \
        read_char != ':' && \
        read_char != '\0') { \
            holder_str[counter++] = read_char;} \
holder_str[counter] = '\0'; \
counter = 0; \
if (holder_str[0] == '\0') return 1;\
read_number = atoi(holder_str);

/*
    Return values:
    0 - everything is okay
    1 - invalid date format
*/
char parse_date(char* word, date_t* d) {
    int count = 0;
    int in_word_counter = 0;
    char holder[5] = { 0 };
    char read_c = 0;
    int read_num = 0;

    date_t new_date;

    read_date_num(word, holder, count, in_word_counter, read_c, read_num);
    new_date.day = (char)read_num;

    read_date_num(word, holder, count, in_word_counter, read_c, read_num);
    new_date.month = (char)read_num;

    read_date_num(word, holder, count, in_word_counter, read_c, read_num);
    new_date.year = (short)read_num;

    read_date_num(word, holder, count, in_word_counter, read_c, read_num);
    new_date.hour = (char)read_num;

    read_date_num(word, holder, count, in_word_counter, read_c, read_num);
    new_date.minute = (char)read_num;

    *d = new_date;
    return 0;
}

/*
    Return values:
    0 - everything was finished correctly and a game or a ticket was appended
    1 - EOF
    2 - \n
    3 - invalid date / ticket -> skip
    4 - just skipped cause there was a problem
*/
char parse_line(FILE* file, game_array_t* g_arr) {
    char word[team_name_len] = { 0 };

    char ret_val = 0;
    int found_num = 0;
    ret_val = get_word(file, word);
    if (ret_val != 0) return ret_val;

    game_t* g_ptr;
    ticket_t t;

    if (!(found_num = atoi(word))) {
        char read_c = 0;
        while ((read_c = fgetc(file)) != '\n' && read_c != EOF);
        if (read_c == EOF) return 1;
        return 5;
    } else if (find_game_by_order_number(g_arr, &g_ptr, found_num)) {
        ret_val = get_word(file, word);
        if (ret_val != 0) return ret_val;
        ret_val = get_word(file, word);
        if (ret_val != 0) return ret_val;
        ret_val = get_word(file, word);
        if (ret_val != 0) return ret_val;
        switch(word[0]) {
            case 'S': t.type = S;      break;
            case 'O': t.type = O;      break;
            case 'P': t.type = P;      break;
            default : return 3;
        }
        ret_val = get_word(file, word);
        if (!parse_date(word, &t.date))
            add_ticket_to_game(g_ptr, &t);
        else return ret_val;
    } else {
        game_t g = create_game(found_num, 0, 0, 0, 0, 0, "\0");

        ret_val = get_word(file, word);
        if (ret_val != 0) return ret_val;
        if (parse_date(word, &g.beginning_date)) return 3;

        ret_val = get_word(file, word);
        if (ret_val != 0) return ret_val;
        char read_c = 0;
        int count = 0;
        while ((read_c = word[count]) != '\0') g.team_name[count++] = (read_c != '_') ? read_c : ' ';
        g.team_name[count] = '\0';

        ret_val = get_word(file, word);
        if (ret_val != 0) return ret_val;
        switch(word[0]) {
            case 'S': t.type = S;      break;
            case 'O': t.type = O;      break;
            case 'P': t.type = P;      break;
            default : return 3;
        }

        ret_val = get_word(file, word);
        if (!parse_date(word, &t.date))
            add_ticket_to_game(&g, &t);
        else return ret_val;
        add_game_to_array(g_arr, &g);
    }
}

void skip_four_lines(FILE* file) {
    int count = 0;
    char read_c;
    while ((read_c = fgetc(file)) != EOF && (read_c == '\n' ? ++count : count) < 4);
}
void parse_file_to_games(const char* file_name, game_array_t* g_arr) {
    FILE* file = fopen(file_name, "r");
    skip_four_lines(file);

    

    char ret_val;
    while (ret_val = parse_line(file, g_arr) != 1) {
        if (ret_val == 3) {
            char read_c;
            while((read_c = fgetc(file)) != '\n' && read_c != EOF);
            if (read_c == EOF) break;
        }
    }

    fclose(file);
}

#endif //BP1_GAME_FILE_PARSER_H