#include "file_work.h"

#define enter() putc('\n', stdout)
char menu() {
    FILE* binary = read_file("games.csv");
    if (!binary) return 1;

    int option = 1;
    int ticket = 0;
    game_t g;
    while (option) {
        printf("________________________________________________\n"
               ">>>     Choose an option you what do you     <<<\n"
               ">>>      want to do with the given data      <<<\n");
        printf(">>=-- 1: print the contents of the file\n");
        printf(">>=-- 2: print the game with the given ticket id\n");
        printf(">>=-- 3: create a new ticket\n");
        printf(">>=-- 4: update ticket\'s values\n");
        printf(">>=-- 5: delate a ticket\n");
        printf(">>=-- 6: count of tickets stored\n");
        printf(">>=-- 0: end the program\n");
        printf("|>- Choose a command: ");
        scanf("%d", &option);
        switch(option) {
            case 1: enter(); print_whole_file(binary, 0); break;
            case 2: {
                printf("|>- ID of ticket you are searching for: ");
                scanf("%d", &ticket);
                enter();
                if (get_value(binary, &g, ticket)) { printf("|>=- Ticket you searched for:\n|>=- "); print_game(&g);}
                else printf("|>- Ticket with ID <%d> doesn\'t exist!\n", ticket);
                enter();
                break;
            }
            case 3: {
                printf("|>- Create a new ticket formated as such:\n"
                       "<ticket_number> <game_number> <game_date> <team_name> <ticket_type> <use_date>\n");
                scanf(ordering_in_file, &g.ticket_num, &g.game_num, &g.date1, &g.name, &g.type, &g.date2);
                g.active = 1;
                if (!write_new_value(binary, &g)) { printf("|>- Ticket with the provided ID already exists!\n\n"); break; }
                ticket = g.ticket_num;
                clear_game(&g);
                if (get_value(binary, &g, ticket)) {
                    enter();
                    printf("|>=- Newly created ticket:\n|>=- ");
                    print_game(&g);
                } else printf("|>- Unable to create the ticket\n");
                break;
            }
            case 4: {
                printf("|>- Update values of a ticket\n"
                       "   (Values updated are of a ticket with the same ID as given)\n"
                       "<ticket_number> <game_number> <game_date> <team_name> <ticket_type> <use_date>\n");
                scanf(ordering_in_file, &g.ticket_num, &g.game_num, &g.date1, &g.name, &g.type, &g.date2);
                g.active = 1;
                if (write_over_value(binary, &g)) {
                    enter();
                    printf("|>=- Updated ticket values are:\n|>=- ");
                    print_game(&g);
                    enter();
                } else printf("|>- Unable to update a ticket because a ticket with ID <%d> doesn\'t exist!\n\n", g.ticket_num);
                break;
            }
            case 5: {
                printf("|>- ID of ticket you are searching to delete: ");
                scanf("%d", &ticket);
                enter();
                if (logic_delete_value(binary, ticket))
                    printf("|>=- Ticket with ID <%d> deleted\n", ticket); 
                else
                    printf("|>- Ticket with ID <%d> doesn\'t exist!\n", ticket);
                enter();
                break;
            }
            case 6: {
                printf("\n|>=- There are %d tickets in the system.\n", count_values(binary));
                break;
            }
            default: option = 0;
        }
    }
    
    fclose(binary);
    return 0;
}

int main() {
    if (menu()) return 1;
    return 0;
}