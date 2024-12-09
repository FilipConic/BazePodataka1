#include "file_functions.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

#define date_format_m "%d/0%d/%d_%d:%d:%d"
/*
* Macro used to generate a random date string
*/
#define generate_random_date_m(str) sprintf(str, date_format_m, rand() % 18 + 10, rand() % 9 + 1, rand() % 23 + 2000, rand() % 13 + 10, rand() % 50 + 10, rand() % 50 + 10)
/*
* Function used to generate random data to be used for filling the serial file
*/
void create_random_data(const char* file_path, int data_amount) {
    srand(time(NULL));

    const char* event_names[] = {"Slava", "Rodjendan", "Vencanje", "Krstenje", "Proslava"};
    const char* names[] = {"Nikola", "Marina", "Radovan", "Vladimir", "Sofija", "Anja", "Jelena", "Filip", "Marko", "Miroslava", "Natalija", "Vladan", "Stefan", "Jovana", "Ivana", "Aleksa"};

    FILE* file = fopen(file_path, "w+");
    if (!file) return;

    tuple_t t;
    char date[23] = { 0 };
    char event_type_name[8] = { 0 };
    for (int i = 0; i < data_amount; ++i) {
        if (i) fprintf(file, "\n");

        t.event_num = (rand() * rand()) % 999999000000 + 1000000; // Genereate a random event number between 10^6 to 10^13 - 1

        generate_random_date_m(date);
        strcpy_s(t.date, date_size * sizeof(char), date); // Generate a random date with the macro given above and copy it into the tuple so all the data would be in one place and to make sure everything is correct size for later use

        if (rand() % 100) { // Generate a random event type. Most likely is INFO, then WARNING and the least common is ERROR
            if (rand() % 50) strcpy(event_type_name, "INFO");
            else strcpy(event_type_name, "WARNING");
        } else strcpy(event_type_name, "ERROR");

        if (rand() % 50) strcpy_s(t.user, user_size * sizeof(char), names[rand() % 16]); // Choose a random name of the ones provided above
        else strcpy_s(t.user, user_size * sizeof(char), "SYSTEM"); // Adding a few random system events

        strcpy_s(t.event_name, event_name_size, event_names[rand() % 5]); // Choose a random event from the ones provided above

        fprintf(file, "%lld %s %s %s %s", t.event_num, t.date, event_type_name, t.user, t.event_name);
    }
    fclose(file);
}

const tuple_t null_event = {-1, "", 0, "", "", 0};

char create_serial_file(const char* file_path, FILE* file) {
    file = fopen(file_path, "ab+");
    if (!file) return 2;
    
}