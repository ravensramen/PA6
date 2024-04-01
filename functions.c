#include "header.h"

//Function Name: void menu_choice(int *choice)
//Description: Displays main menu options to console and uses a pointer to store the user's choice
//Parameters: Address of the choice variable
//Output: Saves the user's choice to be used in the menu switch statement.

void menu_choice(int *choice) {
	int option = 0;
	printf("Main Menu: \n");
	printf("Choose options: Enter 1 to print rules, 2 to begin playing battleship, and 3 to exit: \n");
	scanf("%d", &option);
	*choice = option;
}

//Function Name: void display_rules(void);
//Description: Prints the rules of battleship to the console. 
//Parameters: N/A
//Output: N/A, just prints rules

void display_rules(void) {
	printf("Here are the rules: \n\n");
	printf("This is a two player game, you are player one, and I am player two! \n");
	printf("We each get a 10 by 10 board to place each of our 5 ships of different sizes. \n");
	printf("I'll let you choose where to place each ship (Carrier 'c', Battleship 'b', Cruiser 'r', Submarine 's', and Destroyer 'd'. \n\n");
	printf("Next, you get to choose spaces to shoot on my board, if you get a hit, a '*' will be displayed, otherwise a 'm' will be displayed. \n");
	printf("We will continue alternating turns shooting eachother's ships, whoever sinks all of their opponent's fleet first wins!\n");
	printf("Goodluck! Without further ado, let's play!!\n\n");

}

//Function Name: void battleship_gameplay(void)
//Description: Main function, stores all the functions neccessary to play a game of battleship
//Parameters: N/A
//Output: N/A Runs a round of battleship.

