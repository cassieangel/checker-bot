/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: [Angeline Cassie Ganily]
  Dated:     [04 October 2021]

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <string.h>

/*-------------------------------- DEFINES -----------------------------------*/
#define BOARD_SIZE           8      // board size
#define ROWS_WITH_PIECES     3      // num of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE           1      // one piece cost
#define COST_TOWER           3      // one tower cost
#define TREE_DEPTH           3      // minimax tree depth
#define COMP_ACTIONS        10      // num of computed actions

#define WHITE              'w'      // white 
#define BLACK              'b'      // black 
#define ASCII_A             65      // ascii value of A
#define ASCII_0             48      // ascii value of 0
#define ASCII_DASH          45      // ascii value of -

#define DIRECTION            8      // max number of direction a tower can move
#define NORTH               -1      // goind to the north notation
#define SOUTH                1      // goint to the south notation
#define EAST                 1      // going to the east notation
#define WEST                -1      // goint to the west notation

#define MAX_LEN              6      // max-string of each move          
/*----------------------------------------------------------------------------*/

/*----------------------------- DECLARATIONS ---------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

struct Node { // struct for linked list node
  char move[MAX_LEN];
  struct Node* next;
};

int board_cost(board_t board);
int check_error(char moves_array[], board_t board, int action);
int eror_six(board_t board, int src_col, int src_row, int tgt_col, int tgt_row);
int game_end(board_t board);
int score(board_t board, char cell);
void action_detail(int action, char moves_array[]);
void add_node(struct Node** ref, const char* move);
void board_details(board_t board);
void delete_list(struct Node** ref);
void initialise_board(board_t board);
void print_board(board_t board);
void print_error(int error);
void print_moves(board_t board, char moves_array[], int action);
void stage_moves(board_t board, char moves_array[]);
void update_board(char moves_array[], board_t board, int action);

int find_move(board_t board, int action, 
    char legal_move[BOARD_SIZE * BOARD_SIZE][MAX_LEN]);
int minimax(board_t position, int depth, int maxi_player, char best[]);

int diff(int num1, int num2);
int even(int num);
void blank_line(void);
void horizontal_break(void);
void indent(void);
void line_break(void);
void new_action_marker(void);
void new_line(void); 
void spacing(void);
void ta_daa(void);
void vertical_break(void);
/*----------------------------------------------------------------------------*/

/*----------------------------- MAIN FUNCTION --------------------------------*/
int
main(int argc, char *argv[]) {
  board_t board;
  char moves_array[MAX_LEN]; // (5+1) num of character per input string

  initialise_board(board);
  stage_moves(board, moves_array); // STAGE O, 1, 2

  return EXIT_SUCCESS;  // exit program with the success code
}
/*----------------------------------------------------------------------------*/

/*------------------------------ STAGE ZERO ----------------------------------*/
void
initialise_board(board_t board) {
  /* initialise starting checker board */

  int row, col, row_num, col_int;

  for (row=0; row<BOARD_SIZE; row++) {
    for (col=0; col<BOARD_SIZE; col++) {
      row_num = row+1;
      col_int = col+1; 

      /* first 3 row (white area) */
      if (row_num<=ROWS_WITH_PIECES) {
        /* row 1 or 3, even column */
        if ((row_num==1 || row_num==3) && even(col_int)) { 
          board[row][col] = CELL_WPIECE;
        /* row 2, odd column */ 
        } else if (row_num==2 && !even(col_int)) {
          board[row][col] = CELL_WPIECE;
        } else {
          board[row][col] = CELL_EMPTY;
        }

      /* last 3 row (black area) */
      } else if (row_num>(BOARD_SIZE-ROWS_WITH_PIECES)) {
        /* last and 3rd last row, odd column */
        if ((row_num==BOARD_SIZE || row_num==BOARD_SIZE-2) && !even(col_int)) { 
          board[row][col] = CELL_BPIECE;
        /* 2nd last row, even column */ 
        } else if (row_num==BOARD_SIZE-1 && even(col_int)) {
          board[row][col] = CELL_BPIECE;
        } else {
          board[row][col] = CELL_EMPTY;
        }

      /* middle area */
      } else {
        board[row][col] = CELL_EMPTY;
      }
    }
  }
}

