#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>

#define BOARD_SIZE 8
#define MAX_MOVES BOARD_SIZE *BOARD_SIZE

// Define the possible states of a field on the board
enum FieldState
{
    EMPTY,
    OCCUPIED
};

// Create two boards to represent the knight's tour:
// board stores the state of each field (empty or occupied),
// while move_board stores the move number for each field
enum FieldState board[BOARD_SIZE][BOARD_SIZE] = {EMPTY};
enum FieldState move_board[BOARD_SIZE][BOARD_SIZE] = {EMPTY};

// Define the possible moves for the knight
int x_possibilities[8] = {1, 2, 2, 1, -1, -2, -2, -1};
int y_possibilities[8] = {-2, -1, 1, 2, 2, 1, -1, -2};

// Function to check if a given move is valid
bool is_valid_move(int x, int y);

// Function to print the move_board, which shows the move number for each field
void print_board(void);

// Function to count the number of valid moves from a given position
int count_valid_moves(int x, int y);

// Comparison function to be used by qsort to sort the list of possible moves
int compare_moves(const void *a, const void *b);

// Function to recursively attempt to solve the knight's tour, starting
// from a given move and position
bool solve_knight_tour(int move, int x, int y);

int main(int argc, char *argv[])
{
    // Check that exactly 2 arguments (x and y) were given
    if (argc != 3)
    {
        printf("Bitte geben Sie genau 2 Parameter (x und y) ein.\n");
        return 1;
    }

    // Convert the arguments to integers to get the starting position
    int start_x = atoi(argv[1]);
    int start_y = atoi(argv[2]);

    printf("Startposition: (%d, %d)\n", start_x, start_y);

    // Check that the starting position is valid
    if (!is_valid_move(start_x, start_y))
    {
        printf("Ungültige Startposition.\n");
        return 1;
    }

    // Mark the starting position as occupied and the move number as 1
    board[start_x][start_y] = OCCUPIED;
    move_board[start_x][start_y] = 1;

    // Attempt to solve the knight's tour starting from the given position
    if (solve_knight_tour(1, start_x, start_y))
    {
        // If a solution was found, print the move_board
        print_board();
        return 0;
    }
    printf("Etwas ist schiefgelaufen.\n");
}

// Function to print the move_board
void print_board(void)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%2d ", move_board[i][j]);
        }
        printf("\n");
    }
}

// Function to check if a given move is valid
// (i.e., within the bounds of the board and the field is empty)
bool is_valid_move(int x, int y)
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == EMPTY;
}

int count_valid_moves(int x, int y)
{
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        int new_x = x + x_possibilities[i];
        int new_y = y + y_possibilities[i];

        if (is_valid_move(new_x, new_y))
        {
            count++;
        }
    }
    return count;
}

int compare_moves(const void *a, const void *b)
{
    int *move_a = (int *)a;
    int *move_b = (int *)b;
    int count_a = count_valid_moves(move_a[0], move_a[1]);
    int count_b = count_valid_moves(move_b[0], move_b[1]);
    return count_a - count_b;
}

bool solve_knight_tour(int move, int x, int y)
{
    // mark the current square as occupied
    board[x][y] = OCCUPIED;
    move_board[x][y] = move;

    // check if we have made all the required moves
    if (move == MAX_MOVES)
    {
        return true;
    }

    // generate the list of all possible moves from the current square
    int moves[8][2] = {0};
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        int new_x = x + x_possibilities[i];
        int new_y = y + y_possibilities[i];
        if (is_valid_move(new_x, new_y))
        {
            moves[count][0] = new_x;
            moves[count][1] = new_y;
            count++;
        }
    }

    // sort the list of possible moves by the number of valid moves available
    qsort(moves, count, sizeof(moves[0]), compare_moves);

    // try each possible move in turn
    for (int i = 0; i < count; i++)
    {
        int new_x = moves[i][0];
        int new_y = moves[i][1];
        if (solve_knight_tour(move + 1, new_x, new_y))
        {
            // if we can complete the tour from this position, we are done
            return true;
        }
    }

    // if we have tried all possible moves and cannot complete the tour, backtrack
    board[x][y] = EMPTY;
    return false;
}