void battleship_gameplay(char player_1_gameboard[][NUM_COLS], char player_2_gameboard[][NUM_COLS], char player_2_display_gameboard[][NUM_COLS]) {

	FILE* outfile = fopen("battleship.log", "w"); //opens log file

	int starts_first = 0; //randomly selects first player, determines which player is going
	int place_type = 0; //allows user to manually place ships or randomly generate placement


	int x_position = 0, y_position = 0; //for shooting

	int was_hit = 0, was_sunk = 0; //determines if computer and player shots were hits and if they sunk boats
	int generating = 0; //for computer to randomly generate targetes

	char ship_hit = '/0';

	bool has_won = false, has_lost = false; //boolean for whether or not the player has won or lost, continues gameplay loop

	int computer_has_tried[NUM_ROWS][NUM_COLS] = { {0} }; //keeps track of where the computer has already randomly shot at

	Stats player_one = { 0, 0, 0, 0.0 }; //initialize stat structures
	Stats player_two = { 0, 0, 0, 0.0 };

	initialize_game_board(player_1_gameboard, NUM_ROWS, NUM_COLS); //initialize player 1's gameboard
	initialize_game_board(player_2_gameboard, NUM_ROWS, NUM_COLS); //initialize computer's gameboard

	initialize_game_board(player_2_display_gameboard, NUM_ROWS, NUM_COLS); //initialize player 2's displayed board


	choose_place_type(&place_type); //prompts user to choose manual or random placement

	//player chose manual
	if (place_type == 1) { 
		
		manual_place(player_1_gameboard, NUM_ROWS, NUM_COLS);
	}

	//player chose random
	if (place_type == 2) {

		random_place(player_1_gameboard, NUM_ROWS, NUM_COLS);
	}

	random_place(player_2_gameboard, NUM_ROWS, NUM_COLS); //randomly assigns computer's gameboard

	printf("Okie dokie, here's what your board looks like: \n");
	print_board(player_1_gameboard, NUM_ROWS, NUM_COLS);
	system("pause");

	select_who_starts_first(&starts_first); //randomly decides who starts first

	system("pause");
	system("cls");

	while (!has_won || !has_lost) { //while outcome has yet to be decided

		switch (starts_first) {
		case 1:
			printf("Your Current Board: \n");
			print_board(player_1_gameboard, NUM_ROWS, NUM_COLS);
			printf("My Current Board: \n");
			print_board(player_2_display_gameboard, NUM_ROWS, NUM_COLS);

			printf("Enter a target of my board to hit (row, column): Hit enter after each integer \n");
			scanf("%d%d", &x_position, &y_position);

			player_one.total_shots++;

			was_hit = check_shots(player_2_gameboard, x_position, y_position, &ship_hit);
			was_sunk = check_if_sunk(player_2_gameboard, ship_hit);

			update_board(player_2_gameboard, x_position, y_position, was_hit);
			update_board(player_2_display_gameboard, x_position, y_position, was_hit);

			if (was_hit) {
				player_one.number_hits++;
				printf("Hit!\n");
				if (was_sunk) {
					printf("You sunk my ship with indicated by %c! How RUDE!!\n", ship_hit);
				}
			}
			else {
				player_one.number_misses++;
				printf("Miss... womp womp..\n");
			}
			system("pause");
			system("cls");

			output_current_move(outfile, starts_first, x_position, y_position, was_hit, was_sunk);

			if (determine_winner(player_2_gameboard)) { //check to see if player one has satisfied winning conditions by hitting all of the computer's gameboard
				printf("You won!\n");
				system("pause");
				has_won = 1;
				goto breakout; //writes game summary to outfile
			}
			starts_first = 2; //switches to player 2 (computer)
			break;

		case 2: //computer's turn
			generating = 1;
			while (generating) {
				x_position = rand() % 10; //chooses a random x position out of the 10 possible
				y_position = rand() % 10; //chooses a random y

				if (computer_has_tried[x_position][y_position] != 1) { //if computer hasn't already shot at this random location
					computer_has_tried[x_position][y_position] = 1; //location cannot be used again
					generating = 0; //generation process is finished
				}
			}
			player_two.total_shots++;//adds one to the computer's total shots
			was_hit = check_shots(player_1_gameboard, x_position, y_position, &ship_hit); //checks if shot hit
			was_sunk = check_if_sunk(player_1_gameboard, ship_hit); //checks if ships sank

			if (was_hit) { //if hit
				player_two.number_hits++; //add one to hit stats
				printf("I shot at (%d, %d). It was a hit!! :P \n", x_position, y_position);
				if (was_sunk) {
					printf("I shot at (%d, %d) It was a hit AND I sunk your ship!! >:)\n", x_position, y_position);
				}
			}
			else {
				player_two.number_misses++; //add one to the total misses stat
				printf("I shot at (%d, %d) It was a miss... You're safe for now.. \n", x_position, y_position);
			}

			system("pause");
			system("cls");

			update_board(player_1_gameboard, x_position, y_position, was_hit);

			if (determine_winner(player_1_gameboard)) { //if all computer has reached 17 hits 
				printf("You lost, I sank your fleet... Mwauahahah\n\n");
				system("pause");
				has_lost = 1;
				goto breakout; //writes game summary to outfile
			}
			output_current_move(outfile, starts_first, x_position, y_position, was_hit, was_sunk);
			starts_first = 1; //switches back to player 1's turn
			break;
		}
	}

breakout:

	printf("Statistics added to the logfile! ...");
	
	player_one.hits_misses = ((double)player_one.number_hits / player_one.total_shots) * 100; //calculates ratio of hits and misses for both players, writes as a percent
	player_two.hits_misses = ((double)player_two.number_hits / player_two.total_shots) * 100;

	output_stats(outfile, player_one, player_two); //writes all the game stats stored in player structs to the outfile
	fclose(outfile); //closes file

	return; //go back to main after game is over
}



//Function Name: void initialize_game_board(char board[10][10], int num_rows, int num_columns)
//Description: Initializes the players gameboard at the start of each round.
//Parameters: Character array coresponding to the player's board, number of rows and number of columns.
//Output: A blank gameboard.

void initialize_game_board(char board[10][10], int num_rows, int num_columns) {
	for (int i = 0; i < num_rows; i++) { //sets each row of 10 characters, starts a new column when 10 characters reached
		for (int j = 0; j < num_columns; j++) {
			board[i][j] = '~'; //sets to blank
		}
	}
}