int
score(board_t board, char cell) {
  /* total score of board */

  int score = 0;

  for (int row=0; row<BOARD_SIZE; row++) {
    for (int col=0; col<BOARD_SIZE; col++) {
      if (cell==WHITE) {
        if (board[row][col]==CELL_WPIECE) {
          score+=1;
        } else if (board[row][col]==CELL_WTOWER) {
          score+=3;
        }
      } else if (cell==BLACK) {
        if (board[row][col]==CELL_BPIECE) {
          score+=1;
        } else if (board[row][col]==CELL_BTOWER) {
          score+=3;
        }
      }
    }
  }
  return score;
}

void 
board_details(board_t board) {
  /* board size, number of black and white piece at the start of the game */

  printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
  printf("#BLACK PIECES: %d\n", score(board, BLACK));
  printf("#WHITE PIECES: %d\n", score(board, WHITE));
}

void 
print_board(board_t board) {
  /* prints the board with format */

  int row, col, row_num, col_int, ascii;

  /* COLUMN HEADER */
  indent(); 
  ascii = ASCII_A; // start of ASCII values for alphabets 
  for (col=0; col<BOARD_SIZE; col++) {
    printf("   %c", ascii);
    ascii++;
  }
  horizontal_break();

  /* PRINT EACH ROW'S CHARACTERS */
  for (row=0; row<BOARD_SIZE; row++) {
    row_num = row+1;
    printf(" %d |", row_num);

    for (col=0; col<BOARD_SIZE; col++) { 
      col_int = col+1; 
      printf(" %c ", board[row][col]);
      vertical_break();

      if (col_int%BOARD_SIZE==0) {
        horizontal_break();
      }
    }
  }
}

void 
stage_moves(board_t board, char moves_array[]) {
  /* execute the moves given by the input */

  int action = 1;
  struct Node* move_list = NULL; 

  /* create linked list */
  while (scanf("%s", moves_array) == 1) {
    add_node(&move_list, moves_array);
    if (strlen(moves_array) != 5 && 
      (*(moves_array) == 'A' || *(moves_array) == 'P')) {
      break;
    }
  }

  board_details(board);
  print_board(board);

  struct Node* current_move = move_list; 
  while (current_move != NULL) {
    /* STAGE 0 */
    if (strlen(current_move->move) == 5) {
      print_moves(board, current_move->move, action);
      action++;
    }

    /* STAGE 1 */
    else if (*(current_move->move) == 'A') {
      char best_move[MAX_LEN];

      minimax(board, TREE_DEPTH, action, best_move);
      update_board(best_move, board, action);
      line_break();

      new_action_marker();
      action_detail(action, best_move);
      printf("BOARD COST: %d\n", board_cost(board));
      print_board(board);

      action++;
    }

    /* STAGE 2 */
    else if (*(current_move->move) == 'P') {
      for (int i=0; i<COMP_ACTIONS; i++) {
        char best_move[MAX_LEN];

        minimax(board, TREE_DEPTH, action, best_move);
        update_board(best_move, board, action);
        line_break();

        new_action_marker();
        action_detail(action, best_move);
        printf("BOARD COST: %d\n", board_cost(board));
        print_board(board);

        /* check if game has end */ 
        int game_status = game_end(board);
        if (game_status == 1) { 
          printf("BLACK WIN!"); 
          exit(EXIT_SUCCESS); 

        } else if (game_status == 2) {
          printf("WHITE WIN!"); 
          exit(EXIT_SUCCESS);
        }

        action++;
      }
    }

    /* check if game has end */ 
    int game_status = game_end(board);
    if (game_status == 1) { 
      printf("BLACK WIN!\n"); 
      exit(EXIT_SUCCESS); 
    }      
    else if (game_status == 2) {
      printf("WHITE WIN!\n"); 
      exit(EXIT_SUCCESS);
    }
    
    current_move = current_move->next;
  }

  free(current_move);
  delete_list(&move_list); 
}

void
print_moves(board_t board, char moves_array[], int action) {
  int error = check_error(moves_array, board, action);
  if (error!=0) {
    print_error(error);
    exit(EXIT_FAILURE);
  }
  else { // no errors
    update_board(moves_array, board, action);
    line_break();
    action_detail(action, moves_array);
    printf("BOARD COST: %d\n", board_cost(board));
    print_board(board);
  }
}

