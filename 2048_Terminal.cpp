/**
 * 2048 in terminal!
 * 
 * To compile and build, please refer the instructions mentioned in the README.md
*/

#include <ncurses/ncurses.h>
#include <string>

/* global grid of size 4x4 */
const int size = 4;
int grid[size][size] = {0};

/**
 * Function: empty_cell_finder
 * Return: True or False
 * Description: Returns true if an empty cell is
 * found and also sets the row and column values.
 * Empty cells are represented as zero.
*/

bool empty_cell_finder(int &row, int &col){
    for(row = size-1; row >= 0; row--){
        for(col = size-1; col >= 0; col--){
            if(grid[row][col] == 0){
                return true;
            }
        }
    }
    return false;
}

/**
 * Function: input_generator
 * Return: (nothing)
 * Description: Assigns either 2 or 4 to the empty
 * cell depending on the row and col values set 
 * by the empty_cell_finder().
*/

void input_generator(int &row, int &col){
    int random_number = 2;
    if(row%2 == 0){
        random_number = 4;
    }
    grid[row][col] = random_number;
}

/**
 * Function: cell_merger
 * Return: (nothing)
 * Description: Depending upon the below directions,
 * w = up
 * a = left
 * s = down
 * d = right
 * the neighbouring cells are merged in an 
 * orderly fashion only if the cells have same 
 * values and the newly merged cell is the sum
 * of those two cells.
 * Example: If two cells have the same value (say 2)
 * such that one is above the other and the 
 * direction of merging is 'w' then they are 
 * combined and the merged cell will have a value
 * equal to 4.
*/

unsigned int cell_merger(char direction, unsigned int score){
    int i, j, max;
    max = size - 1;

    for(i=0; i<size; i++){
        for(j=0; j<size; j++){
            switch(direction){
                /**
                 * Merge same cells from top to bottom 
                 * starting from grid[0][0]
                */
                case 'w':
                    if(grid[i][j] == grid[i+1][j]){
                        grid[i][j] += grid[i+1][j];
                        score += grid[i][j];
                        grid[i+1][j] = 0;
                    }
                    break;

                /**
                 * Merge same cells from left to right
                 * starting from grid[0][0]
                */
                case 'a':
                    if(grid[i][j] == grid[i][j+1]){
                        grid[i][j] += grid[i][j+1];
                        score += grid[i][j];
                        grid[i][j+1] = 0;
                    }
                    break;

                /**
                 * Merge same cells from bottom to top
                 * starting from grid[3][3]
                */
                case 's':
                    if(grid[max-i][max-j] == grid[max-i-1][max-j]){
                        grid[max-i][max-j] += grid[max-i-1][max-j];
                        score += grid[max-i][max-j];
                        grid[max-i-1][max-j] = 0;
                    }
                    break;
        
                /**
                 * Merge same cells from right to left
                 * starting from grid[3][3]
                */
                case 'd':
                    if(grid[max-i][max-j] == grid[max-i][max-j-1]){
                        grid[max-i][max-j] += grid[max-i][max-j-1];
                        score += grid[max-i][max-j];
                        grid[max-i][max-j-1] = 0;
                    }
                    break;
            }
        }
    }
    return score;
}

/**
 * Function: consolidate
 * Return: (nothing)
 * Description: Group all the non-empty cells 
 * depending on the direction of grouping
 * w = up
 * a = left
 * s = down
 * d = right
 * Example: If the direction is 'w' then all the 
 * non-empty cells will be moved to the upper 
 * side of the grid and all the empty cells 
 * will be moved to the lower side of the grid
*/

void consolidate(char direction){
    int i, j, empty_cell_count, max;
    max = size - 1;

    for(i=0; i<size; i++){
        empty_cell_count = 0;
        for(j=0; j<size; j++){
            switch(direction){
                // Consolidate cells in upward direction
                case 'w':            
                    if(grid[j][i] == 0){
                        empty_cell_count++;
                    }
                    else{
                        if(empty_cell_count != 0){
                            grid[j-empty_cell_count][i] = grid[j][i];
                            grid[j][i] = 0;
                        }
                    }
                    break;
            
                // Consolidate cells in left direction
                case 'a':
                    if(grid[i][j] == 0){
                        empty_cell_count++;
                    }
                    else{
                        if(empty_cell_count != 0){
                            grid[i][j-empty_cell_count] = grid[i][j];
                            grid[i][j] = 0;
                        }
                    }
                    break;

                // Consolidate cells in downward direction
                case 's':
                    if(grid[max-j][max-i] == 0){
                        empty_cell_count++;
                    }
                    else{
                        if(empty_cell_count != 0){
                            grid[max-j+empty_cell_count][max-i] = grid[max-j][max-i];
                            grid[max-j][max-i] = 0;
                        }
                    }
                    break;

                // Consolidate cells in right direction
                case 'd':
                    if(grid[max-i][max-j] == 0){
                        empty_cell_count++;
                    }
                    else{
                        if(empty_cell_count != 0){
                            grid[max-i][max-j+empty_cell_count] = grid[max-i][max-j];
                            grid[max-i][max-j] = 0;
                        }
                    }
                    break;
            }
        }
    }
}