//Function Name: void print_board(char board[10][10], int num_rows, int num_columns) 
//Description: Prints the current board to the console.
//Parameters: Character array coresponding to player board and the max number of rows and columns
//Output: N/A prints board to the console

void print_board(char board[10][10], int num_rows, int num_columns) {
	for (int i = 0; i < num_rows; i++) {
		if (i == 0) {
			printf("    0  1  2  3  4  5  6  7  8  9\n"); //prints horizontal origin 
		}
		for (int j = 0; j < num_columns; j++) {
			if (j == 0) {
				printf(" %d ", i); //prints vertical origin
			}
			printf(" %c ", board[i][j]);
		}
		printf("\n");
	}
}

//Function Name: void select_who_starts_first(int* starts_first);
//Description: Selects which player starts the game first.
//Parameters: Pointer coresponding to variable of which player goes first.
//Output: N/A just determines who starts first. 


void select_who_starts_first(int *starts_first) {
		int num = 0;
		num = rand() % 2 + 1; //randomly chooses who starts first, 1 for player 1, 2 for player 2
		*starts_first = num;

		if (num == 1) {
			printf("Player 1 will start first (lucky you!)\n");
		}
		if (num == 2) {
			printf("Player 2 will start first (that's me! :P)\n");
		}
	}

//Function Name: void choose_place_type(int* place_type)
//Description: Prompts the user to chose how they wish to place their fleet.
//Parameters: Pointer to variable coresponding to choice.
//Output:  N/A, indirectly modifies choice variable.

void choose_place_type(int* place_type) {
	int choice = 0;
	printf("How do you want to place your fleet?\n");
	printf("Either you can choose placement of each ship manually or placement can be randomly assigned.\n\n"); \
	printf("Enter '1' to place manually, or '2' to have random placement\n");
	scanf("%d", &choice);

	*place_type = choice;
	
}

//Function Name: void manual_place(char board[][NUM_COLS])
//Description: Allows the user to manually choose where to place their fleet.
//Parameters: Array of the player's gameboard
//Output: Indirectly modifies the player's gameboard array.