void
action_detail(int action, char moves_array[]) {
  if (!even(action)) {
    printf("BLACK ACTION #%d: %s\n", action, moves_array); // black is odd
  } else if (even(action)) {
    printf("WHITE ACTION #%d: %s\n", action, moves_array); // white is even
  }
}

int
board_cost(board_t board) {
  int black_score = score(board, BLACK);
  int white_score = score(board, WHITE);

  if (black_score == 0) {
    return INT_MIN;
  } else if (white_score == 0) {
    return INT_MAX;
  } else {
    return score(board, BLACK) - score(board, WHITE);
  }
}

void 
add_node(struct Node** ref, const char* move) {
  /* add node to linked list */

  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
  struct Node* iterator = *ref;

  if (new_node == NULL) {
    printf("FAIL IN MEMORY ALLOCATION!");
    exit(EXIT_FAILURE);
  }

  strcpy(new_node->move, move);
  new_node->next = NULL;

  /* check if the given head reference is null or empty */
  if (*ref == NULL) {
    *ref = new_node;
    return;
  }

  while (iterator->next != NULL)
    iterator = iterator->next;
  
  iterator->next = new_node;
  return;
}

void 
delete_list(struct Node** ref) {
  /* delete linked list */

  struct Node* node = *ref;
  struct Node* temp_node = NULL;

  while (node != NULL) {
    temp_node = node->next;
    free(node);
    node = temp_node;
  }

  *ref = NULL;
}

int
check_error(char moves_array[], board_t board, int action) {
  /* check if a move is valid or not */

  char src_letter, tgt_letter, src_content, tgt_content;
  int src_num, tgt_num;
  int src_col, tgt_col, src_row, tgt_row;

  src_letter = moves_array[0]; 
  src_num = atoi(&moves_array[1]); 
  tgt_letter = moves_array[3];
  tgt_num = atoi(&moves_array[4]);

  src_col = src_letter-ASCII_A;
  src_row = src_num-1;
  tgt_col = tgt_letter-ASCII_A;
  tgt_row = tgt_num-1;

  src_content = board[src_row][src_col];
  tgt_content = board[tgt_row][tgt_col];

  /* ERRORS */
  if (src_col<0 || src_row<0 || src_col>=BOARD_SIZE|| src_row>=BOARD_SIZE) 
    return 1;
  else if (tgt_col<0 || tgt_row<0 || tgt_col>=BOARD_SIZE || tgt_row>=BOARD_SIZE) 
    return 2;
  else if (src_content == CELL_EMPTY) 
    return 3;
  else if (tgt_content != CELL_EMPTY) 
    return 4;
  else if (!even(action)) {
    /* if odd action, cell content cannot be white piece/tower */
    if (src_content == CELL_WPIECE || src_content == CELL_WTOWER) {
      return 5;
    }
  } else if (even(action)) {
    /* if even action, cell content cannot be black piece/tower */
    if (src_content == CELL_BPIECE || src_content == CELL_BTOWER) {
      return 5;
    }
  } 
  if (eror_six(board, src_col, src_row, tgt_col, tgt_row)) {
    return 6;
  } 

  return 0;
}

void
print_error(int error) {
  if (error==1) {
    printf("ERROR: Source cell is outside of the board.\n");
  } else if (error==2) {
    printf("ERROR: Target cell is outside of the board.\n");
  } else if (error==3) {
    printf("ERROR: Source cell is empty.\n");
  } else if (error==4) {
    printf("ERROR: Target cell is not empty.\n");
  } else if (error==5) {
    printf("ERROR: Source cell holds opponent's piece/tower.\n");
  } else if (error==6) {
    printf("ERROR: Illegal action.\n");
  }
}

