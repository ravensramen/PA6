#include "header.h"

int main() {

	srand((unsigned int)(time(NULL))); //allows random boat placement and shots by the computer player

	printf("Welcome to Battleship!\n\n"); //Welcome message upon first launch

	char player_1_gameboard[NUM_ROWS][NUM_COLS] = { {'\0'} }; //player's gameboard
	char player_2_gameboard[NUM_ROWS][NUM_COLS] = { {'\0'} }; //computer's gameboard
	char player_2_display_gameboard[NUM_ROWS][NUM_COLS] = { {'\0'} }; //computer's displayed board

	int choice = 0; //initialize choice variable

	do {
		menu_choice(&choice); //function that displays menu and prompts for user choice, menu repeats each round

		switch (choice) { //switch statement runs program based on user's choice
		case 1:
			display_rules(); //function to display rules
			break;
		case 2:
			battleship_gameplay(player_1_gameboard, player_2_gameboard, player_2_display_gameboard); //main gameplay function
			break;
			
		case 3:
			printf("Goodbye..."); //goodbye message
			break;
		}

	} while (choice != 3); //as long as the user doesn't choose to exit, menu repeats 

	return 0; 
}