void manual_place(char board[][NUM_COLS]) {
	int row = 0, col = 0; //integers coresponding to cordinates chosen

	//create an array for each ship
	int carrier_cords[CARRIER_LENGTH * 2] = { 0 }, battleship_cords[BATTLESHIP_LENGTH * 2] = { 0 },
		cruiser_cords[CRUISER_LENGTH * 2] = { 0 }, submarine_cords[SUBMARINE_LENGTH * 2] = { 0 },
		destroyer_cords[DESTROYER_LENGTH * 2] = { 0 };
	//length values are *2 because each array stores the digits coresponding to row AND column (one after the other)

	printf("So you've chosen to manually place your ships, you will be prompted to enter the cells that you'd like to place each ship.\n\n");
	printf("Enter the cells that you would like to place the carrier ship: \n\n");

	int count = 0; //to distinguish the coordinates of each point 

carrier_placement: 

	for (int i = 0; i < (CARRIER_LENGTH * 2); i += 2) { //iterates to get all points (rows and columns) of the ship type
		printf("Row of point number %d: ", ++count);
		scanf("%d", &row);
		printf("Column of point number %d: ", count);
		scanf("%d", &col);

		if ((row >= 0 && row <= 9) && (col >= 0 && col <= 9)) { //if entered positions are valid on board
			carrier_cords[i] = row; //first value assigned as the row of the carrier ship
			carrier_cords[i + 1] = col; //second value assigned as the column of the carrier ship
			//allows one array to store BOTH row and column value
		}
		else {
			printf("Invalid placement...\n"); //position doesn't exist on player board
			goto carrier_placement; //allows the user to enter until valid
		}

	}
	if (check_placement(board, carrier_cords, CARRIER_LENGTH) == 0) { //if placement is invalid
		printf("Invalid placement, please try again. \n");
		goto carrier_placement; //allows the user to enter until valid
	}

	//if placement is valid

	for (int i = 0; i < (CARRIER_LENGTH * 2); i += 2) {
		board[carrier_cords[i]][carrier_cords[i + 1]] = 'c'; //reassigns the validated, user-inputed positions as 'C' coresponding to carrier
	}
	printf("\n Player 1 Board: \n\n");
	print_board(board, NUM_ROWS, NUM_COLS); //prints the updated board to console

battleship_placement:

	printf("Enter the cells that you would like to place the battleship: \n\n");

	count = 0; //reinitialize point counter to zero


	for (int i = 0; i < (BATTLESHIP_LENGTH * 2); i += 2) { //iterates to get all points (rows and columns) of the ship type
		printf("Row of point number %d: ", ++count); //++ count to start at point 1 and increase from there
		scanf("%d", &row);
		printf("Column of point number %d: ", count);
		scanf("%d", &col);

		if ((row >= 0 && row <= 9) && (col >= 0 && col <= 9)) { //if entered positions are valid on board
			battleship_cords[i] = row; //first value assigned as the row of the battleship
			battleship_cords[i + 1] = col; //second value assigned as the column of the battleship
			//allows one array to store BOTH row and column value
		}
		else {
			printf("Invalid placement...\n"); //position doesn't exist on player board
			goto battleship_placement; //allows the user to enter until valid
		}

	}
	if (check_placement(board, battleship_cords, BATTLESHIP_LENGTH) == 0) { //if placement is invalid
		printf("Invalid placement, please try again. \n");
		count = 0; //resets count
		goto battleship_placement; //allows the user to enter until valid
	}

	//if placement is valid

	for (int i = 0; i < (BATTLESHIP_LENGTH * 2); i += 2) {
		board[battleship_cords[i]][battleship_cords[i + 1]] = 'b'; //reassigns the validated, user-inputed positions as 'b' coresponding to battleship
	}
	printf("\n Player 1 Board: \n\n");
	print_board(board, NUM_ROWS, NUM_COLS); //prints the new board to console


cruiser_placement:

	printf("Enter the cells that you would like to place the cruiser: \n\n");

	count = 0; //reinitialize point counter to zero


	for (int i = 0; i < (CRUISER_LENGTH * 2); i += 2) { //iterates to get all points (rows and columns) of the ship type
		printf("Row of point number %d: ", ++count); //++ count to start at point 1 and increase from there
		scanf("%d", &row);
		printf("Column of point number %d: ", count);
		scanf("%d", &col);

		if ((row >= 0 && row <= 9) && (col >= 0 && col <= 9)) { //if entered positions are valid on board
			cruiser_cords[i] = row; //first value assigned as the row of the cruiser
			cruiser_cords[i + 1] = col; //second value assigned as the column of the cruiser
			//allows one array to store BOTH row and column value
		}
		else {
			printf("Invalid placement...\n"); //position doesn't exist on player board
			goto cruiser_placement; //allows the user to enter until valid
		}

	}
	if (check_placement(board, cruiser_cords, CRUISER_LENGTH) == 0) { //if placement is invalid
		printf("Invalid placement, please try again. \n");
		count = 0; //resets count
		goto cruiser_placement; //allows the user to enter until valid
	}

	//if placement is valid

	for (int i = 0; i < (CRUISER_LENGTH * 2); i += 2) {
		board[cruiser_cords[i]][cruiser_cords[i + 1]] = 'r'; //reassigns the validated, user-inputed positions as 'r' coresponding to cruiser
	}
	printf("\n Player 1 Board: \n\n");
	print_board(board, NUM_ROWS, NUM_COLS); //prints the new board to console

submarine_placement:

	printf("Enter the cells that you would like to place the submarine: \n\n");

	count = 0; //reinitialize point counter to zero


	for (int i = 0; i < (SUBMARINE_LENGTH * 2); i += 2) { //iterates to get all points (rows and columns) of the ship type
		printf("Row of point number %d: ", ++count); //++ count to start at point 1 and increase from there
		scanf("%d", &row);
		printf("Column of point number %d: ", count);
		scanf("%d", &col);

		if ((row >= 0 && row <= 9) && (col >= 0 && col <= 9)) { //if entered positions are valid on board
			submarine_cords[i] = row; //first value assigned as the row of the submarine
			submarine_cords[i + 1] = col; //second value assigned as the column of the submarine
			//allows one array to store BOTH row and column value
		}
		else {
			printf("Invalid placement...\n"); //position doesn't exist on player board
			goto submarine_placement; //allows the user to enter until valid
		}

	}
	if (check_placement(board, submarine_cords, SUBMARINE_LENGTH) == 0) { //if placement is invalid
		printf("Invalid placement, please try again. \n");
		count = 0; //resets count
		goto submarine_placement; //allows the user to enter until valid
	}

	//if placement is valid

	for (int i = 0; i < (SUBMARINE_LENGTH * 2); i += 2) {
		board[submarine_cords[i]][submarine_cords[i + 1]] = 's'; //reassigns the validated, user-inputed positions as 's' coresponding to submarine
	}
	printf("\n Player 1 Board: \n\n");
	print_board(board, NUM_ROWS, NUM_COLS); //prints the new board to console

destroyer_placement:

	printf("Enter the cells that you would like to place the destroyer: \n\n");

	count = 0; //reinitialize point counter to zero


	for (int i = 0; i < (DESTROYER_LENGTH * 2); i += 2) { //iterates to get all points (rows and columns) of the ship type
		printf("Row of point number %d: ", ++count); //++ count to start at point 1 and increase from there
		scanf("%d", &row);
		printf("Column of point number %d: ", count);
		scanf("%d", &col);

		if ((row >= 0 && row <= 9) && (col >= 0 && col <= 9)) { //if entered positions are valid on board
			destroyer_cords[i] = row; //first value assigned as the row of the destroyer
			destroyer_cords[i + 1] = col; //second value assigned as the column of the destroyer
			//allows one array to store BOTH row and column value
		}
		else {
			printf("Invalid placement...\n"); //position doesn't exist on player board
			goto destroyer_placement; //allows the user to enter until valid
		}

	}
	if (check_placement(board, destroyer_cords, DESTROYER_LENGTH) == 0) { //if placement is invalid
		printf("Invalid placement, please try again. \n");
		count = 0; //resets count
		goto destroyer_placement; //allows the user to enter until valid
	}

	//if placement is valid

	for (int i = 0; i < (DESTROYER_LENGTH * 2); i += 2) {
		board[destroyer_cords[i]][destroyer_cords[i + 1]] = 'd'; //reassigns the validated, user-inputed positions as 'd' coresponding to destroyer
	}
	printf("\n Player 1 Board: \n\n");
	print_board(board, NUM_ROWS, NUM_COLS); //prints the new board to console


}