int
eror_six(board_t board, int src_col, int src_row, int tgt_col, int tgt_row) {
  int src_content;
  int row_vector, opp_row, opp_col;

  src_content = board[src_row][src_col]; // getting the char
  row_vector = src_row - tgt_row;

  opp_row = (src_row + tgt_row)/2; // find midpoint
  opp_col = (src_col + tgt_col)/2;
  char opp = board[opp_row][opp_col];

  /* check direction */
  if (row_vector > 0 && src_content==CELL_WPIECE) { // north, no wpiece 
    return 1;
  } else if (row_vector < 0 && src_content==CELL_BPIECE) { // south, no bpiece
    return 1;
  }

  /* moves with a jump */
  if (diff(src_col, tgt_col) == 2 && diff(src_row, tgt_row) == 2) { 
    if (src_content==CELL_BPIECE || src_content==CELL_BTOWER) {
      if (opp==CELL_WPIECE || opp==CELL_WTOWER) {
        return 0;
      }
    } else if (src_content==CELL_WPIECE || src_content==CELL_WTOWER) {
      if (opp==CELL_BPIECE || opp==CELL_BTOWER) {
        return 0;
      }
    }
  } else if (diff(src_col, tgt_col) == 1 && diff(src_row, tgt_row) == 1) {
    return 0;
  }
  return 1;
}

void 
update_board(char moves_array[], board_t board, int action) { 
  /* update board with the given move */

  char src_cell;
  int opp_row, opp_col;
  int src_col, tgt_col, src_row, tgt_row;

  src_col = moves_array[0]-ASCII_A;
  src_row = atoi(&moves_array[1])-1; 
  tgt_col = moves_array[3]-ASCII_A;
  tgt_row = atoi(&moves_array[4])-1;

  opp_row = (src_row + tgt_row)/2; 
  opp_col = (src_col + tgt_col)/2;

  src_cell = board[src_row][src_col];

  /* if pieces moves to the ends of the board, update to tower */
  if (tgt_row==0 && src_cell==CELL_BPIECE) {
    src_cell = CELL_BTOWER;
  } else if (tgt_row==BOARD_SIZE-1 && src_cell==CELL_WPIECE) {
    src_cell = CELL_WTOWER;
  }

  /* moves two step diagonally */
  if (diff(src_col, tgt_col)==2 && diff(src_row, tgt_row)==2) {  
    board[tgt_row][tgt_col] = src_cell;
    board[opp_row][opp_col] = CELL_EMPTY;
    board[src_row][src_col] = CELL_EMPTY;

  /* moves one step diagonally */
  } else if (diff(src_col, tgt_col)==1 && diff(src_row, tgt_row)==1) {
    board[tgt_row][tgt_col] = src_cell;
    board[src_row][src_col] = CELL_EMPTY;
  }
}
/*----------------------------------------------------------------------------*/

/*---------------------------- STAGE ONE & TWO -------------------------------*/
int
find_move(board_t board, int action, 
char legal_move[BOARD_SIZE * BOARD_SIZE][MAX_LEN]) {
  /* find valid move for the given board */

  int row_moves[DIRECTION] = {NORTH, 2*NORTH, SOUTH, 2*SOUTH, SOUTH, 2*SOUTH, 
                             NORTH, 2*NORTH};
  int col_moves[DIRECTION] = {EAST, EAST*2, EAST, EAST*2, WEST, WEST*2, WEST, 
                             WEST*2};

  int row, col, i;
  char cell;
  int tgt_num=0, tgt_let=0;
  char src_num, src_let, possible_move[5+1];

  int current_row = 0;
  for (row=0; row<BOARD_SIZE; row++) {
    for (col=0; col<BOARD_SIZE; col++) {
      cell = board[row][col];
      if (cell!=CELL_EMPTY) {
        src_num = row + 1;
        src_let = col + ASCII_A;

        for (i=0; i<=DIRECTION; i++) {
          tgt_num = row + row_moves[i] + 1;
          tgt_let = col + col_moves[i] + ASCII_A;

          /* create possible move as string */
          possible_move[0] = src_let;
          possible_move[1] = src_num+ASCII_0;
          possible_move[2] = ASCII_DASH;
          possible_move[3] = tgt_let;
          possible_move[4] = tgt_num+ASCII_0;
          possible_move[5] = 0;

          if (check_error(possible_move, board, action) == 0) {
            strcpy(legal_move[current_row], possible_move); 
            current_row++;
          }
        }
      }
    }
  }

  return current_row;
}
   
