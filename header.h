//Name: Sydnee Boothby
//Assignment: Programming Assignment 6: Battleship
//Date: 03/22/2024
//Description: Use user based functions, arrays, pointers, repetition/iteration loops/structures, file reading and other c functions learned thus far in CptS 121 to produce battleship gameplay. 
//Instructor: Andy O'Fallon
//TA: Claire Monsoon

#define _CRT_SECURE_NO_WARNINGS //prevent printf and scanf errors
#include <stdio.h> //basic print, scan functions
#include <time.h> //allow for random functions
#include <string.h> //allow functions with strings
#include <stdlib.h>
#include <stdbool.h> 

#define NUM_ROWS 10
#define NUM_COLS 10

#define CARRIER_LENGTH 5
#define BATTLESHIP_LENGTH 4
#define CRUISER_LENGTH 3
#define SUBMARINE_LENGTH 3
#define DESTROYER_LENGTH 2

//struct for storing stats of each player

typedef struct {
	int number_hits, number_misses, total_shots; 
	double hits_misses;

}Stats;

/////
//Function Name: 
//Description: 
//Parameters: 
//Output:  
//////

//Function Name: void menu_choice(int *choice)
//Description: Displays main menu options to console and uses a pointer to store the user's choice
//Parameters: Address of the choice variable
//Output: Saves the user's choice to be used in the menu switch statement. 

void menu_choice(int *choice);

//Function Name: void display_rules(void)
//Description: Prints the rules of battleship to the console. 
//Parameters: N/A
//Output: N/A, just prints rules

void display_rules(void);

//Function Name: void battleship_gameplay(void)
//Description: Main function, stores all the functions neccessary to play a game of battleship
//Parameters: N/A
//Output: N/A Runs a round of battleship.

//void battleship_gameplay(void); //CHANGE MAYBE FHDGJFHJHFJKLASD

void battleship_gameplay(char player_1_gameboard[][NUM_COLS], char player_2_gameboard[][NUM_COLS], char player_2_display_gameboard[][NUM_COLS]);

//Function Name: void initialize_game_board(char board[10][10], int num_rows, int num_columns)
//Description: Initializes the players gameboard at the start of each round.
//Parameters: Character array coresponding to the player's board, number of rows and number of columns.
//Output: A blank gameboard.

void initialize_game_board(char board[10][10], int num_rows, int num_columns);

//Function Name: void print_board(char board[10][10], int num_rows, int num_columns) 
//Description: Prints the current board to the console.
//Parameters: Character array coresponding to player board and the max number of rows and columns
//Output: N/A prints board to the console

void print_board(char board[10][10], int num_rows, int num_columns);

//Function Name: void select_who_starts_first(int* starts_first);
//Description: Selects which player starts the game first.
//Parameters: Pointer coresponding to variable of which player goes first.
//Output: N/A just determines who starts first. 

void select_who_starts_first(int* starts_first);

//Function Name: void choose_place_type(int* place_type)
//Description: Prompts the user to chose how they wish to place their fleet.
//Parameters: Pointer to variable coresponding to choice.
//Output:  N/A, indirectly modifies choice variable.

void choose_place_type(int* place_type);

//Function Name: void manual_place(char board[][NUM_COLS])
//Description: Allows the user to manually choose where to place their fleet.
//Parameters: Array of the player's gameboard
//Output: Indirectly modifies the player's gameboard array.

void manual_place(char board[][NUM_COLS]);

//Function Name: int check_placement(char board[][NUM_COLS], int coordinates[], int ship_size)
//Description: Checks to insure that the user's ship coordinate input is valid. 
//Parameters: Player board array, user-inputed coordinates of ship, length of ship.
//Output: Integer coresponding to whether or not input is valid. If a zero is returned, placement is invalid. 

int check_placement(char board[][NUM_COLS], int coordinates[], int ship_size);

//Function Name: void bubble_sort(int arr[], int size)
//Description: Sorts values of the placement of ships in order to confirm that points are consecutive. 
//Parameters: Row coordinate array.
//Output: N/A indirectly modifies the coordinate array to be in order. 

void bubble_sort(int arr[], int size);

void random_place(char board[NUM_ROWS][NUM_COLS]);

void generate_random_point(int direction, int ship_length, int* row, int* column);

void horizontal_or_vertical(int* direction); 

int detect_collision(char board[][NUM_COLS], int direction, int ship_length, int row, int col);

void place_ship(char board[][NUM_COLS], int num_rows, int num_cols, int ship_lengths, char ship_symbols, int direction, int row_start, int col_start);