//Function Name: int check_placement(char board[][NUM_COLS], int coordinates[], int ship_size)
//Description: Checks to insure that the user's ship coordinate input is valid. 
//Parameters: Player board array, user-inputed coordinates of ship, length of ship.
//Output: Integer coresponding to whether or not input is valid. If a zero is returned, placement is invalid. 

int check_placement(char board[][NUM_COLS], int coordinates[], int ship_size) {

	int valid_rows = 1, valid_columns = 1, count = 0;

	int row_check = coordinates[0];
	int col_check = coordinates[1];
	int row_cordinates[100] = { 0 };
	int column_cordinates[100] = { 0 };

	//check rows, all row coordinates must be equal to be valid (horizontal array placement)
	//checks every two (i+=2) because every other value coresponds to horizontal placement/row
	for (int i = 0; i < ship_size * 2; i += 2) {
		row_cordinates[count] = coordinates[i];
		if (coordinates[i] != row_check) {
			valid_rows = 0;
		}
		count++;
	}

	//check columns

	count = 0; //reinitialize count to 0
	for (int i = 1; i < ship_size * 2; i += 2) { //iterate to check each column until ship size is reached
		column_cordinates[count] = coordinates[i];
		if (coordinates[i] != col_check) {
			valid_columns = 0;
		}
		count++; 
	}

	if ((valid_rows && !valid_columns)) {
		bubble_sort(row_cordinates, ship_size); 
		for (int i = 1; i < ship_size; i++) {
			if ((column_cordinates[i] > column_cordinates[i - 1] + 1) || board[row_cordinates[i]][column_cordinates[i]] != '~') { //if position is already used or values aren't consecutive
				return 0; //Invalid placement.
			}
		}
		return 1;

	}
	else if ((!valid_rows && valid_columns)) {
		bubble_sort(row_cordinates, ship_size);
		for (int i = 1; i < ship_size; i++) {
			if ((row_cordinates[i] > row_cordinates[i - 1] + 1) || board[row_cordinates[i]][column_cordinates[i]] != '~') {
				return 0; //Invalid placement
			}
		}
		return 1; //Placement is valid
	}

	else if ((!valid_rows && valid_columns)) {
		bubble_sort(row_cordinates, ship_size);
		for (int i = 1; i < ship_size; i++) {
			if ((row_cordinates[i] > row_cordinates[i - 1] + 1) || board[row_cordinates[i]][column_cordinates[i]] != '~') {
				return 0; //Invalid placement
			}
		}
		return 1; //Placement is valid
	}
	return 0; //returns to function declaration
}