/* The code below is an adapted and modified version of 
    Title: Minimax Search Algorithm 
    Author: Sebastian Lague 
    Access Date: October 2021
    Availability: https://pastebin.com/VSehqDM3
for Foundation of Algorithm, Semester 2 2021, Assigment 2. */
int 
minimax(board_t position, int depth, int maxi_player, char best[MAX_LEN]) {
  int eval, max_eval, min_eval, move_count, i;

  /* base case; if depth is 0 and game ends (value 1->black or 2->white) */
  if (depth == 0 || game_end(position) > 0) { 
    return board_cost(position);
  }

  if (!even(maxi_player)) { // black's action
    max_eval = INT_MIN;
    char best_move[MAX_LEN];

    /* try for each legal move in the current position of the board */
    char legal_moves[BOARD_SIZE * BOARD_SIZE][MAX_LEN];
    move_count = find_move(position, 1, legal_moves); // (1) odd, find black
    
    for (i = 0; i < move_count; i++) {
      board_t new_position; // create a temporal position
      memcpy(new_position, position, sizeof(char) * BOARD_SIZE * BOARD_SIZE);
      update_board(legal_moves[i], new_position, 1);
      
      eval = minimax(new_position, depth-1, 0, best); // recursion

      if (eval > max_eval) {
        max_eval = eval;
        strcpy(best_move, legal_moves[i]); // update as best move
      }
    }

    if (depth == TREE_DEPTH) { 
      strcpy(best, best_move);
    }

    return max_eval;
  }
  else { // white's action
    min_eval = INT_MAX;
    char best_move[MAX_LEN];

    char legal_moves[BOARD_SIZE * BOARD_SIZE][MAX_LEN];
    move_count = find_move(position, 0, legal_moves); // (0) even, find black
    
    for (i = 0; i < move_count; i++) {
      board_t new_position;
      memcpy(new_position, position, sizeof(char) * BOARD_SIZE * BOARD_SIZE);
      update_board(legal_moves[i], new_position, 0);
      
      eval = minimax(new_position, depth-1, 1, best);   

      if (eval < min_eval) {
        min_eval = eval;
        strcpy(best_move, legal_moves[i]);
      }
    }
    
    if (depth == TREE_DEPTH) {
      strcpy(best, best_move);
    }

    return min_eval;
  }
}

int 
game_end(board_t board) {
  /* return 0 if game can still continue, 1 if black wins, 2 if white wins */ 

  int row, col, w_legal_move, b_legal_move;
  char legal_move[BOARD_SIZE * BOARD_SIZE][MAX_LEN];

  int w_left = 0;
  int b_left = 0;

  /* rule:
     1. if it is the opponent’s turn and they cannot take action,
     2. either because no their pieces and towers are left on the
        board or because no legal move or capture is possible */

  for (row = 0; row<BOARD_SIZE; row++) {
    for (col = 0; col < BOARD_SIZE; col++) {
      if (board[row][col] == CELL_WPIECE || board[row][col] == CELL_WTOWER) {
        w_left++;
      } else if (board[row][col] == CELL_BPIECE || 
        board[row][col] == CELL_BTOWER) {
        b_left++;
      }
    }
  }

  w_legal_move = find_move(board, 0, legal_move);
  b_legal_move = find_move(board, 1, legal_move);

  if (w_left == 0 || w_legal_move == 0)
    return 1; // black wins
  else if (b_left == 0 || b_legal_move == 0)
    return 2; // white wins

  return 0; // if 0, game continues
}
/*----------------------------------------------------------------------------*/

/*-------------------------- OTHER HELPER FUNCTION ---------------------------*/
int 
even(int num) {
  if (num%2==0) {
    return 1;
  }
  return 0;
}

int 
diff(int num1, int num2) {
  int diff;
  if(num1>num2) {
    diff = num1-num2;
  } else {
    diff = num2-num1;
  }
  return diff;
}

void
new_action_marker(void) {
  printf("*** ");
}

void
indent(void) {
	printf("  ");
}

void
vertical_break(void) {
	printf("|");
}

void
horizontal_break(void) {
	printf("\n   +---+---+---+---+---+---+---+---+\n");
}

void
line_break(void) {
  printf("=====================================\n");
}

void
new_line(void) {
	printf("\n");
}

void
blank_line(void) {
	printf("\n");
}

void
ta_daa(void) {
	printf("ta daa!!!\n");
}
void
spacing(void) {
	printf(" ");
}

/*-------------------------------- THE END -----------------------------------*/

/* algorithms are fun! ヾ(^∇^ゞ) */