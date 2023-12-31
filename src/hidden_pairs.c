#include "hidden_pairs.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a hidden pair
typedef struct {
    Cell *p_cells[2];
    int values[2];
} HiddenPair;

// Global variable to track processing status
bool processed[BOARD_SIZE][BOARD_SIZE];

// Function to check whether a cell has been processed
bool is_processedd(Cell *cell) {
    return processed[cell->row_index][cell->col_index];
}

// Function to mark a cell as processed
void mark_processed(Cell *cell) {
    processed[cell->row_index][cell->col_index] = true;
}

// Function to find hidden pairs in a unit (row, column, or box)
int find_hidden_pairs(Cell **p_cells, HiddenPair *hidden_pairs) {
    int num_pairs = 0;

    // Reset the processed flag for each cell in the unit
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            processed[i][j] = false;
        }
    }

    for (int i = 0; i < BOARD_SIZE - 1; i++) {
        if (p_cells[i]->num_candidates == 2 && !is_processedd(p_cells[i])) {
            for (int j = i + 1; j < BOARD_SIZE; j++) {
                if (p_cells[j]->num_candidates == 2 && !is_processedd(p_cells[j])) {
                    int *candidates_i = get_candidates(p_cells[i]);
                    int *candidates_j = get_candidates(p_cells[j]);

                    // Check if the candidates are the same
                    if ((candidates_i[0] == candidates_j[0] && candidates_i[1] == candidates_j[1]) ||
                        (candidates_i[0] == candidates_j[1] && candidates_i[1] == candidates_j[0])) {
                        hidden_pairs[num_pairs].p_cells[0] = p_cells[i];
                        hidden_pairs[num_pairs].p_cells[1] = p_cells[j];
                        hidden_pairs[num_pairs].values[0] = candidates_i[0];
                        hidden_pairs[num_pairs].values[1] = candidates_i[1];
                        num_pairs++;

                        // Mark the processed cells
                        mark_processed(p_cells[i]);
                        mark_processed(p_cells[j]);
                        
                    }
                    
                    free(candidates_i);
                    free(candidates_j);
                }
            }
        }
    }

    return num_pairs;
}

// Function to apply hidden pairs to the Sudoku board
void apply_hidden_pairs(HiddenPair *hidden_pairs, int num_pairs) {
    for (int i = 0; i < num_pairs; i++) {
        for (int j = 0; j < 2; j++) {
            Cell *cell = hidden_pairs[i].p_cells[j];
            int *candidates = get_candidates(cell);

            // Remove other candidates from the cell
            for (int k = 0; k < cell->num_candidates; k++) {
                int value = candidates[k];

                // Check if the candidate is not part of the hidden pair
                if (value != hidden_pairs[i].values[0] && value != hidden_pairs[i].values[1]) {
                    unset_candidate(cell, value);
                }
            }

            free(candidates);
        }
    }
}

// Function to find and apply hidden pairs in the entire Sudoku board
int hidden_pairs(SudokuBoard *p_board) {
    HiddenPair hidden_pairs[BOARD_SIZE * BOARD_SIZE];
    int num_pairs = 0;

    // Check hidden pairs in rows
    for (int i = 0; i < BOARD_SIZE; i++) {
        num_pairs += find_hidden_pairs(p_board->p_rows[i], hidden_pairs + num_pairs);
    }

    // Check hidden pairs in columns
    for (int i = 0; i < BOARD_SIZE; i++) {
        num_pairs += find_hidden_pairs(p_board->p_cols[i], hidden_pairs + num_pairs);
    }

    // Check hidden pairs in boxes
    for (int i = 0; i < BOARD_SIZE; i++) {
        num_pairs += find_hidden_pairs(p_board->p_boxes[i], hidden_pairs + num_pairs);
    }

    // Apply hidden pairs to the board
    apply_hidden_pairs(hidden_pairs, num_pairs);

    return num_pairs;
}