//Function Name: void bubble_sort(int arr[], int size)
//Description: Sorts values of the placement of ships in order to confirm that points are consecutive. 
//Parameters: Row coordinate array.
//Output: N/A indirectly modifies the coordinate array to be in order. 


void bubble_sort(int arr[], int size) {
	int temp = 0; //initialize temp variable
	for (int i = 0; i < size; i++) { //compares values of all positions in the array
		for (int j = 0; j < size; j++) {
			if (arr[j] > arr[j + 1]) { //if the second value in the array is greater than the first
				temp = arr[j]; //stores value that is greater than the proceeding value
				arr[j] = arr[j + 1]; //stores lower proceeding value in correct position
				arr[j + 1] = temp; //proceeding value is redefined as the greater value
			}
		}
	}
}

//Function Name: void random_place(char board[NUM_ROWS][NUM_COLS])
//Description: Randomly places fleet on board. 
//Parameters: Character array board. 
//Output: Randomly generated board. 

void random_place(char board[NUM_ROWS][NUM_COLS]) {

	int row_start = 0, col_start = 0; //integers coresponding to cordinates chosen
	int direction = 0; //decides whether ships will be horizontally or vertically oriented

	bool placing = false;


	int ship_lengths[5] = { CARRIER_LENGTH, BATTLESHIP_LENGTH, CRUISER_LENGTH, SUBMARINE_LENGTH, DESTROYER_LENGTH };
	char ship_symbols[5] = { 'c', 'r', 'b', 's', 'd' };

		for (int i = 0; i < 5; i++) { //iterates to place each ship
			placing = true;
			horizontal_or_vertical(&direction); //randomly determines horizontal (0) or vertical orientation (1)

			do {
				generate_random_point(direction, ship_lengths[i], &row_start, &col_start);
				if (detect_collision(board, direction, ship_lengths[i], row_start, col_start) == 1)  { //determines if position is valid
					place_ship(board, NUM_ROWS, NUM_COLS, ship_lengths[i], ship_symbols[i], direction, row_start, col_start); //if valid, places ship 
					placing = false; 
				}

			} while (placing); //continues to randomly place ships 
		}

	}

//Function Name: void generate_random_point(int direction, int ship_length, int* row, int* column)
//Description: Generates random rows and columns to place ships. 
//Parameters: Variables coresponding to direction, ship lengths, pointers to row and column variables. 
//Output: Random row and columns for ships. 

void generate_random_point(int direction, int ship_length, int* row, int* column) {
	if (direction == 1) { //if random direction is horizontal
		*row = rand() % NUM_ROWS; //generates a random row to place ship
		*column = rand() % (NUM_COLS - ship_length + 1); 
	}
	else if (direction == 0) {
		*column = rand() % NUM_COLS; //generates a random column to place ship
		*row = rand() % (NUM_ROWS - ship_length + 1);
	}
}

