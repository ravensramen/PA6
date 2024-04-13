#include "header.h"

//fix looping + formatting

int main() {
    int menu = 1;
    int current_player = 1;

    int winner = 0;

    Hand player1, player2, selection_array, selection_array2 = { 0,0,0,0,0 };

    int pair_p1, three_of_a_kind_p1, four_of_a_kind_p1, flush_p1, straight_p1,

        pair_p2, three_of_a_kind_p2, four_of_a_kind_p2, flush_p2, straight_p2;

    /* initialize suit array */
    const char* suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" };
    /* initialize face array */
    const char* face[13] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
    /* initalize deck array */
    int deck[4][13] = { 0 };

    srand((unsigned)time(NULL));

    shuffle(deck);
    

    do {
        display_menu();
        scanf("%d", &menu);
        if (menu == 3) {
            printf("Thanks for playing, bye bye!! :P\n");
            break;
        }
        else if (menu == 2)
            display_rules();

        //deal to player 1
        deal(&player1, current_player, deck, face, suit);

        //deal to player 2 (dealer)
        current_player = 2;
        printf("\n");
        deal(&player2, current_player, deck, face, suit); //dealer's cards
       
        //allow player 1 to redraw desired cards:
        select_cards(&player1, &selection_array, deck, face, suit); //allow to redraw cards
        draw_cards(&player1, &selection_array, deck, face, suit);

        //allow dealer to redraw if bad hand (look for pairs>> redraw some cards if no pairs)
        pair_p2 = determine_pair(&player2, &selection_array2);
        if (pair_p2 < 1) {
            redraw_dealer(&player2, &selection_array2, deck, face, suit);
        }


        //determine final score for dealer
        printf("\nFinding scores for the dealer: \n");
        pair_p2 = determine_pair(&player2, &selection_array2);
        three_of_a_kind_p2 = determine_three(&player2, &selection_array2);
        four_of_a_kind_p2 = determine_four(&player2, &selection_array2);
        flush_p2 = determine_flush(&player2, &selection_array2);
        straight_p2 = determine_straight(&player2, &selection_array2);

        //determine final score for player
        printf("\nFinding scores for player one (you): \n");
        pair_p1 = determine_pair(&player1, &selection_array);
        three_of_a_kind_p1 = determine_three(&player1, &selection_array);
        four_of_a_kind_p1 = determine_four(&player1, &selection_array);
        flush_p1 = determine_flush(&player1, &selection_array);
        straight_p1 = determine_straight(&player1, &selection_array);

        determine_winner(&winner, pair_p1, pair_p2, three_of_a_kind_p1, three_of_a_kind_p2, four_of_a_kind_p1, four_of_a_kind_p2, flush_p1, flush_p2, straight_p1, straight_p2);

        if (winner == 0){
            printf("House wins... too bad...");
        }
        if (winner == 1) {
            printf("You win! Congrats");
        }


        printf("\nRound over... wanna play again? Enter 0 to exit: \n");
        scanf("%d", &menu);

        system("cls");

    } while (menu != 0);

    return 0;
}
