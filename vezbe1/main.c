#include "game_file_parser.h"

int main() {
    game_array_t game_arr = create_game_array();
    
    parse_file_to_games("games.csv", &game_arr);
    print_game_array(&game_arr);

    printf("Basic ticket info: %d, %.2f\n", number_of_tickets_by_type(&game_arr, O), money_earned_by_type(&game_arr, O));
    printf("Seasonal ticket info: %d, %.2f\n", number_of_tickets_by_type(&game_arr, S), money_earned_by_type(&game_arr, S));
    printf("Discounted ticket info: %d, %.2f\n", number_of_tickets_by_type(&game_arr, P), money_earned_by_type(&game_arr, P));

    delete_game_array(&game_arr);
    return 0;
}