//Function Name: void horizontal_or_vertical(int* direction)
//Description: Randomly determines if boat will be placed horizontally or vertically. 
//Parameters: Pointer to direction variable (for random placement)
//Output: 0 for horizontal, 1 for vertical. 


void horizontal_or_vertical(int* direction) {
	int value = rand() % 2; //returns either a 0 (horizontal) or a 1 (vertical)

	*direction = value; 
}
//Function Name: int detect_collision(char board[ ][NUM_COLS], int direction, int ship_length, int row, int col)
//Description: Determines if boat placement is valid (not taken already by another ship)
//Parameters: Character array board, direction of ship, ship length, row and column of ship. 
//Output: Determines if placement is valid or not. 

int detect_collision(char board[ ][NUM_COLS], int direction, int ship_length, int row, int col) {
	int valid = 0;
	if (direction == 0) {//if direction is horizontal
		for (int i = 0; i < ship_length; i++) {
			if (board[row][col+i] != '~') { //if placement not available
				valid = 0; //invalid
			}
			else if (board[row][col+i] == '~') //placement is available
				valid = 1; //placement is valid
		}
	}
	if (direction == 1) { //if direction is vertical
		for (int i = 0; i < ship_length; i++) {
			if (board[row + i][col] != '~') { //placement is available
				valid = 0; //placement is invalid
			}
			else if (board[row + i][col] == '~') {
				valid = 1; //placement is valid
			}
		}
	}
	return valid;
}

//Function Name: void place_ship(char board[][NUM_COLS], tint num_rows, int num_cols, int ship_lengths, char ship_symbols, int direction, int row_start, int col_start)
//Description: If random ship placement is valid, symbols/token printed on board
//Parameters: Character array board, number of rows/columns, ship lengths + symbols, ship direction, row and column start. 
//Output: Places ships on board. 

void place_ship(char board[][NUM_COLS], int num_rows, int num_cols, int ship_lengths, char ship_symbols, int direction, int row_start, int col_start) {
	if (direction == 0) {
		for (int i = 0; i < ship_lengths; i++) {
			board[row_start][col_start+i] = ship_symbols; //prints the ship symbol starting at the correct row and subsequent columns
		}
	}
	else if (direction == 1) {
		for (int i = 0; i < ship_lengths; i++) {
			board[row_start+i][col_start] = ship_symbols; //prints the ship symbol starting at the correct column and subsequent rows
		}
	}
}

//Function Name: int check_shots(char board[][NUM_COLS], int x_position, int y_position, char* ship_hit)
//Description: Checks if player 1's shots hit any of the computer's fleet. 
//Parameters: Computer's board, cordinates of shot, ship_hit pointer variable
//Output: Marks hits or misses, informs player of a repeated shot

int check_shots(char board[][NUM_COLS], int x_position, int y_position, char* ship_hit) {
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (i == x_position && j == y_position) {
				if (board[i][j] != '~') { //if spot is not empty 
					if (board[i][j] != 'M' && board[i][j] != '*') {//if spot on board isn't already shot at (M), or hit (*_
						*ship_hit = board[i][j]; //dereferences ship_hit address, marks ship as hit at location[i][j]
						return 1; //returns was hit as 1 >ship is hit
					}
					else if (board[i][j] == 'M' || board[i][j] == '*') { //if spot has already been shot at
						printf("You already shot there silly! Guess you wasted a turn... :P\n"); //ridicule the player >:)
						if (board[i][j] == 'M') {
							return 0;  //remains the spot as a miss
						}
						if (board[i][j] == '*') {
							return 1; //remains the spot as a hit
						}
					}
					
				}
			}
		}
	}
	return 0; //returns was_hit as 0, ship was not hit

}

//Function Name: int check_if_sunk(char board[][NUM_COLS], char ship_token)
//Description: Determines if any ships have been sunk after each shot. 
//Parameters: Character array of player board, ship token/character symbol to look for. 
//Output: Determines if a ship was sunk (1 if sunk, 0 if not)

int check_if_sunk(char board[][NUM_COLS], char ship_token) {
	int token_count = 0;
	for(int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (board[i][j] == ship_token) { //if any ship symbol is left (unchanged to a hit symbol)
				token_count++;
			}
		}
	}
	if (token_count == 1) {
		return 1; //If at least one ship symbol left, boat is not yet sunk
	}
	return 0; 

}