/**
 * Function: check_possible_move
 * Return: True or False
 * Description: This method is called only if the
 * empty_cell_finder() returns false, i.e. when the
 * grid is full, to see if the game can be continued
 * or not.
*/
bool check_possible_move(){
    int i, j;

    /**
     * Check the cells in the form of an inverted 'L'
     * (the bottom part of the 'L' is on the top)
    */
    for(i=0; i<size-1; i++){
        for(j=0; j<size-1; j++){
            if((grid[i][j] == grid[i][j+1]) || (grid[i][j] == grid[i+1][j])){
                return true;
            }
        }
    }

    /**
     * Check the right boundary cells
    */
    for(i=0; i<size-1; i++){
        if(grid[i][size-1] == grid[i+1][size-1]){
            return true;
        }
    }

    /**
     * Check the bottom boundary cells
    */
    for(j=0; j<size-1; j++){
        if(grid[size-1][j] == grid[size-1][j+1]){
            return true;
        }
    }
    
    return false;
}

/**
 * Function: print_grid
 * Return: (nothing)
 * Description: Prints the grid along with '+', '-', 
 * '|', to show the row and columns neatly. 
 * Empty cells are displayed as blank spaces.
*/

void print_grid(int &row, int &col){
    for(int i=1; i<=size; i++){
        if(i>1){
            for(int k=1, m=6; k<=23; k++){
                if(k == m){
                    printw("+");
                    m += 6;
                }
                else{
                    printw("-");
                }
            }
            printw("\n");
        }
        for(int j=1; j<=size; j++){
            if(j>1){
                printw("|");
            }
            if(grid[i-1][j-1] != 0){
                printw("  ");
                printw("%d", grid[i-1][j-1]);
                if(row == i-1 && col == j-1){
                    printw("* ");    
                }
                else{
                    printw("  ");
                }
            }
            else{
                printw("     ");
            }
        }
        printw("\n");
    }
}

/**
 * Function: print_rules
 * Return: (nothing)
 * Description: Prints the moves to play the game 
 * along with the previous direction (previous_move)
 * used by the user.
*/

void print_rules(char previous_move, unsigned int score){
    std::string keys[5] = {"w", "a", "s", "d"};
    std::string actions[5] = {"up", "left", "down", "right"};
    std::string temp;

    printw("\n\nScore: %d", score);
    printw("\n\nPress one of the keys to merge and move the numbers\n");
    for(int i=0; i<4; i++){
        temp = previous_move;
        if(keys[i].compare(temp) == 0){
            printw("%s = %s  <= previous move\n", keys[i].c_str(), actions[i].c_str());
        }
        else{
            printw("%s = %s\n", keys[i].c_str(), actions[i].c_str());
        }
    }
    printw("\n* indicates newly filled cell\n");
    printw("\nPress any other key to quit\n");
}

int main(void){
    int game_start_choice, move = 0, row, col;
    unsigned int score = 0;

    // initialize ncurses mode
    initscr();

    // turn off echoing 
    noecho();

    // allow contents to be scrolled
    scrollok(stdscr, TRUE);

    printw("Welcome to\n\n");

    // 2048 game artwork
    printw("   2222         0000           444        8888   \n");
    printw(" 22    22     00    00        4 44      88    88 \n");
    printw("      22     00      00      4  44       88  88  \n");
    printw("    22       00      00     4   44         88    \n");
    printw("  22         00      00    444444444     88  88  \n");
    printw("22            00    00          44      88    88 \n");
    printw("222222222       0000            44        8888   \n");

    printw("\n\nPress Y to start or any other key to quit the game");

    // Draw all the output on the screen
    refresh();

    // Check if game_start_choice is 'Y' or 'y'
    game_start_choice = getch();
    if(game_start_choice == 89 || game_start_choice == 121){
        while(true){
            clear();
            if(empty_cell_finder(row, col) || check_possible_move()){
                input_generator(row, col);
                print_grid(row, col);
                print_rules(move, score);
                refresh();
                move = getch();

                // Check if move is either 
                // 'W' or 'w' or 
                // 'A' or 'a' or
                // 'S' or 's' or
                // 'D' or 'd'
                if(move == 87 || move == 119 || move == 65 || move == 97 || move == 83 || move == 115 || move == 68 || move == 100){
                    consolidate(move);
                    score = cell_merger(move, score);
                    consolidate(move);
                }
                else{
                    break;
                }
            }
            else{
                print_grid(row, col);
                printw("\n\nNo more possible moves.. Game Over!");
                printw("\nPress any key to quit");
                refresh();
                getch();
                break;
            }
        }
    }

    // Stop ncurses mode and end the window
    endwin();
    return 0;
}
