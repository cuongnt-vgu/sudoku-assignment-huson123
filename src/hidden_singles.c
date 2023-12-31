#include "hidden_singles.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSED_CELLS 81

static HiddenSingle processed_cells[MAX_PROCESSED_CELLS];
static int processed_count = 0;

// Helper function to check if a value is a candidate in a cell
bool Is_candidate(Cell *cell, int value) {
    int *candidates = get_candidates(cell);
    if (cell->num_candidates==1){
            free(candidates);
            return false;
        }
    //printf("%d ",candidates[0]);
    for (int i=0; i<cell->num_candidates;i++){
        
        //printf("%d ",value);
        //printf("%d ",candidates[i]);
        if (candidates[i]==value){
            free(candidates);
            return true;
        }
    }
    free(candidates);
    return false;
}

// Helper function to count the occurrences of a candidate in a list of cells
int count_candidate_occurrences(Cell **p_cells, int size, int value) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        //printf("\n");
        if (Is_candidate(p_cells[i], value)) {
            count++;
            //printf("count of %d is", value);
            //printf("%d ",count);
            }
    }
    
    return count;
}

bool is_processed(Cell *cell) {
    for (int i = 0; i < processed_count; ++i) {
        if (processed_cells[i].p_cell == cell) {
            return true;
        }
    }
    return false;
}

int find_hidden_single_values(Cell **p_cells, int *hidden_single_values) {
    int hidden_single_counter = 0;

    for (int value = 1; value <= 9; value++) {
        int count = count_candidate_occurrences(p_cells, BOARD_SIZE, value);
        if (count == 1) {
            hidden_single_values[hidden_single_counter++] = value;
        }
        
    }
    //printf("%d ",hidden_single_counter);
    return hidden_single_counter;
}

void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter) {
    int hidden_single_values[BOARD_SIZE];
    int num_hidden_singles = find_hidden_single_values(p_cells, hidden_single_values);
    //*p_counter = *p_counter + num_hidden_singles;
    /*
    for (int i = 0; i < num_hidden_singles; i++) {
        int value = hidden_single_values[i];
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (Is_candidate(p_cells[j], value)) {
                p_hidden_singles[*p_counter].p_cell = p_cells[j];
                p_hidden_singles[*p_counter].value = value;
                (*p_counter)++;
                break;  // Move to the next hidden single value
            }
        }
    }
    */
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (p_cells[i]->num_candidates > 1 && !is_processed(p_cells[i])) {
            int *candidates = get_candidates(p_cells[i]);  
            for (int j = 0; j < num_hidden_singles; j++) {
                if (Is_candidate(p_cells[i], hidden_single_values[j])) {
                    p_hidden_singles[*p_counter].p_cell = p_cells[i];
                    p_hidden_singles[*p_counter].value = hidden_single_values[j];
                    (*p_counter)++;
                    processed_cells[processed_count].p_cell = p_cells[i];
                    processed_cells[processed_count].value = hidden_single_values[j];
                    (processed_count)++;

                    break;
                }
            }
            free(candidates);
        }
    }
}


int hidden_singles(SudokuBoard *p_board) {
    HiddenSingle hidden_singles[BOARD_SIZE * BOARD_SIZE];
    int counter = 0;


    
    // Check hidden singles in rows
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &counter);
    }
    
    // Check hidden singles in columns
    for (int i = 0; i < 9; i++) {
        find_hidden_single(p_board->p_cols[i], hidden_singles, &counter);
    }
    
    // Check hidden singles in boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &counter);
    }
    
    
    for (int i = 0; i < counter; i++) 
    {
        Cell *cell = hidden_singles[i].p_cell;
        int value = hidden_singles[i].value;
        set_candidates(cell, &value, 1);
    }
    
    // Return the number of hidden singles
    return counter;
}