//Function Name: void update_board(char board[][NUM_COLS], int x_position, int y_position, int was_hit)
//Description: Updates player or computer board based on whether shots were hits or misses. 
//Parameters: Character array board, cordinates of shots, hit or miss variable. 
//Output: Marks the shot as either a hit "*" or a miss "M"

void update_board(char board[][NUM_COLS], int x_position, int y_position, int was_hit) {
	char ship_token = '\0';
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (i == x_position && j == y_position) {
				if (was_hit) {
					board[i][j] = '*';
				}
				else {
					board[i][j] = 'M';
				}
			}
		}
	}
}

//Function Name: int determine_winner(char board[][NUM_COLS])
//Description: Determines if either player has won by checking the number of hits. (17 to destroy all ships)
//Parameters: Character array board. 
//Output: 1 if a win, 0 if not a win. 

int determine_winner(char board[][NUM_COLS]) {
	int hit_count = 0;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (board[i][j] == '*') {
				hit_count++;
			}
		}
	}
	if (hit_count == 17) { //if all ship symbols (total 17) have been hit (replaced by *)
		return 1; //win
	}
	return 0; //not win
}

//Function Name: void output_current_move(FILE* outfile, int starts_first, int x_position, int y_position, int was_hit, int was_sunk)
//Description: Writes the summary of each move to the battleship.log file. 
//Parameters: Outfile, variables coresponding to current player, shot cordinates, info on whether it was a hit/miss or sunk any ships. 
//Output: Writes all above info to the oufile.

void output_current_move(FILE* outfile, int starts_first, int x_position, int y_position, int was_hit, int was_sunk) {
	fprintf(outfile, "Gameplay Events: \n");

	switch (starts_first) {
	case 1: //prints details regarding player 1
		if (was_hit) {
			if (was_sunk) {
				fprintf(outfile, "Player 1 shot at (%d, %d). It was a hit, the computer's ship sank. \n", x_position, y_position);

			}
			else {
				fprintf(outfile, "Player 1 shot at (%d, %d). It was a hit. \n", x_position, y_position);
			}
		}
		else {
			fprintf(outfile, "Player 1 shot at (%d, %d). It was a miss. \n", x_position, y_position);
		}
		break;
	case 2: //prints details regarding computer
		if (was_hit) {
			if (was_sunk) {
				fprintf(outfile, "The computer shot at (%d, %d). It was a hit, the player's ship sank. \n", x_position, y_position);

			}
			else {
				fprintf(outfile, "The computer shot at (%d, %d). It was a hit. \n", x_position, y_position);
			}
		}
		else {
			fprintf(outfile, "The computer shot at (%d, %d). It was a miss. \n", x_position, y_position);
		}
		break;
	}
	

}

//Function Name: void output_stats(FILE* outfile, Stats player_one, Stats player_two)
//Description: Writes all the game stats/data to the log file. 
//Parameters: Pointer to outfile, stats struct of player one and computer. 
//Output: Writes all round info to the outfile. 

void output_stats(FILE* outfile, Stats player_one, Stats player_two) {

	fprintf(outfile, "\nPlayer One Stats: \n\n");
	fprintf(outfile, "Number of Hits: %d \n", player_one.number_hits);
	fprintf(outfile, "Number of Misses: %d \n", player_one.number_misses);
	fprintf(outfile, "Total Shots: %d \n", player_one.total_shots);
	fprintf(outfile, "Hit to Miss Ratio Percentage: %d \n\n", player_one.hits_misses);

	fprintf(outfile, "Computer Stats: \n\n");
	fprintf(outfile, "Number of Hits: %d \n", player_two.number_hits);
	fprintf(outfile, "Number of Misses: %d \n", player_two.number_misses);
	fprintf(outfile, "Total Shots: %d \n", player_two.total_shots);
	fprintf(outfile, "Hit to Miss Ratio Percentage: %d \n\n", player_two.hits_misses);

}
