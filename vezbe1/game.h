#ifndef BP1_GAME_H
#define BP1_GAME_H

#include <stdlib.h>

#define team_name_len 50
#define num_fans_inc 10
#define num_game_inc 5

typedef struct date {
    char day, month;
    short year;
    char hour, minute;
} date_t;

typedef enum ticket_type {
    NO_VAL = 0,
    O,
    S,
    P
} ticket_type_e;

typedef struct ticket {
    ticket_type_e type;
    date_t date;
} ticket_t;

typedef struct game {
    int num;
    date_t beginning_date;
    char team_name[team_name_len];

    ticket_t* tickets;
    int num_fans;
    int capacity;
} game_t;

game_t create_game(int order_number, char day, char month, short year, char hour, char minute, char* team_name) {
    date_t beginning_date = {
        .day = day,
        .month = month,
        .year = year,
        .hour = hour,
        .minute = minute
    };
    game_t g = {
        .num = order_number,
        .beginning_date = beginning_date,
        .team_name = 0,

        .tickets = 0,
        .num_fans = 0,
        .capacity = num_fans_inc
    };

    char curr, count = 0;
    while (curr = team_name[count] && count < team_name_len) {
        g.team_name[count] = team_name[count];
        ++count;
    }

    g.tickets = (ticket_t*)calloc(g.capacity, sizeof(ticket_t));

    return g;
}
void increase_game_capacity(game_t* g) {
    g->capacity += num_fans_inc;
    ticket_t* new_tickets = (ticket_t*)calloc(g->capacity, sizeof(ticket_t));

    for (int i = 0; i < g->num_fans; ++i)
        new_tickets[i] = g->tickets[i];
    free(g->tickets);
    g->tickets = new_tickets;
}
void add_ticket_to_game(game_t* g, ticket_t* new_ticket) {
    if (g->num_fans == g->capacity)
        increase_game_capacity(g);
    
    g->tickets[g->num_fans++] = *new_ticket;
}
void delete_game(game_t* g) {
    if (g->tickets != 0) free(g->tickets);
    g->tickets = 0;
    g->num_fans = g->capacity = 0;
}

typedef struct game_array {
    game_t* data;
    int count;
    int capacity;
} game_array_t;

game_array_t create_game_array() {
    game_array_t g_arr = {
        .data = 0,
        .count = 0,
        .capacity = num_game_inc
    };

    g_arr.data = (game_t*)calloc(g_arr.capacity, sizeof(game_t));
    return g_arr;
}
void increase_game_array_capacity(game_array_t* g_arr) {
    g_arr->capacity += num_game_inc;
    game_t* new_data = (game_t*)calloc(g_arr->capacity, sizeof(game_t));

    for (int i = 0; i < g_arr->count; ++i)
        new_data[i] = g_arr->data[i];
    free(g_arr->data);
    g_arr->data = new_data;
}
char find_game_by_order_number(game_array_t* g_arr, game_t** g, int number) {
    for (int i = 0; i < g_arr->count; ++i)
        if (g_arr->data[i].num == number) {
            *g = &(g_arr->data[i]);
            return 1;
        }
    return 0;
}
char add_game_to_array(game_array_t* g_arr, game_t* g) {
    if (g_arr->capacity == g_arr->count)
        increase_game_array_capacity(g_arr);

    g_arr->data[g_arr->count++] = *g;

    return 0;
}
void delete_game_array(game_array_t* g_arr) {
    for (int i = 0; i < g_arr->count; ++i)
        delete_game(&(g_arr->data[i]));
    
    free(g_arr->data);
    g_arr->capacity = g_arr->count = 0;
}

void print_date(date_t* d) {
    printf("%d/%d/%d at %d:%d", (int)d->day, (int)d->month, (int)d->year, (int)d->hour, (int)d->minute);
}
void print_ticket(ticket_t* t) {
    switch(t->type) {
        case O: printf(" - Basic"); break;
        case S: printf(" - Seasonal"); break;
        case P: printf(" - Discount"); break;
    }
    printf(" ticket used on ");
    print_date(&(t->date));
    printf("\n");
}
void print_game(game_t* g) {
    date_t* d = &(g->beginning_date);;
    printf("Game %d, Team: %s on ", g->num, g->team_name);
    print_date(&(g->beginning_date));
    printf("\n");

    for (int i = 0; i < g->num_fans; ++i)
        print_ticket(&(g->tickets[i]));
}
void print_game_array(game_array_t* g_arr) {
    for (int i = 0; i < g_arr->count; ++i) {
        print_game(&(g_arr->data[i]));
    }
}

int number_of_tickets_by_type(game_array_t* g_arr, ticket_type_e tt) {
    int counter = 0;

    for (int i = 0; i < g_arr->count; ++i) {
        game_t* g_ptr = &(g_arr->data[i]);
        for (int i = 0; i < g_ptr->num_fans; ++i) {
            ticket_t* t_ptr = &(g_ptr->tickets[i]);
            if (t_ptr->type == tt) ++counter;
        }
    }

    return counter;
}
float money_earned_by_type(game_array_t* g_arr, ticket_type_e tt) {
    float mult;
    switch (tt) {
        case O: mult = 50.0; break;
        case S: mult = 30.0; break;
        case P: mult = 25.0; break;
    }
    return number_of_tickets_by_type(g_arr, tt) * mult;
}

#endif //BP1_GAME_H